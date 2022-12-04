#include "PenguinBody.h"
#include "Game.h"

PenguinBody* PenguinBody::player = nullptr;

PenguinBody::PenguinBody(GameObject& associated) : GameObject(associated){
    Sprite* pbody = new Sprite(associated, "assets/img/penguin.png");
    associated.AddComponent(pbody);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    speed = Vec2();
    linearSpeed = 0;
    angle = 0;
    hp = 180;
    player = this;
}

PenguinBody::~PenguinBody(){
    player = nullptr;
}

void PenguinBody::Start(){
    State& state = Game::GetInstance().GetCurrentState();
   
    GameObject* goPCannon = new GameObject();
    goPCannon->depth = associated.depth+0.0001f;
    std::weak_ptr<GameObject> penguinBody = state.GetObjectPtr(&associated);
    PenguinCannon* penguinCannon = new PenguinCannon(*goPCannon, penguinBody);
    goPCannon->AddComponent(penguinCannon);

    pcannon = state.AddObject(goPCannon);
}

void PenguinBody::Update(float dt){
    InputManager& inManager = InputManager::GetInstance();
    float acceleration = 200;
    float friction = 100;
    if(inManager.IsKeyDown(W_KEY)){
        if(linearSpeed < MAX_SPEED)
            linearSpeed += acceleration * dt;
        else
            linearSpeed = MAX_SPEED;
    }
    else if(inManager.IsKeyDown(S_KEY)){
        if(linearSpeed > MIN_SPEED)
            linearSpeed -= acceleration * dt;
        else
            linearSpeed = MIN_SPEED;
    }
    else{
        if(abs(linearSpeed) < THRESHOLD)
            linearSpeed = 0;
        else if(linearSpeed > 0)
            linearSpeed -= friction * dt;
        else if(linearSpeed < 0)
            linearSpeed += friction * dt;
    }

    float angSpeed = PI;
    if(inManager.IsKeyDown(D_KEY))
        angle += angSpeed * dt;
    if(inManager.IsKeyDown(A_KEY))
        angle -= angSpeed * dt;
    associated.angleDeg = angle * (180 / PI);

    Vec2 direction = Vec2(1, 0).Rotate(angle);
    speed = direction * linearSpeed;
    associated.box += speed * dt;

    associated.box.x = std::max(associated.box.x, 0.f);
    associated.box.x = std::min(associated.box.x, 1408.f - associated.box.w);
    associated.box.y = std::max(associated.box.y, 0.f);
    associated.box.y = std::min(associated.box.y, 1280.f - associated.box.h);
}

void PenguinBody::Render(){
}

bool PenguinBody::Is(std::string type){
    return type == "PenguinBody";
}

void PenguinBody::NotifyCollision(GameObject& other){
    cout << "here\n";
    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if(bullet != nullptr && bullet->targetsPlayer){
        hp -= bullet->GetDamage();
        other.RequestDelete();
        if(hp < 1){
            State& state = Game::GetInstance().GetCurrentState();
            GameObject* goPenguinDeath = new GameObject();

            Sprite* penguinDeath = new Sprite(*goPenguinDeath, "assets/img/penguindeath.png", 5, 0.1, 0.5);
            goPenguinDeath->AddComponent(penguinDeath);

            Sound* penguinBoom = new Sound(*goPenguinDeath, "assets/audio/boom.wav");
            penguinBoom->Play();
            goPenguinDeath->AddComponent(penguinBoom);

            Vec2 center = associated.box.GetCenter();
            goPenguinDeath->box.SetCenter(center.x, center.y);

            state.AddObject(goPenguinDeath);

            if(std::shared_ptr<GameObject> goPCannon = pcannon.lock())
                goPCannon->RequestDelete();
            associated.RequestDelete();
            Camera::Unfollow();
        }
    }
}

Vec2 PenguinBody::GetPlayerCenter(){
    return associated.box.GetCenter();
}