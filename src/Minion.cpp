#include "Minion.h"
#include "Game.h"

Minion::Minion(GameObject& associated, std::weak_ptr<GameObject> alienCenter, float arcOffsetDeg)
: Component(associated){
    Sprite* minion = new Sprite(associated, "assets/img/minion.png");
    float scale = (((float) rand() / (float) RAND_MAX) * 0.5) + 1.0;
    minion->SetScaleX(scale, scale);
    associated.AddComponent(minion);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    this->alienCenter = alienCenter;
    arc = arcOffsetDeg * (PI / 180.0);
    associated.angleDeg = arcOffsetDeg;

    if(std::shared_ptr<GameObject> alienGO = alienCenter.lock())
        associated.box.SetCenter(alienGO->box.x + 110, alienGO->box.y + 0);
}

void Minion::Update(float dt){
    if(std::shared_ptr<GameObject> orig = alienCenter.lock()){
        float angSpeed = PI / 4;
        Vec2 radius = Vec2(110, 0);
        Vec2 newPos = radius.Rotate(arc) + orig->box.GetCenter();

        associated.box.SetCenter(newPos.x, newPos.y);
        arc += angSpeed * dt;

        associated.angleDeg = newPos.AngleLine(orig->box.GetCenter()) * (180 / PI) - 90;
    }
    else
        associated.RequestDelete();
}

void Minion::Render(){
}

bool Minion::Is(std::string type){
    return type == "Minion";
}

void Minion::Shoot(Vec2 target){
    int height, width;
    SDL_GetRendererOutputSize(Game::GetInstance().GetRenderer(), &width, &height);

    Vec2 pos = associated.box.GetCenter();
    Vec2 radius = Vec2(associated.box.w/2, 0);
    float angle = pos.AngleLine(target);
    pos += radius.Rotate(angle);

    GameObject* goBullet = new GameObject();
    Bullet* bullet = new Bullet(*goBullet, angle, 425, 15, width, "assets/img/minionbullet2.png", 3, 0.1, true);
    goBullet->AddComponent(bullet);
    goBullet->box.SetCenter(pos.x, pos.y);
    goBullet->angleDeg = angle * (180 / PI);

    State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(goBullet);
}