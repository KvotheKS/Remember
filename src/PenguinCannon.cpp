#include "PenguinCannon.h"
#include "Game.h"

PenguinCannon::PenguinCannon(GameObject& associated, std::weak_ptr<GameObject> penguinBody) : GameObject(associated){
    Sprite* pcannon = new Sprite(associated, "assets/img/cubngun.png");
    associated.AddComponent(pcannon);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    pbody = penguinBody;
    angle = 0;
    timer.Update(0.5);
}

void PenguinCannon::Update(float dt){
    InputManager& inManager = InputManager::GetInstance();
    timer.Update(dt);
    if(std::shared_ptr<GameObject> goPBody = pbody.lock()){
        Vec2 center = goPBody->box.GetCenter();
        associated.box.SetCenter(center.x, center.y);

        Vec2 target = Vec2(inManager.GetMouseX() + Camera::pos.x, inManager.GetMouseY() + Camera::pos.y);
        Vec2 pos = associated.box.GetCenter();
        angle = pos.AngleLine(target);
        associated.angleDeg = angle * (180 / PI);

        if(inManager.MousePress(LEFT_MOUSE_BUTTON) && timer.Get() > 0.5){
            Shoot();
            timer.Restart();
        }
    }
    else
        associated.RequestDelete();
}

void PenguinCannon::Render(){
}

bool PenguinCannon::Is(std::string type){
    return type == "PenguinCannon";
}

void PenguinCannon::Shoot(){
    int height, width;
    SDL_GetRendererOutputSize(Game::GetInstance().GetRenderer(), &width, &height);

    Vec2 pos = associated.box.GetCenter();
    Vec2 radius = Vec2(associated.box.w/2, 0);
    pos += radius.Rotate(angle);

    GameObject* goBullet = new GameObject();
    Bullet* bullet = new Bullet(*goBullet, angle, 425, 20, width, "assets/img/penguinbullet.png", 4, 0.1, false);
    goBullet->AddComponent(bullet);
    goBullet->box.SetCenter(pos.x, pos.y);
    goBullet->angleDeg = angle * (180 / PI);

    State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(goBullet);
}