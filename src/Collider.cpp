#include "Collider.h"
#include "Game.h"
#include "Scheduler.h"

#if defined(DEBUG)
    bool Collider::debugMode = true;
#else
    bool Collider::debugMode = false;
#endif

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset, C_ID type) : GameObject(associated){
    this->scale = scale;
    this->offset = offset;
    this->type = type;
}
void Collider::Start(){
}
void Collider::Update(float dt){
    auto& inManager = InputManager::GetInstance();
    box = associated.box;
    box.w *= scale.x;
    box.h *= scale.y;
    float angle = associated.angleDeg * (PI / 180.0);
    Vec2 offsetR = offset.Rotate(angle);
    Vec2 center = associated.box.GetCenter();
    box.SetCenter(center.x + offsetR.x, center.y + offsetR.y);
    // if(associated.GetComponent(C_ID::Player) && inManager.IsKeyDown(Q_KEY))
    //     std::cout << this->box;
}

void Collider::Render()
{
    // std::cout << "COLLIDER RENDER\n";
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
        // if(associated.GetComponent(C_ID::Attack))
        //     std::cout << "COL: " << points[0].x << ' ' << points[0].y << ' ' << points[2].x << ' ' << points[2].y << '\n';
        
        
        SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
            
    }
}

bool Collider::Is(std::string type){
    return type == "Collider";
}

bool Collider::Is(C_ID type)
{ return type == C_ID::Collider || type == this->type; }

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