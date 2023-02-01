#include "LionBoss.h"
#include "IA.h"
#include "StateMac.h"
#include "Game.h"
#include "Attack.h"
#include "TimeBomb.h"
#include "Rand.h"
#include "Projectile.h"
#include <memory>
#include <cmath>

#define PI_DEG 180.0f / M_PI

LionBoss::LionBoss(GameObject& associated)
    : GameObject(associated)
{
    CLOSEX = 50.0f;
    MIDDLEX = 150.0f;
    FARX = 300.0f;

    LASERDURATION = 3.0f;
    TOWERDURATION = 3.0f;
    BALLSDURATION = 2.5f;
    SHOCKWAVEDURATION = 4.0f;
    
    LASERDAMAGE = 25;
    LASERKNOCK = Vec2();
    LASERSIZE = Vec2(350,200);

    BALLSDAMAGE = 15;
    BALLSKNOCK = Vec2();
    BALLSSPEED = -300.0f;
    BALLSYSPEED = -800.0f;
    BALLSIZE = Vec2(50,50);

    TOWERDAMAGE = 15;
    TOWERKNOCK = Vec2(-10, -15);

    WAVESIZE = Vec2(50,250);
    SHOCKWAVEDAMAGE = 25;
    SHOCKWAVEKNOCK = Vec2(10, 40);
    SHOCKWAVESPEED = 250;

    auto lionbrainz = new IA(associated, nullptr, 1.5f);
    auto stmac = new StateMachine(associated);
    associated.AddComponents({lionbrainz, stmac});

    auto anm = new AnimNode("assets/img/LionBoss.jpeg", 1,1,Vec2(1,1), false, false);
    anm->SetSize(100, 150);
    stmac->AddNode(IDLE, anm);
    stmac->ChangeState_s(IDLE);
    
    auto& currstate = Game::GetInstance().GetCurrentState();
    
    auto target = currstate.GetObject(C_ID::Player);
    if(target.expired())
    {
        std::cout << "PLAYER DEVE SER DECLARADO ATES DO LIONBOSS\n";
        exit(0);
    }

    lionbrainz->SetTarget(target.lock().get());
    lionbrainz->SetActions({
            {Vec2(MIDDLEX, 0), 1, LASERDURATION},
            {Vec2(MIDDLEX, 0), 1, BALLSDURATION},
            {Vec2(CLOSEX, 0), 1, TOWERDURATION},
            {Vec2(MIDDLEX, 0), 1, SHOCKWAVEDURATION}
        }
    );
}

void LionBoss::Update(float dt)
{
    auto lionbrainz = (IA*)associated.GetComponent(C_ID::IA);
    auto stmac = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    int choice = lionbrainz->selectedAction;
    
    switch(choice)
    {
        case LASER: Laser(); break;
        case BALLS: FlameBalls(); break;
        case TOWER: FlameTower(); break;
        case SHOCKWAVE: ShockWave(); break;
        default:
        break;
    }
}

void LionBoss::ShockWave()
{
    auto& st = Game::GetInstance().GetCurrentState();
    
    GameObject* wave_go = new GameObject;
        Sprite* spr = new Sprite(*wave_go, "assets/img/laser.png", 1, 0, -1);
        spr->SetSize(WAVESIZE.x, WAVESIZE.y);
        wave_go->box.x = associated.box.x - spr->GetWidth();
        wave_go->box.y = associated.box.y; //+ associated.box.h - spr->GetHeight();
        Projectile* wave_proj = new Projectile(*wave_go, 7.0f, -180.0f, SHOCKWAVESPEED);
        Attack* atk = new Attack(*wave_go, SHOCKWAVEDAMAGE, SHOCKWAVEKNOCK);
        wave_go->AddComponents({spr, wave_proj, atk});
    st.bulletArray.emplace_back(wave_go);
}

void LionBoss::Laser()
{
    auto& st = Game::GetInstance().GetCurrentState();

    GameObject* laser_go = new GameObject();
        Sprite* spr = new Sprite(*laser_go, "assets/img/laser.png", 1, 0, -1);
        spr->SetSize(LASERSIZE.x, LASERSIZE.y);
        laser_go->box.x = associated.box.x - spr->GetWidth();
        laser_go->box.y = associated.box.y;
        Collider* cld = new Collider(*laser_go);
        TimeBomb* tmb = new TimeBomb(*laser_go,LASERDURATION);
        Attack* laser_atk = new Attack(*laser_go, LASERDAMAGE, LASERKNOCK);
        laser_go->AddComponents({spr, cld, tmb, laser_atk});
    st.bulletArray.emplace_back(laser_go);
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
            DisappearOnHit* dsp = new DisappearOnHit(*balls_go);
            Projectile* ballsproj = new Projectile(*balls_go, 5.0f, vecinic.AngleX() * PI_DEG, vecinic.Magnitude(), vecinic.Magnitude(), grav);
            spr->SetSize(BALLSIZE.x, BALLSIZE.y);
            balls_go->angleDeg = vecinic.AngleX();
            balls_go->box.x = associated.box.x + associated.box.w; 
            balls_go->box.y = associated.box.y + associated.box.h - BALLSIZE.y;
            Attack* ball_atk = new Attack(*balls_go, BALLSDAMAGE, BALLSKNOCK);
            balls_go->AddComponents({dsp, spr, ballsproj, ball_atk});
        st.bulletArray.emplace_back(balls_go);
    }

}

void LionBoss::FlameTower()
{
    auto& st = Game::GetInstance().GetCurrentState();
    // std::cout << "wtfman\n";
    GameObject* tower_go = new GameObject();
        Sprite* spr = new Sprite(*tower_go, "assets/img/laser.png", 1,0, -1);
        spr->SetSize(150, 100 + associated.box.h);
        spr->SetFliped(true);
        tower_go->box.x = associated.box.x - spr->GetWidth(); 
        tower_go->box.y = associated.box.y - 100;
        Collider* tower_co = new Collider(*tower_go);
        Attack* atk = new Attack(*tower_go, TOWERDAMAGE, TOWERKNOCK);
        tower_go->AddComponents({spr, tower_co, atk, new TimeBomb(*tower_go, TOWERDURATION)});
    st.bulletArray.emplace_back(tower_go);
}