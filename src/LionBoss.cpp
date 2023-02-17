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
#include "SpriteSheetNode.h"
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
    TOWERDURATION = 4.5f;
    BALLSDURATION = 2.5f;
    SHOCKWAVEDURATION = 4.0f;
    
    LASERDAMAGE = 25;
    LASERKNOCK = Vec2(-10,0);
    LASERSIZE = Vec2(1250,325);
    LASERPREP = Vec2(100, 50);
    LASERINGRESTFRAMES = 7;
    LASERINGTOCOLLIDE = 19;
    LASERINGACTIVE = 16;
    LASERFRAMETIME = 0.05;

    BALLSDAMAGE = 15;
    BALLSKNOCK = Vec2(-10,0);
    BALLSSPEED = -600.0f;
    BALLSYSPEED = -1000.0f;
    BALLSIZE = Vec2(85,85);
    BALLSFRAMES = 11;
    BALLSFRAMETIME = 0.08f;

    TOWERDAMAGE = 15;
    TOWERKNOCK = Vec2(-10, -15);
    TOWERTOTALFRAMES = 18;
    TOWERFRAMESTOCREATE = 8;
    TOWERFRAMETIME = 0.1f;
    TOWERSIZE = Vec2(1250,250);

    WAVESIZE = Vec2(50,250);
    SHOCKWAVEDAMAGE = 25;
    SHOCKWAVEKNOCK = Vec2(10, 40);
    SHOCKWAVESPEED = 550;
    SHOCKWAVINGFRAMES = 22;
    SHOCKWAVINGXFRAMES = 7;
    SHOCKWAVINGDURATION = 1.0f;

    EXPLOSIONDURATION = 0.8f;
    LIONCONTACTKNOCK = Vec2(-40, 10);
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
    auto atk = new Attack(associated, 0, 0, &associated);
    associated.AddComponents({lionbrainz, stmac, terr, cld, atk});

    auto anm = new AnimNode("assets/img/Lion/closed_mouth.png", 1,1,Vec2(1,1), false, false);
    // anm->SetSize(LIONSIZE.x, LIONSIZE.y);
    anm->SetScaleX(2,2);
    stmac->AddNode(IDLE, anm);
    stmac->ChangeState(IDLE);

    anm = new AnimNode("assets/img/Lion/leao_laser.png", 56, LASERFRAMETIME/1.5f,Vec2(1,1), false, false);
    // anm->SetSize(LIONSIZE.x, LIONSIZE.y);
    anm->SetScaleX(2,2);
    stmac->AddNode(LASERING, anm); stmac->AddTransition(LASERING, IDLE);
    
    anm = new AnimNode("assets/img/Lion/leao_rabo.png", BALLSFRAMES, BALLSFRAMETIME,Vec2(1,1), false, false);
    // anm->SetSize(LIONSIZE.x, LIONSIZE.y);
    anm->SetScaleX(2,2);
    stmac->AddNode(BALLING, anm); stmac->AddTransition(BALLING, IDLE);
    
    anm = new AnimNode("assets/img/Lion/leao_rugido.png", TOWERTOTALFRAMES, TOWERFRAMETIME,Vec2(1,1), false, false);
    // anm->SetSize(LIONSIZE.x, LIONSIZE.y);
    anm->SetScaleX(2,2);
    stmac->AddNode(TOWERING, anm); stmac->AddTransition(TOWERING, IDLE);

    anm = new SSNode("assets/img/Lion/leao_shockwave.png", {0,0, 187*SHOCKWAVINGXFRAMES,208} ,SHOCKWAVINGXFRAMES, SHOCKWAVINGDURATION/SHOCKWAVINGXFRAMES, Vec2(1,1), false, false);
    anm->SetScaleX(2,2);
    stmac->AddNode(SHOCKWAVING, anm); stmac->AddTransition(SHOCKWAVING, SHOCKWAVINGIDLE);

    anm = new SSNode("assets/img/Lion/leao_shockwave.png", {187*SHOCKWAVINGXFRAMES,0, 187*(SHOCKWAVINGFRAMES-SHOCKWAVINGXFRAMES),208} ,SHOCKWAVINGFRAMES - SHOCKWAVINGXFRAMES, (SHOCKWAVEDURATION-SHOCKWAVINGDURATION)/(SHOCKWAVINGFRAMES-SHOCKWAVINGXFRAMES), Vec2(1,1), false, false);
    anm->SetScaleX(2,2);
    stmac->AddNode(SHOCKWAVINGIDLE, anm); stmac->AddTransition(SHOCKWAVINGIDLE, IDLE);

    auto lionw = anm->GetWidth()/2.0f;
    lionbrainz->SetActions(
        {
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
       
        if(target.expired())
        { return; }

        auto targ_go = target.lock().get();
        if(targ_go->box.GetCenter().Distance(associated.box.GetCenter()) < 500)
        {
            Camera::SetCameraFunction(Camera::Stationary);
            ((Player*)targ_go->GetComponent(C_ID::Player))->Bounds=Rect(Camera::pos.x-50.0f, Camera::pos.y, Camera::width, Camera::height);
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
        case TOWER: FlameTower(); stmac->ChangeState(TOWERING); break;
        case SHOCKWAVE: ShockWave(); stmac->ChangeState(SHOCKWAVING); break;
        default:
        break;
    }
}

void LionBoss::ShockWave()
{
    AddComponent(new TimedTrigger(*this, SHOCKWAVINGDURATION,
        [](GameObject& assc)
        {
            LionBoss& lbass = (LionBoss&)assc;
            auto& st = Game::GetInstance().GetCurrentState();
            GameObject* wave_go = new GameObject();
                Sprite* spr = new Sprite(*wave_go, "assets/img/Lion/wave.png", 1, 0.0f, -1); 
                spr->SetSize(lbass.WAVESIZE.x, lbass.WAVESIZE.y);

                wave_go->box.x = lbass.associated.box.x - spr->GetWidth();
                wave_go->box.y = lbass.associated.box.y + lbass.associated.box.h - spr->GetHeight();

                Projectile* wave_proj = new Projectile(*wave_go, 7.0f, -180.0f, lbass.SHOCKWAVESPEED);
                auto cld = (Collider*)wave_go->GetComponent(C_ID::Collider);
                cld->type = C_ID::Hitbox;
                Attack* atk = new Attack(*wave_go, lbass.SHOCKWAVEDAMAGE, 0, &lbass.associated);
                wave_go->AddComponents({spr, wave_proj, atk});
            st.bulletArray.emplace_back(wave_go);
        }
    ));
}

void LionBoss::Laser()
{
    auto& st = Game::GetInstance().GetCurrentState();
    GameObject* laser_go = new GameObject();
        laser_go->depth = 12;
        auto spr = new StateMachine(*laser_go);
        auto node = new AnimNode("assets/img/Lion/malu_laser_start.png", LASERINGTOCOLLIDE, LASERFRAMETIME/2.0f);
        node->SetSize(LASERSIZE.x*LASERINGTOCOLLIDE, LASERSIZE.y);
        spr->AddNode(LASERBEGIN, node); spr->AddTransition(LASERBEGIN,LASERLOOP);
        spr->ChangeState(LASERBEGIN);

        node = new AnimNode("assets/img/Lion/malu_laser_loop.png", 4, LASERFRAMETIME);
        node->SetSize(LASERSIZE.x*4, LASERSIZE.y);
        spr->AddNode(LASERLOOP, node); spr->AddTransition(LASERLOOP, LASERLOOP);

        node = new AnimNode("assets/img/Lion/malu_laser_end.png", LASERINGRESTFRAMES, LASERFRAMETIME);
        node->SetSize(LASERSIZE.x*LASERINGRESTFRAMES, LASERSIZE.y);
        spr->AddNode(LASEREND, node);

        laser_go->box.x = associated.box.x - laser_go->box.w;
        laser_go->box.y = associated.box.y;
        std::cout << laser_go->box << associated.box;

        TimeBomb* tmb = new TimeBomb(*laser_go, (LASERINGTOCOLLIDE + LASERINGACTIVE + LASERINGRESTFRAMES)*LASERFRAMETIME);
        Attack* laser_atk = new Attack(*laser_go, LASERDAMAGE, 0, &associated);
        auto dpsow = new DisappearOnDeadOwner(*laser_go, st.GetObjectPtr(&associated));
        laser_go->AddComponents({spr, tmb, laser_atk, dpsow});
    st.bulletArray.emplace_back(laser_go);

    laser_go->AddComponent(new TimedTrigger(
        *laser_go,
        (LASERINGACTIVE+LASERINGTOCOLLIDE)*LASERFRAMETIME,
        [](GameObject& associated)
        {
            auto& st = Game::GetInstance().GetCurrentState();
            auto lion_go = st.GetObject(C_ID::Lion, &st.enemyArray);
            if(lion_go.expired()) return;

            auto lionboss = (LionBoss*)(lion_go.lock())->GetComponent(C_ID::Lion);
            ((StateMachine*)associated.GetComponent(C_ID::StateMachine))->ChangeState(lionboss->LASEREND);
            associated.RemoveComponent(associated.GetComponent(C_ID::Collider));
        }
    ));
    laser_go->AddComponent(new TimedTrigger(
        *laser_go,
        LASERINGTOCOLLIDE*LASERFRAMETIME,
        [](GameObject& associated)
        {
            Collider* cld = new Collider(associated); cld->type = C_ID::Hitbox;
            associated.AddComponent(cld);
        }
    ));


}

void LionBoss::FlameBalls()
{
    AddComponent(
        new TimedTrigger(*this, BALLSFRAMETIME*5,
            [](GameObject& associated)
            {
                LionBoss& assc = (LionBoss&)associated;
                auto& st = Game::GetInstance().GetCurrentState();
                auto player_weak = st.GetObject(C_ID::Player, &st.rigidArray);
                if(player_weak.expired())
                    return;

                GameObject* player_go = player_weak.lock().get();

                float leftb = player_go->box.x - 3*assc.BALLSIZE.x, rightb = player_go->box.x +player_go->box.w;
                Vec2 vecinic = Vec2(assc.BALLSSPEED, assc.BALLSYSPEED);
                
                for(int i = 0; i < 2; i++, leftb += assc.BALLSIZE.x, rightb += assc.BALLSIZE.x)
                {
                    float xpred = leftb + (rightb-leftb)*Rand::Get_r();
                    float time = abs(xpred - (assc.associated.box.x + assc.associated.box.w))/assc.BALLSSPEED;
                    float grav = 2*assc.BALLSYSPEED/time;
                    GameObject* balls_go = new GameObject();
                        balls_go->angleDeg = 180.0f;
                        balls_go->depth = 15;
                        Sprite* spr = new Sprite(*balls_go, "assets/img/Lion/bola de fogo.png", 4, assc.BALLSFRAMETIME, -1);
                        DisappearOnHit* dsp = new DisappearOnHit(*balls_go, &assc.associated);
                        Projectile* ballsproj = new Projectile(*balls_go, 5.0f, vecinic.AngleX() * PI_DEG, vecinic.Magnitude(), vecinic.Magnitude(), grav);
                        ballsproj->rotSprt = true;
                        spr->SetSize(assc.BALLSIZE.x*4, assc.BALLSIZE.y);
                        balls_go->angleDeg = vecinic.AngleX();
                        balls_go->box.x = assc.associated.box.x + assc.associated.box.w; 
                        balls_go->box.y = assc.associated.box.y + (assc.associated.box.h - assc.BALLSIZE.y)/2.0f;
                        Attack* ball_atk = new Attack(*balls_go, assc.BALLSDAMAGE, 0, &assc.associated);
                        auto cld = (Collider*)balls_go->GetComponent(C_ID::Collider);
                        cld->type = C_ID::Hitbox;
                        balls_go->AddComponents({dsp, spr, ballsproj, ball_atk});
                    st.bulletArray.emplace_back(balls_go);
                }
            }
        )
    );
}

void LionBoss::FlameTower()
{
    auto& st = Game::GetInstance().GetCurrentState();
    auto tower_go = new GameObject;
        tower_go->angleDeg = 270.0f;
        auto spr = new Sprite(*tower_go, "assets/img/Lion/laser_fire_inverted.png", 4, (TOWERFRAMESTOCREATE/4)*TOWERFRAMETIME, -1);
        auto tmb = new TimeBomb(*tower_go, TOWERFRAMESTOCREATE*TOWERFRAMETIME);
        spr->SetSize(TOWERSIZE.x*4, TOWERSIZE.y);
        tower_go->AddComponents({spr, tmb});
        tower_go->box.SetCenter(associated.box.x - tower_go->box.h/2, associated.box.y+associated.box.h-tower_go->box.w/2+35.0f);
    st.bulletArray.emplace_back(tower_go);

    associated.AddComponent(
        new TimedTrigger(associated, TOWERFRAMESTOCREATE*TOWERFRAMETIME,
            [](GameObject& associated)
            {
                LionBoss* lbass = (LionBoss*)associated.GetComponent(C_ID::Lion);
                auto& st = Game::GetInstance().GetCurrentState();
                GameObject* tower_go = new GameObject();
                    tower_go->angleDeg = 270.0f;
                    Sprite* spr = new Sprite(*tower_go, "assets/img/Lion/laser_loop_inverted.png", 1, 0, -1);
                    spr->SetSize(lbass->TOWERSIZE.x, lbass->TOWERSIZE.y);
                    spr->SetFliped(true);
                    tower_go->box.SetCenter(associated.box.x - tower_go->box.h/2, associated.box.y+associated.box.h-tower_go->box.w/2);
    
                    Collider* tower_co = new Collider(*tower_go);
                    tower_co->type = C_ID::Hitbox;
                    Attack* atk = new Attack(*tower_go, lbass->TOWERDAMAGE, 0, &associated);
                    auto dpsow = new DisappearOnDeadOwner(*tower_go, st.GetObject(C_ID::Lion, &st.enemyArray));
                    tower_go->AddComponents({spr, tower_co, atk, new TimeBomb(*tower_go, lbass->TOWERDURATION-(lbass->TOWERFRAMESTOCREATE*lbass->TOWERFRAMETIME)-0.5f), dpsow});
                st.bulletArray.emplace_back(tower_go);
            }
        )
    );
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
    if(!activated) return;
    currentHealth -= damage;
    
    // std::cout << "ai" << ' ' << currentHealth << '\n';
    if(currentHealth <= 0)
        DIEEE();
    else
        TakeDamageVisuals();
    
}

void LionBoss::TakeDamageVisuals() // TODO -> quando receber um ataque gerar os efeitos (tanto visuais quanto sonoros)
{
    // auto spr = associated.GetComponent(C_ID::StateMachine);
    // spr->AddComponent(new ShakeBehavior(*spr, Vec2(7,7), 1.0f, 0.2f));
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