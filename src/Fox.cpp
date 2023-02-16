#include "Fox.h"
#include "StateMac.h"
#include "IA.h"
#include "Game.h"
#include "Collider.h"
#include "Projectile.h"
#include "Sprite.h"
#include "Attack.h"
#include "ProjectileBehaviors.h"
#include "Rand.h"
#include "TimeBomb.h"
#include <cmath>

#define ANGLETOPI M_PI/180.0f

void Fox::SetVariables()
{
    auto& st = Game::GetInstance().GetCurrentState();
    FOXSIZE = Vec2(650, 500);

    ARCBALLSSIZE = Vec2(75,75);
    ARCBALLSQTT = 3;
    ARCBALLSINITIALYSPEED = 500.0f;
    ARCBALLSDURATION = 2.5f;
    ARCBALLSTIMEBETWEEN = 0.3f;
    ARCBALLSFIRSTTIME = 1.5f;
    ARCBALLSSPACEBETWEEN = 250.0f;

    KONSIZE;
    KONSETUPTIME;
    KONDURATION;

    BULLETHELLDURATION;

    WÕEXSPEED = 500.0f; // Wõe eh o que cria 2 projeteis q vão dos pontos do estágio e quando os projeteis se encontram eles invertem o angulo 180graus
    WÕEDURATION = 3.0f;
    WÕESIZE = Vec2(60, 200);

    LIONPHASINGTIME = 1.5f;
    LIONLASERDURATION;

    StageLBound = Rect(-56.655, 928, 160, 160);
    StageRBound = Rect(2531.59, 928, 160, 160);
    COMETSSPACEBETWEEN = 50.0f;
    COMETSIZE = Vec2(150, 100);
    COMETLANGLE = 60.0f;
    COMETRANGLE = 120.0f;
    COMETSPEED = 285.0f;
    COMETFIRSTTIME = 1.5f;
    COMETDELAY = 0.25f;
    COMETDURATION = 5.0f;

    TORNADODURATION = 2.0f;
    TORNADOLIVEDURATION = 10.0f;
    TORNADOSIZE = Vec2(700, 80);

    activated = false;
    moving = false;
    phase2 = false;
    currAttack = -1;

    player = st.GetObject(C_ID::Player, &st.rigidArray).lock().get();
}

Fox::Fox(GameObject& associated)
    : GameObject(associated)
{
    SetVariables();

    auto& st = Game::GetInstance().GetCurrentState();
    auto stmac = new StateMachine(associated);
    auto cld = new Collider(associated);
    
    auto node  = new AnimNode("assets/img/Fox/Kihiko Idle1.png", 1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(IDLE, node);

    node = new AnimNode("assets/img/Fox/Kihiko Bullethell K.png", 1, 0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(BULLETHELLANIM, node); stmac->AddTransition(BULLETHELLANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Dano K.png",1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(DAMAGED, node); stmac->AddTransition(DAMAGED, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Kon K.png", 1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(KONANIM, node); stmac->AddTransition(KONANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Snap K.png", 1, 1.5f);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(SNAPANIM, node); stmac->AddTransition(SNAPANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Stun K.png", 1,0);
    node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(STUNNED, node); stmac->AddTransition(STUNNED, IDLE);
    
    auto brainz = new IA(associated, st.GetObject(C_ID::Player, &st.rigidArray).lock().get(), 1.2f);
    
    brainz->SetActions(
        {
            {Vec2(), 1, ARCBALLSDURATION, true},
            {Vec2(), 1, KONDURATION, true},
            {Vec2(), 1, LIONLASERDURATION, true},
            {Vec2(), 1, BULLETHELLDURATION, true},
            {Vec2(), 1, WÕEDURATION+3.5f, true},
            {Vec2(), 1, COMETDURATION, true},
            {Vec2(), 1,  TORNADODURATION, true}
        }
    );
    
    associated.AddComponents({cld, stmac, brainz});
}


void Fox::Update(float dt)
{
    auto st = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    auto brainz = (IA*)associated.GetComponent(C_ID::IA);
    auto [idx, currNode] = st->GetCurrent();
    if(idx != STUNNED)
        currNode->SetFliped((player->box.GetCenter().x < this->box.GetCenter().x));
    switch(brainz->selectedAction)
    {
        case COMETS:
            st->ChangeState(SNAPANIM);
            COMET_f();
        break;
        case WÕE:
            st->ChangeState(SNAPANIM);
            WÕE_f();
        break;
        case TORNADO:
            TORNADO_f();
        break;
        case LIONLASER:

        break;
        case ARCBALLS:
            ARCBALL_f();
        break;
    }
}

void Fox::ARCBALL_f()
{
    auto& st = Game::GetInstance().GetCurrentState();
    float arctime = ARCBALLSFIRSTTIME;
    float arcybasespeed = ARCBALLSINITIALYSPEED;
    Vec2 vecinic(0,arcybasespeed);

    for(int i = 0; i < ARCBALLSQTT; i++, arctime += ARCBALLSTIMEBETWEEN)
    {
        auto arcballs_GO = new GameObject;
            auto arcballs_spr = new Sprite(*arcballs_GO, "assets/img/laser.png", 1, 0);
            arcballs_spr->SetSize(ARCBALLSSIZE.x, ARCBALLSSIZE.y);
            float xpos = Rand::FloatRange(player->box.x, player->box.x + ARCBALLSSPACEBETWEEN) + ARCBALLSSPACEBETWEEN*i;
            auto arcballs_proj = new Projectile(*arcballs_GO, arctime + 1.5f, 90.0f, vecinic.Magnitude(), vecinic.Magnitude());
            auto dsp = new DisappearOnHit(*arcballs_GO, &associated);
            arcballs_GO->AddComponents({arcballs_spr, arcballs_proj, dsp});
            arcballs_GO->box.x = xpos;
            arcballs_GO->box.y = StageLBound.y + StageLBound.h - arcybasespeed*arctime;
        st.bulletArray.emplace_back(arcballs_GO);

        arcballs_GO = new GameObject;
            arcballs_spr = new Sprite(*arcballs_GO, "assets/img/laser.png", 1, 0);
            arcballs_spr->SetSize(ARCBALLSSIZE.x, ARCBALLSSIZE.y);
            xpos = Rand::FloatRange(player->box.x - ARCBALLSSPACEBETWEEN, player->box.x) - ARCBALLSSPACEBETWEEN*i;
            arcballs_proj = new Projectile(*arcballs_GO, arctime + 1.5f, 90.0f, vecinic.Magnitude(), vecinic.Magnitude());
            dsp = new DisappearOnHit(*arcballs_GO, &associated);
            arcballs_GO->AddComponents({arcballs_spr, arcballs_proj, dsp});
            arcballs_GO->box.x = xpos;
            arcballs_GO->box.y = StageLBound.y + StageLBound.h - arcybasespeed*arctime;
        st.bulletArray.emplace_back(arcballs_GO);
        
    }
}

void Fox::KON_f()
{

}

void Fox::BULLETHELL_f()
{

}

void Fox::WÕE_f()
{
    auto& st = Game::GetInstance().GetCurrentState();
    float stageDist = (StageRBound.x-StageLBound.x)/WÕEXSPEED;
    auto wõe_GO = new GameObject;
    auto wõe_GO2 = new GameObject;
        auto wõe_spr = new Sprite(*wõe_GO, "assets/img/laser.png", 1, 0);
        wõe_spr->SetSize(WÕESIZE.x, WÕESIZE.y);
        auto wõe_proj = new Projectile(*wõe_GO, stageDist, 0.0f, WÕEXSPEED, WÕEXSPEED);
        // auto wõe_inver = new InvertOnConnection(*wõe_GO, wõe_GO2);
        wõe_GO->box.x = StageLBound.x + StageLBound.w - WÕESIZE.x;
        wõe_GO->box.y = StageLBound.y + StageLBound.h - WÕESIZE.y;
        wõe_GO->AddComponents({wõe_spr, wõe_proj/*, wõe_inver*/});
    st.bulletArray.emplace_back(wõe_GO);

        auto wõe_spr2 = new Sprite(*wõe_GO2, "assets/img/laser.png", 1, 0);
        wõe_spr2->SetFliped(true);
        wõe_spr2->SetSize(WÕESIZE.x, WÕESIZE.y);
        auto wõe_proj2 = new Projectile(*wõe_GO2, stageDist, 180.0f, WÕEXSPEED, WÕEXSPEED);
        //auto wõe_inver2 = new InvertOnConnection(*wõe_GO2, wõe_GO);
        wõe_GO2->box.x = StageRBound.x + StageRBound.w - WÕESIZE.x;
        wõe_GO2->box.y = StageRBound.y + StageRBound.h - WÕESIZE.y;
        wõe_GO2->AddComponents({wõe_spr2, wõe_proj2/*, wõe_inver2*/});
    st.bulletArray.emplace_back(wõe_GO2);

}

void Fox::LIONLASERS_f()
{

}

void Fox::COMET_f()
{
    auto plcent = player->box.GetCenter();
    auto& st = Game::GetInstance().GetCurrentState();
    bool corrdist = plcent.Distance(StageLBound.GetCenter()) < plcent.Distance(StageRBound.GetCenter());
    if(corrdist)
    {   
        float i = 0;
        for(float pos = StageLBound.x; pos < StageRBound.x-COMETSIZE.x-StageRBound.w; pos+=COMETSSPACEBETWEEN+COMETSIZE.x, i++)
        {
            auto comet_GO = new GameObject();
                comet_GO->depth = 51000;
                auto comet_spr = new Sprite(*comet_GO, "assets/img/laser.png", 1, 0);
                comet_spr->SetSize(COMETSIZE.x, COMETSIZE.y);
                auto comet_proj = new Projectile(*comet_GO, COMETFIRSTTIME + COMETDELAY*i + 2.0f, COMETLANGLE, COMETSPEED, COMETSPEED);
                auto dsp = new DisappearOnHit(*comet_GO, &associated);

                comet_GO->AddComponents({comet_spr, comet_proj, dsp});

                Vec2 xy = Vec2(COMETSPEED, 0).Rotate(COMETLANGLE*ANGLETOPI) * (COMETFIRSTTIME + COMETDELAY*i);
                comet_GO->box.x = -xy.x + pos; 
                
                comet_GO->box.y = StageLBound.y - StageLBound.h + COMETSIZE.y - xy.y;
                comet_GO->angleDeg = COMETLANGLE;

            st.bulletArray.emplace_back(comet_GO);
        }
    }

    else
    {
        float i = 0;
        for(float pos = StageRBound.x+StageRBound.w; pos > StageLBound.x+StageLBound.w+COMETSIZE.x; pos-=(COMETSSPACEBETWEEN+COMETSIZE.x), i++)
        {
        
            auto comet_GO = new GameObject();
                comet_GO->depth = 51000;
                auto comet_spr = new Sprite(*comet_GO, "assets/img/laser.png", 1, 0);
                comet_spr->SetSize(COMETSIZE.x, COMETSIZE.y);
                auto comet_proj = new Projectile(*comet_GO, COMETFIRSTTIME + COMETDELAY*i + 2.0f, COMETRANGLE, COMETSPEED, COMETSPEED);
                comet_proj->rotSprt = false;
                auto dsp = new DisappearOnHit(*comet_GO, &associated);

                comet_GO->AddComponents({comet_spr, comet_proj, dsp});

                Vec2 xy = Vec2(COMETSPEED, 0).Rotate(COMETRANGLE*ANGLETOPI) * (COMETFIRSTTIME + COMETDELAY*i);
                comet_GO->box.x = -xy.x + pos; 
                
                comet_GO->box.y = StageRBound.y - StageRBound.h + COMETSIZE.y - xy.y;
                comet_GO->angleDeg = COMETRANGLE;
            st.bulletArray.emplace_back(comet_GO);
        }
    }
}

void Fox::TORNADO_f()
{
    auto& st = Game::GetInstance().GetCurrentState();
    float xpos = Rand::FloatRange(StageLBound.x + 2*StageLBound.w, StageRBound.x - StageRBound.w - TORNADOSIZE.x);
    auto tornado_GO = new GameObject;
        auto spr = new Sprite(*tornado_GO, "assets/img/laser.png");
        auto cld = new Collider(*tornado_GO);
        auto tmb = new TimeBomb(*tornado_GO, TORNADOLIVEDURATION);
        spr->SetSize(TORNADOSIZE.x, TORNADOSIZE.y);
        tornado_GO->box.SetCenter(xpos, 75.0f -TORNADOSIZE.x/2 + StageRBound.y);
        tornado_GO->AddComponents({spr, cld, tmb});
        tornado_GO->angleDeg = 270.0f;
    st.bulletArray.emplace_back(tornado_GO);
}

void Fox::Phase2Transition()
{

}