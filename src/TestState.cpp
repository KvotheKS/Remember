#include "TestState.h"
#include "FpsPrinter.h"
#include "Game.h"

//test stage pra mecher em collision

TestState::TestState() : State(){
    backgroundMusic.Open("assets/audio/StageState.ogg");
    tileSet = new TileSet(64, 64, "assets/img/tileset.png");
}

TestState::~TestState(){
    delete tileSet;
    objectArray.clear();
}

void TestState::LoadAssets(){
    GameObject* goBackground = new GameObject();
    goBackground->depth = -1;
    Sprite* bg = new Sprite(*goBackground, "assets/img/ocean.jpg");
    goBackground->AddComponent(bg);

    CameraFollower* cf = new CameraFollower(*goBackground);
    goBackground->AddComponent(cf);

    goBackground->box.x = 0;
    goBackground->box.y = 0;
    objectArray.emplace_back(goBackground);

    GameObject* goTileMap = new GameObject();
    goTileMap->depth = 0;
    TileMap* tileMap = new TileMap(*goTileMap, "assets/map/tileMap.txt", tileSet);
    goTileMap->AddComponent(tileMap);
    goTileMap->box.x = 0;
    goTileMap->box.y = 0;
    objectArray.emplace_back(goTileMap);

    // GameObject* goPBody = new GameObject();
    // goPBody->depth = 0;
    // PenguinBody* pbody = new PenguinBody(*goPBody);
    // goPBody->AddComponent(pbody);
    // goPBody->box.SetCenter(704, 640);
    // objectArray.emplace_back(goPBody);
    // Camera::Follow(goPBody);

    // for(int i = 0; i < 3; i++){
    //     GameObject* goAlien = new GameObject();
    //     Alien* alien = new Alien(*goAlien, rand() % 2 ? 5 : 7, rand() % 4);
    //     goAlien->AddComponent(alien);

    //     Vec2 alienPos = Vec2(rand() % 1409, rand() % 1281);
    //     while(alienPos.Distance(PenguinBody::player->GetPlayerCenter()) <= 500)
    //         alienPos = Vec2(rand() % 1409, rand() % 1281);
    //     goAlien->box.SetCenter(alienPos.x, alienPos.y);

    //     objectArray.emplace_back(goAlien);
    // }
    
    GameObject* fpsChecker = new GameObject();
    fpsChecker->depth = 9999;
    fpsChecker->AddComponent(new FpsPrinter(*fpsChecker));
    std::string file = "assets/font/Call me maybe.ttf";
    fpsChecker->AddComponent(new Text(*fpsChecker, file, 35, Text::TextStyle::BLENDED, "0", {0, 100, 255, 255}, 0.35));
    fpsChecker->AddComponent(new CameraFollower(*fpsChecker));
    objectArray.emplace_back(fpsChecker);
    
    GameObject* goBox1 = new GameObject();
        RigidBody* box = new RigidBody(*goBox1,1);
        goBox1->AddComponent(box);
        goBox1->box.SetCenter(100, 100);
    objectArray.emplace_back(goBox1);

    backgroundMusic.Play();

    GameObject* goBox2 = new GameObject();
        RigidBody* box2 = new RigidBody(*goBox2,0);
        goBox2->AddComponent(box2);
        goBox2->box.SetCenter(300, 300);
        goBox2->angleDeg = 35;
    objectArray.emplace_back(goBox2);
}

void TestState::Start(){
    LoadAssets();
    StartArray();
}

void TestState::Update(float dt){
    // if(PenguinBody::player == nullptr){
    //     GameData::playerVictory = false;
    //     popRequested = true;
    //     backgroundMusic.Stop(500);
    //     Game::GetInstance().Push(new EndState());
    // }

    // removendo condicao de vitoria -m

    // else if(Alien::alienCount == 0){
    //     GameData::playerVictory = true;
    //     popRequested = true;
    //     backgroundMusic.Stop(0);
    //     Game::GetInstance().Push(new EndState());
    // }

    InputManager& inManager = InputManager::GetInstance();

    if(inManager.QuitRequested())
        quitRequested = true;
    if(inManager.KeyPress(ESCAPE_KEY))
        popRequested = true;
    if(inManager.KeyPress(F3_KEY)){
        Collider::debugMode = !Collider::debugMode;
        std::cout << "hit: " << Collider::debugMode << std::endl;
    }
        

    Camera::Update(dt);

    UpdateArray(dt);

    for(unsigned i = 0; i < objectArray.size(); i++){
        Collider* colliderA = (Collider*) objectArray[i]->GetComponent("Collider");
        if(colliderA == nullptr)
            continue;
        float angleOfA = objectArray[i]->angleDeg * (PI / 180.0);

        for(unsigned j = i + 1; j < objectArray.size(); j++){
            Collider* colliderB = (Collider*) objectArray[j]->GetComponent("Collider");
            if(colliderB == nullptr)
                continue;
            float angleOfB = objectArray[j]->angleDeg * (PI / 180.0);

            if(Collision::IsColliding(colliderA->box, colliderB->box, angleOfA, angleOfB)){
                objectArray[i]->NotifyCollision(*objectArray[j]);
                objectArray[j]->NotifyCollision(*objectArray[i]);
            }
        }
    }

    for(unsigned i = 0; i < objectArray.size(); i++){
        if(objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin() + i);
            i--;
        }
    }
}

void TestState::Render(){
    RenderArray();
}

void TestState::Pause(){
}

void TestState::Resume(){
}