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
#include "LionBoss.h"
#include "Trigger.h"
#include <cmath>

#define ANGLETOPI M_PI/180.0f

void Fox::SetVariables()
{
    auto& st = Game::GetInstance().GetCurrentState();
    FOXSIZE = Vec2(650, 500);
    IDLEFRAMES = 3;
    IDLEFRAMETIME = 0.3f;

    SNAPTRANSITIONFRAMES = 4;
    SNAPTRANSITIONFRAMETIME = 0.25f;
    SNAPMOVEMENTFRAMES = 3;
    SNAPMOVEMENTFRAMETIME = 0.2f;

    WARNINGCIRCLEFRAMES = 20;

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

    LIONLASERDURATION = 5.0f;
    LIONPHASINGTIME = 1.5f;
    LIONLASERDAMAGEDURATION = 0.4f;
    LIONLASERSIZE = Vec2(0, 250);

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
    
    auto node  = new AnimNode("assets/img/Fox/Kihiko Idle-Sheet.png", IDLEFRAMES, IDLEFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(IDLE, node); stmac->ChangeState(IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Bullethell K.png", 1, 0);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(BULLETHELLANIM, node); stmac->AddTransition(BULLETHELLANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Dano K.png",1,0);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(DAMAGED, node); stmac->AddTransition(DAMAGED, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Kon K.png", 1,0);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(KONANIM, node); stmac->AddTransition(KONANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Snap Transition-Sheet.png", SNAPTRANSITIONFRAMES, SNAPTRANSITIONFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(SNAPANIM, node); stmac->AddTransition(SNAPANIM, SNAPIDLEANIM);

    node = new AnimNode("assets/img/Fox/Kihiko Snap Movement-Sheet.png", SNAPMOVEMENTFRAMES, SNAPMOVEMENTFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(SNAPIDLEANIM, node); stmac->AddTransition(SNAPIDLEANIM, IDLE);


    node = new AnimNode("assets/img/Fox/Kihiko Stun K.png", 1,0);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(STUNNED, node); stmac->AddTransition(STUNNED, IDLE);
    
    auto brainz = new IA(associated, st.GetObject(C_ID::Player, &st.rigidArray).lock().get(), 1.2f);
    
    brainz->SetActions(
        {
            {Vec2(), 1, ARCBALLSDURATION, true},
            {Vec2(), 1, KONDURATION, true},
            {Vec2(), 1, LIONPHASINGTIME, true},
            {Vec2(), 1, BULLETHELLDURATION, true},
            {Vec2(), 1, WÕEDURATION},
            {Vec2(), 1, COMETDURATION, true},
            {Vec2(), 1, TORNADODURATION, true}
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
            // st->ChangeState(SNAPANIM);
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
            LIONLASERS_f();
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

bool FinishedSnap(GameObject& associated)
{
    auto assc = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    return (assc->IsDone());
}

void Fox::KON_f()
{

}

void Fox::BULLETHELL_f()
{

}

void Fox::WÕE_f()
{
    associated.AddComponent(
        new ConditionalTrigger(associated, FinishedSnap, [](GameObject& associated)
        {
            auto fx = (Fox*)associated.GetComponent(C_ID::Fox);
            auto& st = Game::GetInstance().GetCurrentState();
            float stageDist = (fx->StageRBound.x-fx->StageLBound.x)/fx->WÕEXSPEED;
            auto wõe_GO = new GameObject;
            auto wõe_GO2 = new GameObject;
                auto wõe_spr = new Sprite(*wõe_GO, "assets/img/Lion/wave.png", 1, 0);
                wõe_spr->SetSize(fx->WÕESIZE.x, fx->WÕESIZE.y);
                wõe_spr->SetFliped(true);
                auto wõe_proj = new Projectile(*wõe_GO, stageDist, 0.0f, fx->WÕEXSPEED, fx->WÕEXSPEED);
                wõe_GO->box.x = fx->StageLBound.x + fx->StageLBound.w - fx->WÕESIZE.x;
                wõe_GO->box.y = fx->StageLBound.y + fx->StageLBound.h - fx->WÕESIZE.y;
                wõe_GO->AddComponents({wõe_spr, wõe_proj});
            st.bulletArray.emplace_back(wõe_GO);

                auto wõe_spr2 = new Sprite(*wõe_GO2, "assets/img/Lion/wave.png", 1, 0);
                wõe_spr2->SetSize(fx->WÕESIZE.x, fx->WÕESIZE.y);
                auto wõe_proj2 = new Projectile(*wõe_GO2, stageDist, 180.0f, fx->WÕEXSPEED,fx->WÕEXSPEED);
                
                wõe_GO2->box.x = fx->StageRBound.x + fx->StageRBound.w - fx->WÕESIZE.x;
                wõe_GO2->box.y = fx->StageRBound.y + fx->StageRBound.h - fx->WÕESIZE.y;
                wõe_GO2->AddComponents({wõe_spr2, wõe_proj2});
            st.bulletArray.emplace_back(wõe_GO2);
        }
    ));
}

void Fox::LIONLASERS_f()
{
    auto& st = Game::GetInstance().GetCurrentState();
    auto brainz = (IA*)associated.GetComponent(C_ID::IA);
    brainz->actions[LIONLASER].deactivated = true;
    auto lion_go = new GameObject;
        lion_go->depth = 15;

        auto lion_st = new StateMachine(*lion_go);
        auto node = new AnimNode("assets/img/Lion/closed_mouth.png", 1, LIONPHASINGTIME, Vec2(1,1), false, false);
        node->SetScaleX(1.2,1.2); node->SetTint(50,127,255); node->SetFliped(true);
        lion_st->AddNode(LionBoss::IDLE, node); lion_st->AddTransition(LionBoss::IDLE, LionBoss::LASERING);
        lion_st->ChangeState(LionBoss::IDLE);

        node = new AnimNode("assets/img/Lion/open_mouth.png", 1, 1, Vec2(1,1), false, false);
        node->SetScaleX(1.2,1.2); node->SetTint(50,127,255); node->SetFliped(true);
        lion_st->AddNode(LionBoss::LASERING, node);
        auto tmb = new TimeBomb(*lion_go, LIONLASERDURATION);
        auto lion_prog = new ProgressTrigger(*lion_go, LIONPHASINGTIME, &PhasingLion);
        auto lion_timed = new TimedTrigger(*lion_go, LIONPHASINGTIME, &SpawnLionLaser);
        
        lion_go->AddComponents({lion_st, tmb, lion_prog, lion_timed});
        lion_go->box.x = StageLBound.x;
        lion_go->box.y = StageLBound.y + StageLBound.h - lion_go->box.h;
    st.objectArray.emplace_back(lion_go);
}

void PhasingLion(GameObject& associated, float pgr)
{
    auto ln_st = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    auto [idx, currNode] = ln_st->GetCurrent();
    SDL_SetTextureAlphaMod(currNode->texture.get(), (Uint8)(255.0f*pgr));
}

void UnphasingLion(GameObject& associated, float pgr)
{
    if(pgr == 1.0f)
    {
        auto& st = Game::GetInstance().GetCurrentState();
        auto fox = st.GetObject(C_ID::Fox, &st.enemyArray);
        if(fox.expired())
        {
            associated.RequestDelete();
            return;
        }
        auto foxptr = (Fox*)fox.lock().get()->GetComponent(C_ID::Fox);
        auto brainz = (IA*)foxptr->associated.GetComponent(C_ID::IA);
        brainz->actions[foxptr->LIONLASER].deactivated = false;
    }

    auto ln_st = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    auto [idx, currNode] = ln_st->GetCurrent();
    SDL_SetTextureAlphaMod(currNode->texture.get(), (Uint8)(255.0f*(1.0f-pgr)));
}

void UnphasingLionTrg(GameObject& associated)
{ 
    auto& st = Game::GetInstance().GetCurrentState();
    auto fox = st.GetObject(C_ID::Fox, &st.enemyArray);
    if(fox.expired())
    {
        associated.RequestDelete();
        return;
    }
    auto foxptr = (Fox*)fox.lock().get()->GetComponent(C_ID::Fox);
    associated.AddComponent(new ProgressTrigger(associated, foxptr->LIONPHASINGTIME, UnphasingLion)); 
}

void SpawnLionLaser(GameObject& associated)
{
    auto& st = Game::GetInstance().GetCurrentState();
    
    auto fox = st.GetObject(C_ID::Fox, &st.enemyArray);

    if(fox.expired()) return;

    auto foxptr = (Fox*)fox.lock().get()->GetComponent(C_ID::Fox);

    auto laser_GO = new GameObject;
        laser_GO->depth = 50;
        auto laser_spr = new Sprite(*laser_GO, "assets/img/laser.png", 1, 0);
        laser_spr->SetSize(foxptr->StageRBound.x+foxptr->StageRBound.w-foxptr->StageLBound.x, foxptr->LIONLASERSIZE.y);
        Vec2 scl(((foxptr->StageRBound.x+foxptr->StageRBound.w)-foxptr->StageLBound.x)/laser_GO->box.w, 1);
        auto laser_col = new Collider(*laser_GO, scl, Vec2(-associated.box.w,0));
        auto tmb = new TimeBomb(*laser_GO, foxptr->LIONLASERDAMAGEDURATION);
        laser_GO->AddComponents({laser_spr, laser_col, tmb});
        laser_GO->box.x = associated.box.x + associated.box.w;  
        laser_GO->box.y = foxptr->StageLBound.y + foxptr->StageLBound.h - laser_GO->box.h;
        std::cout << foxptr->StageLBound;
        std::cout << laser_GO->box;
    st.bulletArray.emplace_back(laser_GO);

    auto tmtrg = new TimedTrigger(associated, foxptr->LIONLASERDAMAGEDURATION, UnphasingLionTrg);
    associated.AddComponent(tmtrg);
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

bool Fox::Is(C_ID type)
{ return C_ID::Fox == type; }