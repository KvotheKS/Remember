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
#include "GameData.h"
#include "Trigger.h"
#include <string>
#include "ScreenFade.h"
#include "Kon.h"
#include <cmath>

#define ANGLETOPI M_PI/180.0f

void Fox::SetVariables()
{
    auto& st = Game::GetInstance().GetCurrentState();
    REALFOXFRAMES = 2;
    REALFOXFRAMETIME = 0.4f;

    PHASE2TRANSITION = 4.0f;

    FOXSIZE = Vec2(650, 500);
    IDLEFRAMES = 3;
    IDLEFRAMETIME = 0.3f;

    SNAPTRANSITIONFRAMES = 4;
    SNAPTRANSITIONFRAMETIME = 0.25f;
    SNAPMOVEMENTFRAMES = 3;
    SNAPMOVEMENTFRAMETIME = 0.2f;

    WARNINGCIRCLEFRAMES = 20;

    ARCBALLSSIZE = Vec2(90,90);
    ARCBALLSQTT = 5;
    ARCBALLSINITIALYSPEED = 500.0f;
    ARCBALLSDURATION = 2.0f;
    ARCBALLSTIMEBETWEEN = 0.3f;
    ARCBALLSFIRSTTIME = 1.5f;
    ARCBALLSSPACEBETWEEN = 250.0f;
    ARCBALLSDAMAGE = 5;
    ARCBALLSKNOCKBACK= 0;

    KONSIZE;
    KONSETUPTIME = 1.0f;
    KONDURATION = 8.0f;
    KONTRANSITIONFRAMES = 3;
    KONTRANSITIONFRAMETIME = 0.3f;
    KONMOVEMENTFRAMES = 3;
    KONMOVEMENTFRAMETIME = 0.2f;
    KONFRAMES = 2;
    KONFRAMETIME = 0.5f;
    KONLASERDURATION = 2.5f;
    KONFADEOUT = 1.5f;
    KONLASERSIZE = Vec2(4000,315);
    KONDAMAGE = 15;
    KONKNOCKBACK = 0;


    BULLETHELLDURATION = 15.0f;
    BULLETHELLMOVEMENTFRAMES = 6.0f;
    BULLETHELLMOVEMENTFRAMETIME = 0.3f;
    BULLETHELLTRANSITIONFRAMES = 3;
    BULLETHELLTRANSITIONFRAMETIME = 0.3f;
    

    WÕEXSPEED = 570.0f; // Wõe eh o que cria 2 projeteis q vão dos pontos do estágio e quando os projeteis se encontram eles invertem o angulo 180graus
    WÕEDURATION = 2.5f;
    WÕESIZE = Vec2(60, 200);
    WÕEDAMAGE = 3;
    WÕEKNOCKBACK = 0;

    LIONLASERDURATION = 5.0f;
    LIONPHASINGTIME = 1.5f;
    LIONLASERDAMAGEDURATION = 0.4f;
    LIONLASERSIZE = Vec2(0, 250);
    LIONLASERDAMAGE = 8;
    LIONLASERKNOCKBACK = 1;

    StageLBound = Rect(-56.655, 928, 160, 160);
    StageRBound = Rect(2531.59, 928, 160, 160);
    COMETSSPACEBETWEEN = 50.0f;
    COMETSIZE = Vec2(150, 100);
    COMETLANGLE = 60.0f;
    COMETRANGLE = 120.0f;
    COMETSPEED = 300.0f;
    COMETFIRSTTIME = 1.4f;
    COMETDELAY = 0.22f;
    COMETDURATION = 5.5f;
    COMETDAMAGE = 3;
    COMETKNOCKBACK = 0;

    TORNADODURATION = 2.0f;
    TORNADOLIVEDURATION = 10.0f;
    TORNADOSIZE = Vec2(700, 80);
    TORNADODAMAGE = 3;
    TORNADOKNOCKBACK = 1;

    PHASE1LIFE = 300;
    PHASE2LIFE = 500;
    
    currentLife = PHASE1LIFE;

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
    
    RealFox = new GameObject;
        RealFox->depth = -4.05f;
        auto real_spr = new Sprite(*RealFox, "assets/img/Fox/Kihiko Sit-Sheet.png", REALFOXFRAMES, REALFOXFRAMETIME);
        RealFox->AddComponent(real_spr);
        real_spr->SetScaleX(2,2);
        RealFox->box.x = 880; RealFox->box.y = 440;
    st.objectArray.emplace_back(RealFox);

    auto stmac = new StateMachine(associated);
    auto cld = new Collider(associated);
    
    auto node  = new AnimNode("assets/img/Fox/Kihiko Idle-Sheet.png", IDLEFRAMES, IDLEFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(IDLE, node); stmac->ChangeState(IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Bullethell Transition-Sheet.png", BULLETHELLTRANSITIONFRAMES, BULLETHELLTRANSITIONFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(BULLETHELLANIM, node); stmac->AddTransition(BULLETHELLANIM, BULLETHELLIDLEANIM);

    node = new AnimNode("assets/img/Fox/Kihiko Bullethell Movement-Sheet.png", BULLETHELLMOVEMENTFRAMES, BULLETHELLMOVEMENTFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(BULLETHELLIDLEANIM, node); stmac->AddTransition(BULLETHELLIDLEANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Dano K.png",1,0.2f);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(DAMAGED, node); stmac->AddTransition(DAMAGED, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Kon Transition-Sheet.png", KONTRANSITIONFRAMES, KONTRANSITIONFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(KONANIM, node); stmac->AddTransition(KONANIM, KONIDLEANIM);

    node = new AnimNode("assets/img/Fox/Kihiko Kon Movement-Sheet.png", KONMOVEMENTFRAMES, KONMOVEMENTFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(KONIDLEANIM, node); stmac->AddTransition(KONIDLEANIM, IDLE);

    node = new AnimNode("assets/img/Fox/Kihiko Snap Transition-Sheet.png", SNAPTRANSITIONFRAMES, SNAPTRANSITIONFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(SNAPANIM, node); stmac->AddTransition(SNAPANIM, SNAPIDLEANIM);

    node = new AnimNode("assets/img/Fox/Kihiko Snap Movement-Sheet.png", SNAPMOVEMENTFRAMES, SNAPMOVEMENTFRAMETIME);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(SNAPIDLEANIM, node); stmac->AddTransition(SNAPIDLEANIM, IDLE);


    node = new AnimNode("assets/img/Fox/Kihiko Stun K.png", 1,0);
    // node->SetSize(FOXSIZE.x, FOXSIZE.y);
    stmac->AddNode(STUNNED, node); stmac->AddTransition(STUNNED, IDLE);
    
    for(auto& stnds : stmac->GetStates())
    {
        stnds.second->SetScaleX(2,2);
        stnds.second->SetTint(70, 120,200);
    }

    auto brainz = new IA(associated, st.GetObject(C_ID::Player, &st.rigidArray).lock().get(), 1.2f);
    
    brainz->SetActions(
        {
            {Vec2(), 1, ARCBALLSDURATION},
            {Vec2(), 1, KONDURATION, true},
            {Vec2(), 1, LIONPHASINGTIME, true},
            {Vec2(), 1.0f, BULLETHELLDURATION,true},
            {Vec2(), 1, WÕEDURATION},
            {Vec2(), 1, COMETDURATION},
            {Vec2(), 1, TORNADODURATION}
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
            if(phase2)
                AddComponent(new TimedTrigger(*this, COMETFIRSTTIME + COMETDELAY*14,
                [](GameObject& associated){ ((Fox&)associated).COMET_f(); }));
        break;
        case WÕE:
            st->ChangeState(SNAPANIM);
            WÕE_f();
        break;
        case TORNADO:
            TORNADO_f();
        break;
        case LIONLASER:
            // FoxLaugh();
            // LIONLASERS_f();
        break;
        case ARCBALLS:
            ARCBALL_f();
        break;
        case KON:
            st->ChangeState(KONANIM);
            KON_f();
        break;
        case BULLETHELL:
            st->ChangeState(BULLETHELLANIM);
            BULLETHELL_f();
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
            auto arcballs_spr = new Sprite(*arcballs_GO, "assets/img/Fox/projectile_birth.png", 5, 0.1f);
            arcballs_spr->SetSize(ARCBALLSSIZE.x*5, ARCBALLSSIZE.y);
            arcballs_spr->SetTint(200,200,70);
            float xpos = Rand::FloatRange(player->box.x, player->box.x + ARCBALLSSPACEBETWEEN) + ARCBALLSSPACEBETWEEN*i;
            auto arcballs_proj = new Projectile(*arcballs_GO, arctime + 1.5f, 90.0f, vecinic.Magnitude(), vecinic.Magnitude());
            auto dsp = new DisappearOnHit(*arcballs_GO, &associated);
            auto atk = new Attack(*arcballs_GO, ARCBALLSDAMAGE, ARCBALLSKNOCKBACK, &associated);
            arcballs_GO->AddComponents({arcballs_spr, arcballs_proj, dsp, atk});
            arcballs_GO->box.x = xpos;
            arcballs_GO->box.y = StageLBound.y + StageLBound.h - arcybasespeed*arctime;
        st.bulletArray.emplace_back(arcballs_GO);

        arcballs_GO = new GameObject;
            arcballs_spr = new Sprite(*arcballs_GO, "assets/img/Fox/projectile_birth.png", 5, 0.2f);
            arcballs_spr->SetSize(ARCBALLSSIZE.x*5, ARCBALLSSIZE.y);
            arcballs_spr->SetTint(200,200,70);
            xpos = Rand::FloatRange(player->box.x - ARCBALLSSPACEBETWEEN, player->box.x) - ARCBALLSSPACEBETWEEN*i;
            arcballs_proj = new Projectile(*arcballs_GO, arctime + 1.5f, 90.0f, vecinic.Magnitude(), vecinic.Magnitude());
            atk = new Attack(*arcballs_GO, ARCBALLSDAMAGE, ARCBALLSKNOCKBACK, &associated);
            dsp = new DisappearOnHit(*arcballs_GO, &associated);
            arcballs_GO->AddComponents({arcballs_spr, arcballs_proj, dsp, atk});
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
    associated.AddComponent(
        new TimedTrigger(associated, 0.4f + KONTRANSITIONFRAMES*KONTRANSITIONFRAMETIME,
            [](GameObject& associated)
            {
                auto& st = Game::GetInstance().GetCurrentState();
                auto fx = (Fox*)associated.GetComponent(C_ID::Fox);
               
                GameObject* Go_hurtsound = new GameObject();
                std::string soundname;
                std::cout << "KONJ";
                soundname = "assets/audio/Foleys/KON.wav";        
                Sound* ypehurtsound = new Sound(*Go_hurtsound, soundname);
                ypehurtsound->Play();
                Go_hurtsound->AddComponent(ypehurtsound);
                std::cout << "WSEekon";
                auto kon_go = new GameObject;
                    kon_go->AddComponents({new Kon(*kon_go, fx->KONFRAMES, fx->KONFRAMETIME)});
                st.cameraFollowerObjectArray.emplace_back(kon_go);
            }
        )
    );
    auto& st = Game::GetInstance().GetCurrentState();

    float scrdur = 0.4f + KONTRANSITIONFRAMES*KONTRANSITIONFRAMETIME + KONFRAMETIME + KONLASERDURATION;
    auto screen_go = new GameObject;
        screen_go->depth = 0;
        auto screen_fade = new ScreenFade(*screen_go, scrdur, scrdur, scrdur);
        auto tmb         = new TimeBomb(*screen_go, scrdur);
        screen_fade->SetColor(0,0,0,255);
        screen_go->AddComponents({tmb,screen_fade});

    st.objectArray.emplace_back(screen_go);
}

void Fox::BULLETHELL_f()
{
    GameObject* Go_hurtsound = new GameObject();
        std::string soundname;
        std::cout << "WEE_b";
        soundname = "assets/audio/Foleys/Risada da raposa.wav";
        Sound* ypehurtsound = new Sound(*Go_hurtsound, soundname);
        ypehurtsound->Play();
        Go_hurtsound->AddComponent(ypehurtsound);
        std::cout << "WOO_B";

    associated.AddComponent(
        new TimedTrigger(associated, BULLETHELLTRANSITIONFRAMES*BULLETHELLTRANSITIONFRAMETIME,
            [](GameObject& associated)
            {
                auto& st = Game::GetInstance().GetCurrentState();
                auto  stm= (StateMachine*)associated.GetComponent(C_ID::StateMachine);
                auto  fox= (Fox*)associated.GetComponent(C_ID::Fox);
                auto  brainz = (IA*)associated.GetComponent(C_ID::IA);
                fox->LIONLASERS_f();
                brainz->actions[LIONLASER].deactivated = true;
                brainz->actions[BULLETHELL].deactivated = true;

                brainz->SetActionTimer((float)0.1f);
                brainz->Update(0.2f);

                fox->Update(0.0f);
                brainz->actions[LIONLASER].deactivated = false;
                brainz->actions[BULLETHELL].deactivated = false;
                stm->ChangeState(BULLETHELLIDLEANIM);
            }
        )
    );
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
                auto atk      = new Attack(*wõe_GO, fx->WÕEDAMAGE, fx->WÕEKNOCKBACK, &associated);
                wõe_GO->box.x = fx->StageLBound.x + fx->StageLBound.w - fx->WÕESIZE.x;
                wõe_GO->box.y = fx->StageLBound.y + fx->StageLBound.h - fx->WÕESIZE.y;
                wõe_GO->AddComponents({wõe_spr, wõe_proj, atk});
            st.bulletArray.emplace_back(wõe_GO);

                auto wõe_spr2 = new Sprite(*wõe_GO2, "assets/img/Lion/wave.png", 1, 0);
                wõe_spr2->SetSize(fx->WÕESIZE.x, fx->WÕESIZE.y);
                auto wõe_proj2 = new Projectile(*wõe_GO2, stageDist, 180.0f, fx->WÕEXSPEED,fx->WÕEXSPEED);
                auto atk2      = new Attack(*wõe_GO2, fx->WÕEDAMAGE, fx->WÕEKNOCKBACK, &associated);
                wõe_GO2->box.x = fx->StageRBound.x + fx->StageRBound.w - fx->WÕESIZE.x;
                wõe_GO2->box.y = fx->StageRBound.y + fx->StageRBound.h - fx->WÕESIZE.y;
                wõe_GO2->AddComponents({wõe_spr2, wõe_proj2, atk2});
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
        laser_GO->angleDeg = 180;
        auto laser_spr = new Sprite(*laser_GO, "assets/img/Lion/malu_laser_loop.png", 4, 0.2f);
        laser_spr->SetSize((foxptr->StageRBound.x+foxptr->StageRBound.w-foxptr->StageLBound.x)*4, foxptr->LIONLASERSIZE.y);
        laser_spr->SetTint(70,127,210);
        Vec2 scl(((foxptr->StageRBound.x+foxptr->StageRBound.w)-foxptr->StageLBound.x)/laser_GO->box.w, 1);
        auto laser_col = new Collider(*laser_GO, scl, Vec2(-associated.box.w,0));
        auto tmb = new TimeBomb(*laser_GO, foxptr->LIONLASERDAMAGEDURATION);
        auto atk = new Attack(*laser_GO, foxptr->LIONLASERDAMAGE, foxptr->LIONLASERKNOCKBACK, &foxptr->associated);
        laser_GO->AddComponents({laser_spr, laser_col, tmb, atk});
        laser_GO->box.x = associated.box.x + associated.box.w;  
        laser_GO->box.y = foxptr->StageLBound.y + foxptr->StageLBound.h - laser_GO->box.h;
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
                auto comet_spr = new Sprite(*comet_GO, "assets/img/Fox/projectile_birth.png", 5, 0.2f);
                comet_spr->SetSize(COMETSIZE.x*5, COMETSIZE.y);
                comet_spr->SetTint(200,200, 75);
                auto comet_proj = new Projectile(*comet_GO, COMETFIRSTTIME + COMETDELAY*i + 2.0f, COMETLANGLE, COMETSPEED, COMETSPEED);
                auto dsp = new DisappearOnHit(*comet_GO, &associated);
                auto atk = new Attack(*comet_GO, COMETDAMAGE, COMETKNOCKBACK,&associated);
                comet_GO->AddComponents({comet_spr, comet_proj, dsp, atk});

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
                auto comet_spr = new Sprite(*comet_GO, "assets/img/Fox/projectile_birth.png", 5, 0.2f);
                comet_spr->SetSize(COMETSIZE.x*5, COMETSIZE.y);
                comet_spr->SetTint(200,200, 75);
                auto comet_proj = new Projectile(*comet_GO, COMETFIRSTTIME + COMETDELAY*i + 2.0f, COMETRANGLE, COMETSPEED, COMETSPEED);
                comet_proj->rotSprt = false;
                auto dsp = new DisappearOnHit(*comet_GO, &associated);
                auto atk = new Attack(*comet_GO, COMETDAMAGE, COMETKNOCKBACK,&associated);
                comet_GO->AddComponents({comet_spr, comet_proj, dsp, atk});

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
        auto atk = new Attack(*tornado_GO, TORNADODAMAGE, TORNADOKNOCKBACK, &associated);
        spr->SetSize(TORNADOSIZE.x, TORNADOSIZE.y);
        tornado_GO->box.SetCenter(xpos, 75.0f -TORNADOSIZE.x/2 + StageRBound.y);
        tornado_GO->AddComponents({spr, cld, tmb, atk});
        tornado_GO->angleDeg = 270.0f;
    st.bulletArray.emplace_back(tornado_GO);
}

void ToPhase2Time(GameObject& associated)
{
    auto fx     = (Fox*)associated.GetComponent(C_ID::Fox);
    fx->Phase2Transition();
}

void Fox::Phase2Transition()
{
    auto& st = Game::GetInstance().GetCurrentState();
    associated.RemoveComponent(associated.GetComponent(C_ID::Collider));
    auto screenfade_go = new GameObject;
        screenfade_go->depth = 100000.0f;
        auto screen_fade = new ScreenFade(*screenfade_go, PHASE2TRANSITION/2, PHASE2TRANSITION/2, PHASE2TRANSITION);
        screen_fade->SetColor(255,255,255,255);
        screenfade_go->AddComponent(screen_fade);
    st.objectArray.emplace_back(screenfade_go);
    auto brainz = (IA*)associated.GetComponent(C_ID::IA);
    brainz->SetActionTimer((float)0.0f);
    auto stm     = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    for(auto& states : stm->GetStates())
        SDL_SetTextureAlphaMod(states.second->texture.get(), 0);        

    associated.AddComponent(
        new TimedTrigger(
            associated,
            PHASE2TRANSITION,
        [](GameObject& associated)
        {
            auto brainz = (IA*)associated.GetComponent(C_ID::IA);
            auto st     = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
            auto fx     = (Fox*)associated.GetComponent(C_ID::Fox);
            brainz->SetActionTimer((float)0.01f);
            fx->currentLife = fx->PHASE2LIFE;

            for(auto& it : brainz->actions)
                it.deactivated = false;
            brainz->actions[COMETS].cooldown *= 2;
            for(auto& states : st->GetStates())
            {
                states.second->SetTint(255,255,255);
                SDL_SetTextureAlphaMod(states.second->texture.get(), 255);      
            }
            fx->RealFox->RequestDelete();
            st->AddTransition(fx->BULLETHELLANIM,fx->BULLETHELLIDLEANIM);
            associated.AddComponent(new Collider(associated));
            
            GameObject* Go_hurtsound = new GameObject();
            std::string soundname;
            std::cout << "in RIS";
            soundname = "assets/audio/Foleys/Risada da raposa.wav";        
            Sound* ypehurtsound = new Sound(*Go_hurtsound, soundname);
            ypehurtsound->Play();
            Go_hurtsound->AddComponent(ypehurtsound);
            
            std::cout << "out RIS";
        }
    ));
}

void Fox::NotifyCollision(GameObject& other, Vec2 sep)
{
    auto bingus = (Attack*)other.GetComponent(C_ID::Attack);
    if(!bingus || bingus->OwnedBy(&associated))
        return;
    currentLife -= bingus->GetDamage();
    if(currentLife <= 0)
    {
        if(phase2)
        {
            DIEEEE();
            return;
        }
        else
        {
            Phase2Transition();
            phase2 = true;
            return;
        }
    }
    auto stm = (StateMachine*)associated.GetComponent(C_ID::StateMachine); 
    auto [idx, currnode] = stm->GetCurrent();

    if(idx == BULLETHELLIDLEANIM || idx == IDLE || idx == SNAPIDLEANIM)
    {
        GameObject* Go_hurtsound = new GameObject();
        float rando = Rand::Get_r();
        std::string soundname;
        if (rando > 0.5 )
            soundname = "assets/audio/Foleys/Raposa machucada 1.wav";
        
        else
            soundname = "assets/audio/Foleys/Raposa Machucada 2.wav";
        
        
        Sound* ypehurtsound = new Sound(*Go_hurtsound, soundname);
        ypehurtsound->Play();
        Go_hurtsound->AddComponent(ypehurtsound);

        State& state = Game::GetInstance().GetCurrentState();
        state.AddObject(Go_hurtsound);
        stm->ChangeState(DAMAGED);
    }
}

void Fox::DIEEEE()
{
    GameData::isAlive[2] = false;
    auto& st = Game::GetInstance().GetCurrentState();
    
    auto stm = (StateMachine*)associated.GetComponent(C_ID::StateMachine); 
    stm->ChangeState(DAMAGED);

    auto [idx, currnode] = stm->GetCurrent();
    currnode->SetFrameTime(10000.0f);

    auto screen_go = new GameObject;
        auto screen_fade = new ScreenFade(*screen_go, 1.0f,1.0f, 1.0f);
        screen_fade->SetColor(255,255,255,255);
        screen_go->AddComponent(screen_fade);
    st.objectArray.emplace_back(screen_go);

    associated.AddComponent(new TimeBomb(associated, 0.5f));
    
    auto spr_apr = new GameObject;
    spr_apr->depth = 10;
    spr_apr->box.w = 250;
    spr_apr->box.h = 150;
    spr_apr->box.x = StageLBound.x + (StageRBound.x - StageLBound.x)/2;
    spr_apr->box.y = StageRBound.y + StageRBound.h - spr_apr->box.h;
    spr_apr->AddComponent(new TimedTrigger(*spr_apr, 0.5f, 
        [](GameObject& associated)
        {
            Rect orig = associated.box;
            auto spr = new Sprite(associated, "assets/img/Fox/Kihiko Death.png", 1,0,-1);
            spr->SetSize(orig.w,orig.h);
            associated.box.x = orig.x;
            associated.box.y = orig.y; 
            associated.AddComponent(spr);
        }
    ));

    st.objectArray.emplace_back(spr_apr);
}

bool Fox::Is(C_ID type)
{ return C_ID::Fox == type; }
