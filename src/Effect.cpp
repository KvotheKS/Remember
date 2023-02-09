#include "Effect.h"

//moving animated effect
Effect::Effect(GameObject& associated, float angle, float speed, float maxDistance, float duration,
    std::string sprite, int frameCount, float frameTime) : GameObject(associated){
    Sprite* Effect = new Sprite(associated, sprite, frameCount, frameTime);
    Effect->SetScaleX(2,2);
    associated.AddComponent(Effect);


    if(duration == 0)duration = frameCount*frameTime;
    Vec2 direction = Vec2(1, 0).Rotate(angle);
    this->speed = direction * speed;
    this->duration = duration;
    distanceLeft = maxDistance;
}
//animated effect
Effect::Effect(GameObject &associated, float angle, float duration, std::string sprite, int frameCount, float frameTime): GameObject(associated){
    Sprite* Effect = new Sprite(associated, sprite, frameCount, frameTime);
    Effect->SetScaleX(2,2);
    associated.AddComponent(Effect);

    
    if(duration == 0)duration = frameCount*frameTime;
    Vec2 direction = Vec2(1, 0).Rotate(angle);
    this->speed = Vec2(0,0);
    this->duration = duration;
    distanceLeft = 10;


}
//non animated effect
Effect::Effect(GameObject &associated, float angle, float duration, std::string sprite):GameObject(associated){
    this->sprite = new Sprite(associated, sprite, 1);
    this->sprite->SetScaleX(2,2);
    associated.AddComponent(this->sprite);


    if(duration == 0)duration = 1;
    Vec2 direction = Vec2(1, 0).Rotate(angle);
    this->speed = Vec2(0,0);
    this->duration = duration;
    distanceLeft = 10;
}

void Effect::Update(float dt){
   
    Vec2 movement = Vec2(1,0) * dt;
   
   
    associated.box += movement;

    duration_timer.Update(dt);
   
    distanceLeft -= movement.Magnitude();
    
    if(distanceLeft <= 0 || duration_timer.Get() > duration)
        associated.RequestDelete();

}
void Effect::SetColor(Uint8 r , Uint8 g ,Uint8 b, Uint8 a)
{      
    // sprite->SetTint(r,g,b,a);

}
void Effect::Render(){
}

bool Effect::Is(std::string type){
    return type == "Effect";
}

bool Effect::Is(C_ID type)
{ return type == C_ID::Effect; }

