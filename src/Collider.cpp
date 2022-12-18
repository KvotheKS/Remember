#include "Collider.h"
#include "Game.h"
#include "Scheduler.h"

#if defined(DEBUG)
    bool Collider::debugMode = true;
#else
    bool Collider::debugMode = false;
#endif

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset) : GameObject(associated){
    this->scale = scale;
    this->offset = offset;
}

void Collider::Update(float dt){
    box = associated.box;
    box.w *= scale.x;
    box.h *= scale.y;

    float angle = associated.angleDeg * (PI / 180.0);
    Vec2 offsetR = offset.Rotate(angle);

    Vec2 center = associated.box.GetCenter();
    box.SetCenter(center.x + offsetR.x, center.y + offsetR.y);
}

void Collider::Render()
{
    Scheduler::Push(this, associated.depth);
}

void Collider::Print(float x, float y){
    this->Update(0);
        
    if(debugMode){    
        // associated.depth = associated.depth + 100; //fodase tudo que tem collider fica em cima removame depois -m
        
        Vec2 center = box.GetCenter();
        SDL_Point points[5];
        float angle = associated.angleDeg * (PI / 180.0);

        Vec2 point = (Vec2(box.x, box.y) - center).Rotate(angle) + center - Camera::pos;
        points[0] = {(int) point.x, (int) point.y};
        points[4] = {(int) point.x, (int) point.y};

        point = (Vec2(box.x + box.w, box.y) - center).Rotate(angle) + center - Camera::pos;
        points[1] = {(int) point.x, (int) point.y};

        point = (Vec2(box.x + box.w, box.y + box.h) - center).Rotate(angle) + center - Camera::pos;
        points[2] = {(int) point.x, (int) point.y};

        point = (Vec2(box.x, box.y + box.h) - center).Rotate(angle) + center - Camera::pos;
        points[3] = {(int) point.x, (int) point.y};

        SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
            
    }
}

bool Collider::Is(std::string type){
    return type == "Collider";
}

void Collider::SetScale(Vec2 scale){
    this->scale = scale;
}

void Collider::SetOffset(Vec2 offset){
    this->offset = offset;
}

Vec2 Collider::GetScale(){
    return scale;
}
Vec2 Collider::GetOffset(){
    return offset;
}