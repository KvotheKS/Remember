#include "ScreenFade.h"
#include "Game.h"
#include "Scheduler.h"
#include "Rand.h"

ScreenFade::ScreenFade(GameObject& associated, float fadeinTime, float fadeoutTime,float duration): GameObject(associated){
    
    local_state = FSTATE::F_IN;
    this->duration.SetFinish(duration);
    fadeTimer.SetFinish(fadeinTime);
    this->fadeoutTime = fadeoutTime;
    this->fadeinTime = fadeinTime;
    SetClip(0,0,1280,720);
    SetColor(0,0,0,0);

}
ScreenFade::~ScreenFade(){
    
}

void ScreenFade::Open(std::string file){

}
void ScreenFade::SetClip(int x, int y, int w, int h){
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void ScreenFade::SetColor(Uint8 r , Uint8 g ,Uint8 b, Uint8 a)
{   
    color = { r, g, b ,a};

}

void ScreenFade::Update(float dt){ 
    if(duration.Update(dt)){
        
        associated.RequestDelete();
    }
        
    
 
    if(fadeTimer.Update(dt) || fadeTimer.GetFinish() == 0){
       
        switch(local_state){
            case FSTATE::F_IN:
                local_state = FSTATE::SOLID;
                fadeTimer.Restart();
                fadeTimer.SetFinish(duration.GetFinish()-fadeoutTime-fadeinTime);
            
            break;

            case FSTATE::SOLID:
                local_state = FSTATE::F_OUT;
                fadeTimer.Restart();
                fadeTimer.SetFinish(fadeoutTime);

                
            break;
        }
    }

    Uint8 alpha ;
    switch(local_state){
        case FSTATE::F_IN:
            alpha = 255*(fadeTimer.Get()/fadeinTime);
           
            break;

        case FSTATE::SOLID:
            alpha = 255;
            break;

        case FSTATE::F_OUT:
            alpha = 255-(255*(fadeTimer.Get()/fadeoutTime));
            
            break;
    }

    SetColor(color.r,color.g,color.b,(int)alpha);
   
}

void ScreenFade::Render(){
    
    Render(0,0);
}

void ScreenFade::Render(float x, float y){
    auto deep = associated.depth; 
  
    Scheduler::Push(this, deep , x, y);
   
}

void ScreenFade::Print(float x, float y){
    SDL_SetRenderDrawBlendMode( Game::GetInstance().GetRenderer(),SDL_BLENDMODE_BLEND);//!! talvez so preciso setir isso uma vez testar dps to com preguiça
    SDL_SetRenderDrawColor( Game::GetInstance().GetRenderer(), color.r, color.g, color.b, color.a );
    SDL_RenderFillRect(Game::GetInstance().GetRenderer(),&clipRect) ;
    
}
bool ScreenFade::Is(std::string type){
    return type == "Sprite";
}
bool ScreenFade::Is(C_ID){

}