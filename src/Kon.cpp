#include "Kon.h"
#include "StateMac.h"
#include "Game.h"
#include "Fox.h"
#include "Attack.h"
#include "Trigger.h"
#include "Player.h"
#include "Camera.h"
#include "Sprite.h"
#include "StageFox.h"
#include "TimeBomb.h"

Kon::Kon(GameObject& associated, float FRAMEQUANTITY, float FRAMETIME)
    : GameObject(associated)
{
    auto& st = Game::GetInstance().GetCurrentState();
    auto pl = st.GetObject(C_ID::Player, &st.rigidArray);
    
    if(pl.expired())
    {
        associated.RequestDelete();
        return;
    }
    ((Player*)pl.lock()->GetComponent(C_ID::Player))->Bounds = Rect(Camera::pos.x,Camera::pos.y, Camera::width - 200, Camera::height);
    auto stm = new StateMachine(associated);
    auto node = new AnimNode("assets/img/Fox/Fox-Sheet.png", FRAMEQUANTITY, FRAMETIME, Vec2(1,1),true, false);
    stm->AddNode(0, node); stm->ChangeState(0); node->oneWay = true;
    associated.box.x = Camera::pos.x + Camera::width - associated.box.w;
    associated.box.y = Camera::pos.y + Camera::height/2 - associated.box.h/2 - 25.0f;
    associated.AddComponents({stm, new TimedTrigger(associated, FRAMETIME, KonShootLaser)});
    Camera::SetCameraFunction(Camera::Stationary);
}

void Kon::ResetWorld()
{
    auto& st = (StageFox&)Game::GetInstance().GetCurrentState();
    auto pl = st.GetObject(C_ID::Player, &st.rigidArray);
    
    if(pl.expired())
    {   
        return;
    }   
    Camera::Follow(pl.lock().get());
    ((Player*)pl.lock()->GetComponent(C_ID::Player))->Bounds = Rect(-70,-500, 60*st.tlmap->GetWidth()+140,60*st.tlmap->GetHeight()+500);
    Camera::Bounds = Rect(0,0, 60*st.tlmap->GetWidth(), 60*st.tlmap->GetHeight());
    Camera::SetCameraFunction(Camera::FollowTarget);
}

bool OpenMouth(GameObject& associated)
{
    auto assc = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    return (assc->IsDone());
}

void KonShootLaser(GameObject& associated)
{
    auto& st = Game::GetInstance().GetCurrentState();
    auto fx = st.GetObject(C_ID::Fox, &st.enemyArray);
    auto stm = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    if(fx.expired())
    {
        associated.RequestDelete();
        return;
    }
    auto fx_go = fx.lock().get();
    auto fox = (Fox*)fx_go->GetComponent(C_ID::Fox);

    auto laser_go = new GameObject;
        laser_go->angleDeg = 180.0f;
        auto laser_spr = new Sprite(*laser_go, "assets/img/Lion/laser_loop_inverted.png", 1, 0);
        laser_spr->SetTint(255,127,0);
        auto tmb       = new TimeBomb(*laser_go, fox->KONLASERDURATION);
        auto cld       = new Collider(*laser_go);
        auto atk       = new Attack(*laser_go, fox->KONDAMAGE, fox->KONKNOCKBACK, fx_go);
        laser_spr->SetSize(fox->KONLASERSIZE.x, fox->KONLASERSIZE.y);
        laser_go->box.SetCenter(associated.box.x - laser_go->box.w/2, associated.box.y + associated.box.h/2+25.0f);
        laser_go->AddComponents({laser_spr, tmb, cld, atk});
    st.bulletArray.emplace_back(laser_go);

    auto [idx, currnode] = stm->GetCurrent();
    currnode->reverse = false;
    currnode->SetFrameTime(fox->KONLASERDURATION);
    stm->ChangeState(idx);
    associated.AddComponent(new TimedTrigger(associated, fox->KONLASERDURATION, KonDisappear));
}

void KonFade(GameObject& associated, float prg)
{
    auto stm = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    auto [idx, currnode] = stm->GetCurrent();
    if(prg == 1.0f)
    {
        auto& st = Game::GetInstance().GetCurrentState();
        auto fx = st.GetObject(C_ID::Fox, &st.enemyArray);
        if(!fx.expired())
        {
            auto fx_go = fx.lock().get();
            auto stm = (StateMachine*)fx_go->GetComponent(C_ID::StateMachine);
            stm->ChangeState(Fox::IDLE);
        }
        SDL_SetTextureAlphaMod(currnode->texture.get(), 255);
        associated.RequestDelete();
        ((Kon*)associated.GetComponent(C_ID::Kon))->ResetWorld();
        return;
    }
    SDL_SetTextureAlphaMod(currnode->texture.get(), (Uint8)((1.0f-prg)*255.0f));
}

void KonDisappear(GameObject& associated)
{
    auto& st = Game::GetInstance().GetCurrentState();
    auto fx = st.GetObject(C_ID::Fox, &st.enemyArray);
    if(fx.expired())
    {
        associated.RequestDelete();
        return;
    }
    auto fx_go = fx.lock().get();
    auto fox = (Fox*)fx_go->GetComponent(C_ID::Fox);

    associated.AddComponent(new ProgressTrigger(associated, fox->KONFADEOUT, KonFade));
}

bool Kon::Is(C_ID type)
{ return C_ID::Kon == type; }