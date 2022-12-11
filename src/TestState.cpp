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
    goBackground->depth = 1;
    Sprite* bg = new Sprite(*goBackground, "assets/img/space.jpg");
    goBackground->AddComponent(bg);

    CameraFollower* cf = new CameraFollower(*goBackground);
    goBackground->AddComponent(cf);

    goBackground->box.x = 0;
    goBackground->box.y = 0;
    cameraFollowerObjectArray.emplace_back(goBackground);

    // GameObject* goTileMap = new GameObject();
    // goTileMap->depth = 0;
    // TileMap* tileMap = new TileMap(*goTileMap, "assets/map/tileMap.txt", tileSet);
    // goTileMap->AddComponent(tileMap);
    // goTileMap->box.x = 0;
    // goTileMap->box.y = 0;
    // objectArray.emplace_back(goTileMap);

    
    
    // GameObject* fpsChecker = new GameObject();
    // fpsChecker->depth = 9999;
    // fpsChecker->AddComponent(new FpsPrinter(*fpsChecker));
    // std::string file = "assets/font/Call me maybe.ttf";
    // fpsChecker->AddComponent(new Text(*fpsChecker, file, 35, Text::TextStyle::BLENDED, "0", {0, 100, 255, 255}, 0.35));
    // fpsChecker->AddComponent(new CameraFollower(*fpsChecker));
    // objectArray.emplace_back(fpsChecker);
    
    GameObject* goBox1 = new GameObject();
        RigidBody* box = new RigidBody(*goBox1,1);
        goBox1->AddComponent(box);
        goBox1->box.SetCenter(100, 100);
    objectArray.emplace_back(goBox1);

    backgroundMusic.Play();

    Camera::Follow(goBox1);

    for(int i = 0; i<15; i++){
        GameObject* goBox2 = new GameObject();
        TerrainBody* box2 = new TerrainBody(*goBox2);
        goBox2->AddComponent(box2);
        goBox2->box.SetCenter(i*100-500, 500);
        goBox2->angleDeg = 0;
        objectArray.emplace_back(goBox2);
    }
    
}

void TestState::Start(){
    LoadAssets();
    StartArray();
}

void TestState::Update(float dt){
    // removendo condicao de vitoria/derrota -m
    
    // if(PenguinBody::player == nullptr){
    //     GameData::playerVictory = false;
    //     popRequested = true;
    //     backgroundMusic.Stop(500);
    //     Game::GetInstance().Push(new EndState());
    // }

    

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
        

    /* ordem de update necessaria

    */
    
    UpdateArray(dt);
    Camera::Update(dt);
    
    
     

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

            if((Collision::IsColliding(colliderA->box, colliderB->box, angleOfA, angleOfB)).first){
                Vec2 sep = (Collision::IsColliding(colliderA->box, colliderB->box, angleOfA, angleOfB)).second;
               
                objectArray[i]->NotifyCollisionBehavior(*objectArray[j],sep);
                objectArray[j]->NotifyCollisionBehavior(*objectArray[i],sep);
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