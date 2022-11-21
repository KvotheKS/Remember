#include "Alien.h"
#include "Game.h"

int Alien::alienCount = 0;

Alien::Alien(GameObject& associated, int nMinions, float timeOffset) : Component(associated){
    Sprite* alien = new Sprite(associated, "assets/img/alien.png");
    associated.AddComponent(alien);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    hp = 240;
    speed = Vec2();
    minionArray.reserve(nMinions);
    this->timeOffset = timeOffset;

    alienCount++;
    restTimer.Restart();
    state = AlienState::RESTING;
}

Alien::~Alien(){
    minionArray.clear();
    alienCount--;
}

void Alien::Start(){
    int nMinions = minionArray.capacity();
    State& state = Game::GetInstance().GetCurrentState();

    for(int i = 0; i < nMinions; i++){
        GameObject* goMinion = new GameObject();
        std::weak_ptr<GameObject> alienCenter = state.GetObjectPtr(&associated);
        Minion* minion = new Minion(*goMinion, alienCenter, (360 / nMinions) * i);
        goMinion->AddComponent(minion);
        minionArray.push_back(state.AddObject(goMinion));
    }
}

void Alien::Update(float dt){
    float angSpeed = PI / 6;
    associated.angleDeg -= angSpeed * dt * (180 / PI);

    if(PenguinBody::player != nullptr){
        Vec2 alien = associated.box.GetCenter();

        if(state == AlienState::RESTING){
            if(restTimer.Get() > 1.5 + timeOffset){
                int linearSpeed = 275;
                destination = PenguinBody::player->GetPlayerCenter();
                Vec2 direction = (destination - alien).Normalize();

                speed = direction * linearSpeed;
                state = AlienState::MOVING;
            }
            else
                restTimer.Update(dt);
        }
        else if(state == AlienState::MOVING){
            if(alien.Distance(destination) < 300){
                destination = PenguinBody::player->GetPlayerCenter();

                int idx = 0;
                float minDist = std::numeric_limits<float>::max();

                for(unsigned i = 0; i < minionArray.size(); i++){
                    if(std::shared_ptr<GameObject> goMinion = minionArray[i].lock()){
                        Vec2 pos = goMinion->box.GetCenter();
                        float dist = pos.Distance(destination);
                        if(dist <= minDist){
                            idx = i;
                            minDist = dist;
                        }
                    }
                }

                if(std::shared_ptr<GameObject> goMinion = minionArray[idx].lock()){
                    Minion* minion = (Minion*) goMinion->GetComponent("Minion");
                    minion->Shoot(destination);
                }

                restTimer.Restart();
                state = AlienState::RESTING;
            }
            else{
                alien += speed * dt;
                associated.box.SetCenter(alien.x, alien.y);
            }
        }
    }
}

void Alien::Render(){
}

bool Alien::Is(std::string type){
    return type == "Alien";
}

void Alien::NotifyCollision(GameObject& other){
    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if(bullet != nullptr && !bullet->targetsPlayer){
        hp -= bullet->GetDamage();
        other.RequestDelete();
        if(hp < 1){
            State& state = Game::GetInstance().GetCurrentState();
            GameObject* goAlienDeath = new GameObject();

            Sprite* alienDeath = new Sprite(*goAlienDeath, "assets/img/aliendeath.png", 4, 0.1, 0.4);
            goAlienDeath->AddComponent(alienDeath);

            Sound* alienBoom = new Sound(*goAlienDeath, "assets/audio/boom.wav");
            alienBoom->Play();
            goAlienDeath->AddComponent(alienBoom);

            Vec2 center = associated.box.GetCenter();
            goAlienDeath->box.SetCenter(center.x, center.y);
            goAlienDeath->angleDeg = associated.angleDeg;

            state.AddObject(goAlienDeath);

            for(unsigned i = 0; i < minionArray.size(); i++){
                if(std::shared_ptr<GameObject> goMinion = minionArray[i].lock()){
                    GameObject* goMinionDeath = new GameObject();
                    Sprite* minionSprite = (Sprite*) goMinion->GetComponent("Sprite");
                    Vec2 scale = minionSprite->GetScale();

                    Sprite* minionDeath = new Sprite(*goMinionDeath, "assets/img/miniondeath.png", 4, 0.1, 0.4);
                    minionDeath->SetScaleX(scale.x, scale.y);
                    goMinionDeath->AddComponent(minionDeath);

                    center = goMinion->box.GetCenter();
                    goMinionDeath->box.SetCenter(center.x, center.y);
                    goMinionDeath->angleDeg = goMinion->angleDeg;

                    state.AddObject(goMinionDeath);
                }
            }

            associated.RequestDelete();
        }
    }
}