#include "Bullet.h"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, std::string sprite,
               int frameCount, float frameTime, bool targetsPlayer) : GameObject(associated){
    Sprite* bullet = new Sprite(associated, sprite, frameCount, frameTime);
    associated.AddComponent(bullet);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    this->targetsPlayer = targetsPlayer;

    Vec2 direction = Vec2(1, 0).Rotate(angle);
    this->speed = direction * speed;

    this->damage = damage;
    distanceLeft = maxDistance;
}

void Bullet::Update(float dt){
    Vec2 movement = speed * dt;
    associated.box += movement;

    distanceLeft -= movement.Magnitude();
    if(distanceLeft <= 0)
        associated.RequestDelete();
}

void Bullet::Render(){
}

bool Bullet::Is(std::string type){
    return type == "Bullet";
}

int Bullet::GetDamage(){
    return damage;
}