#include "LionBoss.h"
#include "IA.h"
#include "StateMac.h"
#include "Game.h"
#include "Attack.h"
#include "TimeBomb.h"
#include "Rand.h"
#include "Projectile.h"
#include "Player.h"
#include "Trigger.h"
#include "ShakeBehavior.h"
#include "TerrainBody.h"
#include <memory>
#include <cmath>

#define PI_DEG 180.0f / M_PI

LionBoss::LionBoss(GameObject& associated)
    : GameObject(associated)
{
    LIONSIZE = Vec2(200, 250);

    CLOSEX = 50.0f;
    MIDDLEX = 150.0f;
    FARX = 300.0f;

    LASERDURATION = 3.0f;
    LASERCHARGE   = 1.0f;
    LASERCOOLDOWN = 1.5f;
    TOWERDURATION = 3.0f;
    BALLSDURATION = 2.5f;
    SHOCKWAVEDURATION = 4.0f;
    
    LASERDAMAGE = 25;
    LASERKNOCK = Vec2();
    LASERSIZE = Vec2(750,325);
    LASERPREP = Vec2(100, 50);

    BALLSDAMAGE = 15;
    BALLSKNOCK = Vec2();
    BALLSSPEED = -600.0f;
    BALLSYSPEED = -1000.0f;
    BALLSIZE = Vec2(70,60);

    TOWERDAMAGE = 15;
    TOWERKNOCK = Vec2(-10, -15);

    WAVESIZE = Vec2(50,250);
    SHOCKWAVEDAMAGE = 25;
    SHOCKWAVEKNOCK = Vec2(10, 40);
    SHOCKWAVESPEED = 400;

    EXPLOSIONDURATION = 0.8f;

    LIONMAXHEALTH = 200;
    currentHealth = LIONMAXHEALTH;
    activated = false;

    auto& currstate = Game::GetInstance().GetCurrentState();
    
    auto target = currstate.GetObject(C_ID::Player);
    if(target.expired())
    {
        std::cout << "PLAYER DEVE SER DECLARADO ATES DO LIONBOSS\n";
        exit(0);
    }

    auto stmac = new StateMachine(associated);
    auto lionbrainz = new IA(associated, target.lock().get(), 1.5f);
    auto cld = new Collider(associated);
    auto terr = new TerrainBody(associated, false);
    associated.AddComponents({lionbrainz, stmac, terr, cld});

    auto anm = new AnimNode("assets/img/Lion/closed_mouth.png", 1,1,Vec2(1,1), false, false);
    // anm->SetSize(LIONSIZE.x, LIONSIZE.y);
    anm->SetScaleX(2,2);
    stmac->AddNode(IDLE, anm);
    stmac->ChangeState(IDLE);

    anm = new AnimNode("assets/img/Lion/open_mouth.png", 1, LASERDURATION+LASERCHARGE,Vec2(1,1), false, false);
    // anm->SetSize(LIONSIZE.x, LIONSIZE.y);
    anm->SetScaleX(2,2);
    stmac->AddNode(LASERING, anm); stmac->AddTransition(LASERING, IDLE);
    
    anm = new AnimNode("assets/img/Lion/low_tail.png", 1,BALLSDURATION/2,Vec2(1,1), false, false);
    // anm->SetSize(LIONSIZE.x, LIONSIZE.y);
    anm->SetScaleX(2,2);
    stmac->AddNode(BALLING, anm); stmac->AddTransition(BALLING, IDLE);
    
    auto lionw = anm->GetWidth()/2.0f;
    lionbrainz->SetActions({
            {Vec2(FARX+lionw, 0), 2.5f, LASERDURATION + LASERCHARGE + LASERCOOLDOWN},
            {Vec2(MIDDLEX+lionw, 0), 1, BALLSDURATION},
            {Vec2(CLOSEX+lionw, 0), 1, TOWERDURATION},
            {Vec2(MIDDLEX+lionw, 0), 1, SHOCKWAVEDURATION}
        }
    );

}

void LionBoss::Update(float dt)
{
    if(!activated)
    {
        auto& currstate = Game::GetInstance().GetCurrentState();
        auto target = currstate.GetObject(C_ID::Player, &currstate.rigidArray);
        // std::cout << target.lock().get()->box.GetCenter().Distance(associated.box.GetCenter()) << '\n';
        if(target.lock().get()->box.GetCenter().Distance(associated.box.GetCenter()) < 500)
        {
            auto lionb = (IA*)associated.GetComponent(C_ID::IA);
            lionb->SetActionTimer((float)2.0f);
            activated = true;
        }
        return;
    }

    auto lionbrainz = (IA*)associated.GetComponent(C_ID::IA);
    auto stmac = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    int choice = lionbrainz->selectedAction;
    // if(choice != -1)
    //     std::cout << choice << '\n';
    switch(choice)
    {
        case LASER: Laser(); stmac->ChangeState(LASERING); break;
        case BALLS: FlameBalls(); stmac->ChangeState(BALLING); break;
        case TOWER: FlameTower(); stmac->ChangeState(LASERING); break;
        case SHOCKWAVE: ShockWave(); stmac->ChangeState(BALLING); break;
        default:
        break;
    }
}

void LionBoss::ShockWave()
{
    auto& st = Game::GetInstance().GetCurrentState();
    
    GameObject* wave_go = new GameObject();
        Sprite* spr = new Sprite(*wave_go, "assets/img/Lion/wave.png", 1, 0, -1); 
        spr->SetSize(WAVESIZE.x, WAVESIZE.y);

        wave_go->box.x = associated.box.x - spr->GetWidth();
        wave_go->box.y = associated.box.y + associated.box.h - spr->GetHeight();

        Projectile* wave_proj = new Projectile(*wave_go, 7.0f, -180.0f, SHOCKWAVESPEED);
        auto cld = (Collider*)wave_go->GetComponent(C_ID::Collider);
        cld->type = C_ID::Hitbox;
        Attack* atk = new Attack(*wave_go, SHOCKWAVEDAMAGE, SHOCKWAVEKNOCK, &associated);
        wave_go->AddComponents({spr, wave_proj, atk});
    st.bulletArray.emplace_back(wave_go);

}

void LionBoss::Laser()
{
    auto& st = Game::GetInstance().GetCurrentState();
    GameObject* laser_prep = new GameObject;
        Sprite* spr = new Sprite(*laser_prep, "assets/img/laser.png", 1, 0, -1);
        TimeBomb* tmb = new TimeBomb(*laser_prep, LASERCHARGE);
        spr->SetSize(LASERPREP.x, LASERPREP.y);
        laser_prep->box.x = associated.box.x - spr->GetWidth();
        laser_prep->box.y = associated.box.y;
        laser_prep->AddComponents({spr, tmb});
    st.objectArray.emplace_back(laser_prep);
    std::cout << &associated << '\n';
    this->AddComponent(new TimedTrigger(*this, LASERCHARGE,
        [](GameObject& associated)
        {
            auto& st = Game::GetInstance().GetCurrentState();
            auto playerweakptr = st.GetObject(C_ID::Lion, &st.rigidArray);
            LionBoss& assc = (LionBoss&)associated;
            // auto target = &assc.associated;
            GameObject* laser_go = new GameObject();
                Sprite* spr = new Sprite(*laser_go, "assets/img/laser.png", 1, 0, -1);
                spr->SetSize(assc.LASERSIZE.x, assc.LASERSIZE.y);
                laser_go->box.x = assc.associated.box.x - spr->GetWidth();
                laser_go->box.y = assc.associated.box.y;
                Collider* cld = new Collider(*laser_go); cld->type = C_ID::Hitbox;
                TimeBomb* tmb = new TimeBomb(*laser_go,assc.LASERDURATION);
                Attack* laser_atk = new Attack(*laser_go, assc.LASERDAMAGE, assc.LASERKNOCK, &assc.associated);
                auto dpsow = new DisappearOnDeadOwner(*laser_go, playerweakptr);
                laser_go->AddComponents({spr, cld, tmb, laser_atk, dpsow});
            st.bulletArray.emplace_back(laser_go);
        }
    ));

    // this->AddComponent(new TimedTrigger(*this, LASERCHARGE + LASERDURATION, // TODO -- deixar um efeito de "cansado?"
    //     [](GameObject& associated)
    //     {
    //         LionBoss& assc = (LionBoss&)associated;
    //         auto stm = (StateMachine*)assc.GetComponent(C_ID::StateMachine);
    //         stm->ChangeState(LionBoss::IDLE);
    //     }
    // ));
}

void LionBoss::FlameBalls()
{
    auto& st = Game::GetInstance().GetCurrentState();
    GameObject* player_go = st.GetObject(C_ID::Player, &st.rigidArray).lock().get();
    float leftb = player_go->box.x - BALLSIZE.x, rightb = player_go->box.x + BALLSIZE.x;
    Vec2 vecinic = Vec2(BALLSSPEED, BALLSYSPEED);
    
    for(int i = 0; i < 2; i++, leftb += BALLSIZE.x, rightb += BALLSIZE.x)
    {
        float xpred = leftb + (rightb-leftb)*Rand::Get_r();
        float time = abs(xpred - (associated.box.x + associated.box.w))/BALLSSPEED;
        float grav = 2*BALLSYSPEED/time;
        GameObject* balls_go = new GameObject();
            Sprite* spr = new Sprite(*balls_go, "assets/img/laser.png", 1, 0, -1);
            DisappearOnHit* dsp = new DisappearOnHit(*balls_go, &associated);
            Projectile* ballsproj = new Projectile(*balls_go, 5.0f, vecinic.AngleX() * PI_DEG, vecinic.Magnitude(), vecinic.Magnitude(), grav);
            ballsproj->rotSprt = true;
            spr->SetSize(BALLSIZE.x, BALLSIZE.y);
            balls_go->angleDeg = vecinic.AngleX();
            balls_go->box.x = associated.box.x + associated.box.w; 
            balls_go->box.y = associated.box.y + associated.box.h - BALLSIZE.y;
            Attack* ball_atk = new Attack(*balls_go, BALLSDAMAGE, BALLSKNOCK, &associated);
            auto cld = (Collider*)balls_go->GetComponent(C_ID::Collider);
            cld->type = C_ID::Hitbox;
               
            balls_go->AddComponents({dsp, spr, ballsproj, ball_atk});
        st.bulletArray.emplace_back(balls_go);
    }

}

void LionBoss::FlameTower()
{
    auto& st = Game::GetInstance().GetCurrentState();
    GameObject* tower_go = new GameObject();
        Sprite* spr = new Sprite(*tower_go, "assets/img/laser.png", 1,0, -1);
        spr->SetSize(150, 100 + associated.box.h);
        spr->SetFliped(true);
        tower_go->box.x = associated.box.x - spr->GetWidth(); 
        tower_go->box.y = associated.box.y - 100;
        Collider* tower_co = new Collider(*tower_go);
        tower_co->type = C_ID::Hitbox;
        Attack* atk = new Attack(*tower_go, TOWERDAMAGE, TOWERKNOCK, &associated);
        auto dpsow = new DisappearOnDeadOwner(*tower_go, st.GetObject(C_ID::Lion, &st.rigidArray));
        tower_go->AddComponents({spr, tower_co, atk, new TimeBomb(*tower_go, TOWERDURATION), dpsow});
    st.bulletArray.emplace_back(tower_go);
}

bool LionBoss::Is(C_ID type)
{ return type == C_ID::Lion; }

void LionBoss::NotifyCollision(GameObject& other, Vec2 sep)
{
    auto bingus = (Attack*)other.GetComponent(C_ID::Attack);
    if(!bingus || bingus->OwnedBy(&associated))
        return;
    
    TakeDamage(bingus->GetDamage());
}

void LionBoss::TakeDamage(int damage)
{
    currentHealth -= damage;
    
    // std::cout << "ai" << ' ' << currentHealth << '\n';
    if(currentHealth <= 0)
        DIEEE();
    else
        TakeDamageVisuals();
    
}

void LionBoss::TakeDamageVisuals() // TODO -> quando receber um ataque gerar os efeitos (tanto visuais quanto sonoros)
{
    auto spr = associated.GetComponent(C_ID::StateMachine);
    spr->AddComponent(new ShakeBehavior(*spr, Vec2(7,7), 1.0f, 0.2f));
}

void LionBoss::DIEEE()
{
    auto [_, anmptr] = ((StateMachine*)associated.GetComponent(C_ID::StateMachine))->GetCurrent();
    auto spr_go = new GameObject;
        auto spr = new Sprite(*spr_go, "assets/img/aliendeath.png", 4, EXPLOSIONDURATION/4.0f);
        auto tmb = new TimeBomb(*spr_go, EXPLOSIONDURATION);
        auto explsound = new Sound(*spr_go, "assets/audio/boom.wav");

        spr->SetSize(anmptr->GetWidth(), anmptr->GetHeight());
        spr_go->box.x = associated.box.x; spr_go->box.y = associated.box.y;
        spr_go->AddComponents({spr,tmb, explsound});

    Game::GetInstance().GetCurrentState().objectArray.emplace_back(spr_go);
    associated.RequestDelete();
}