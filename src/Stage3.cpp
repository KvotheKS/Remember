
#include "FpsPrinter.h"
#include "Game.h"
#include "StateMac.h"
#include "SpriteSheetNode.h"
#include "ActionMachine.h"
#include "Player.h"
#include "Enemy.h"
#include "LionBoss.h"
#include "Stage3.h"
#include "ScreenFade.h"
#include "Gate.h"
#include "Stage2.h"
#include "Stage4.h"
#include "StageMask.h"


Stage3::Stage3() : State(){
    backgroundMusic.Open("assets/audio/StageState.ogg");
    tileSet = new TileSet(60, 60, "assets/img/Tileset02.png");
}

Stage3::~Stage3(){
    delete tileSet;
    for(auto it : GetArrays())
        it->clear();
}

void Stage3::LoadAssets(){

    //FADE IN EFFECT
    GameObject* GO_fade = new GameObject();
        GO_fade->depth = 1000;
        float fadeinTime = 0; float fadeoutTime = 0.8;float duration = 1;
        ScreenFade* effect = new ScreenFade(*GO_fade, fadeinTime, fadeoutTime, duration);
        
        GO_fade->AddComponent(effect);
    
        State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(GO_fade);

    //BACKGROUND
    GameObject* goBackground = new GameObject();
        goBackground->depth = -1;
        Sprite* bg = new Sprite(*goBackground, "assets/img/Stage2/stg02bgl01.png");
        goBackground->AddComponent(bg);

        goBackground->box.x = 0;
        goBackground->box.y = 60*20;
    cameraFollowerObjectArray.emplace_back(goBackground);
    
    GameObject* goBackground2 = new GameObject();
        goBackground2->depth = -2;
        bg = new Sprite(*goBackground2, "assets/img/Stage2/stg02bgl02.png");
        goBackground2->AddComponent(bg);


        goBackground2->box.x = 0;
        goBackground2->box.y = 60*18;
    cameraFollowerObjectArray.emplace_back(goBackground2);

    GameObject* goBackground3 = new GameObject();
        goBackground3->depth = -3;
        bg = new Sprite(*goBackground3, "assets/img/Stage2/stg02bgl03.png");
        goBackground3->AddComponent(bg);


        goBackground3->box.x = 0;
        goBackground3->box.y = 60*20;
    cameraFollowerObjectArray.emplace_back(goBackground3);

    GameObject* goBackground4 = new GameObject();
        goBackground4->depth = -4;
        bg = new Sprite(*goBackground4, "assets/img/Stage2/stg02bgl04.png");
        goBackground4->AddComponent(bg);


        goBackground4->box.x = 60*4;
        goBackground4->box.y = 60*10;
    cameraFollowerObjectArray.emplace_back(goBackground4);

    // GameObject* goBackground5 = new GameObject();
    //     goBackground5->depth = -5;
    //     bg = new Sprite(*goBackground5, "assets/img/Stage2/stg2bgcolor.png");
    //     goBackground4->AddComponent(bg);


    //     goBackground5->box.x = 0;
    //     goBackground5->box.y = 0;
    // cameraFollowerObjectArray.emplace_back(goBackground5);

    GameObject* goBackground5 = new GameObject();
        goBackground5->depth = -10;
        bg = new Sprite(*goBackground5, "assets/img/Stage2/stg02Moon.png");
        goBackground5->AddComponent(bg);

        // goBackground5->AddComponent(new CameraFollower(*goBackground5));
        goBackground5->box.x = 60*3.5;
        goBackground5->box.y = 60;
    cameraFollowerObjectArray.emplace_back(goBackground5);

    GameObject* goBackground6 = new GameObject();
        goBackground6->depth = 1;
        bg = new Sprite(*goBackground6, "assets/img/Stage2/stg03bgTree.png");
        goBackground6->AddComponent(bg);

        // goBackground5->AddComponent(new CameraFollower(*goBackground6));
        goBackground6->box.x = 60*20;
        goBackground6->box.y = 60*25+10;
    cameraFollowerObjectArray.emplace_back(goBackground6);


    //STAGEART
    //FOREGROUND
    //OBJECTS
    //GATES
    GameObject* GO_Gate = new GameObject();
        int spawnpoint = 1;
        bool active = false;
        GO_Gate->AddComponent(new Gate(*GO_Gate,new Stage2(),spawnpoint,active));
        GO_Gate->box.x = 60*-1.8;
        GO_Gate->box.y = 60*33;
    terrainArray.emplace_back(GO_Gate);
    
    GameObject* GO_Gate2 = new GameObject();
        spawnpoint = 0;
        active = true;
        GO_Gate2->AddComponent(new Gate(*GO_Gate2,new Stage4(),spawnpoint,active));
        GO_Gate2->box.x = 60*89.8;
        GO_Gate2->box.y = 60*6;
    terrainArray.emplace_back(GO_Gate2);

    GameObject* GO_Gatemask = new GameObject();
        spawnpoint = 0;
        active = true;
        GO_Gatemask->AddComponent(new Gate(*GO_Gatemask,new StageMask(),spawnpoint,active));
        GO_Gatemask->box.x = 60*30;
        GO_Gatemask->box.y = 60*32;
    terrainArray.emplace_back(GO_Gatemask);
    
    GameObject* fpsChecker = new GameObject();
        fpsChecker->depth = 9999;
        fpsChecker->AddComponent(new FpsPrinter(*fpsChecker));
        std::string file = "assets/font/Call me maybe.ttf";
        fpsChecker->AddComponent(new Text(*fpsChecker, file, 35, Text::TextStyle::BLENDED, "0", {0, 100, 255, 255}, 0.35));
        fpsChecker->AddComponent(new CameraFollower(*fpsChecker));
    cameraFollowerObjectArray.emplace_back(fpsChecker);
    
    //PLAYER
    spawnList ={Vec2(1,34),Vec2(88,6),Vec2(33,34)};
    GameObject* player_GO = new GameObject();
        player_GO->depth = 10;


        StateMachine* st = new StateMachine(*player_GO);
        player_GO->AddComponent(st);
        Collider* collider = new Collider(*player_GO);
        player_GO->AddComponent(collider);
        RigidBody* box = new RigidBody(*player_GO);
        player_GO->AddComponent(box);
        Player* pl = new Player(*player_GO);
        player_GO->AddComponent(pl);
        
        
        player_GO->box.SetCenter(spawnList[GameData::spawn_ID].x*60, spawnList[GameData::spawn_ID].y*60);
      
    rigidArray.emplace_back(player_GO);

  

    backgroundMusic.Play();

    Camera::Follow(player_GO);

    GameObject* goTileMap = new GameObject();
        goTileMap->depth = 2;
        TileMap* tileMap = new TileMap(*goTileMap, "assets/map/tilemap_state3.txt", tileSet);
        goTileMap->AddComponent(tileMap);
        goTileMap->box.x = 0;
        goTileMap->box.y = 0;
    objectArray.emplace_back(goTileMap);

    Camera::SetCameraFunction(&Camera::FollowTarget);
    Camera::SetCameraTransition([]() -> bool {return Camera::pos.x >= 5967.5f;}, &Camera::Stationary);
    Camera::Bounds = Rect(0,0, 60*tileMap->GetWidth(), 60*tileMap->GetHeight());



    
}

void Stage3::Start(){
    
    LoadAssets();
    StartArray();
   
}

void Stage3::Update(float dt){
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
    }
    

    /* ordem de update necessaria
    */
    // std::cout << "ENTRA";
    UpdateArray(dt);
    // std::cout << "SAI";

    CollideVectors(rigidArray, terrainArray);
    // std::cout << "colidiu";
    
    Camera::Update(dt);
   
    KillDeads();
}

void Stage3::CollideVectors(std::vector<std::shared_ptr<GameObject>>& alpha, std::vector<std::shared_ptr<GameObject>>& beta)
{
    int i = 0;
    for(auto& alphaObj : alpha)
    {
        Collider* colliderA = (Collider*) alphaObj->GetComponent(C_ID::Collider);
        if(!colliderA)
            continue;
        float angleOfA = alphaObj->angleDeg * (PI / 180.0);
        for(auto& betaObj : beta)
        {
            
            Collider* colliderB = (Collider*) betaObj->GetComponent(C_ID::Collider);
            if(!colliderB)
                continue;
            
            float angleOfB = betaObj->angleDeg * (PI / 180.0);
            
            auto [flag, sep] = Collision::IsColliding(colliderA->box, colliderB->box, angleOfA, angleOfB);
            // std::cout << "\nCOLLISION TESTED";
            if(flag){    
                // std::cout << "COL MSG\n";
                alphaObj->NotifyCollisionBehavior(*betaObj,sep);
                // std::cout << "B INFORMED\n";
                betaObj->NotifyCollisionBehavior(*alphaObj,sep);

            }
            // std::cout << '\n' << i;
            i++;
        }
    }
    // std::cout << "end\n";;
}

void Stage3::Render(){
    RenderArray();
}

void Stage3::Pause(){
}

void Stage3::Resume(){
}

void Stage3::Collision(){
}


