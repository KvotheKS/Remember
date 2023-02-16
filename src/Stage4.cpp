
#include "FpsPrinter.h"
#include "Game.h"
#include "StateMac.h"
#include "SpriteSheetNode.h"
#include "ActionMachine.h"
#include "Player.h"
#include "Enemy.h"
#include "LionBoss.h"
#include "Stage4.h"
#include "ScreenFade.h"
#include "Gate.h"
#include "Stage3.h"
#include "Stage4.h"
#include "StageFox.h"


Stage4::Stage4() : State(){
    backgroundMusic.Open("assets/audio/StageState.ogg");
    tileSet = new TileSet(60, 60, "assets/img/Tileset01.png");
}

Stage4::~Stage4(){
    delete tileSet;
    for(auto it : GetArrays())
        it->clear();
}

void Stage4::LoadAssets(){

    //FADE IN EFFECT
    GameObject* GO_fade = new GameObject();
        GO_fade->depth = 1000;
        float fadeinTime = 0; float fadeoutTime = 0.8;float duration = 1;
        ScreenFade* effect = new ScreenFade(*GO_fade, fadeinTime, fadeoutTime, duration);
        
        GO_fade->AddComponent(effect);
    
        State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(GO_fade);
     //TILEMAP
    GameObject* goTileMap = new GameObject();
        goTileMap->depth = 2;
        TileMap* tileMap = new TileMap(*goTileMap, "assets/map/tilemap_stage4.txt", tileSet);
        goTileMap->AddComponent(tileMap);
        goTileMap->box.x = 0;
        goTileMap->box.y = 0;
    objectArray.emplace_back(goTileMap);
    //BACKGROUND
    GameObject* goBackground = new GameObject();
        goBackground->depth = -6;
        Sprite* bg = new Sprite(*goBackground, "assets/img/Stage4/stg4_bgl02.png");
        goBackground->AddComponent(bg);

        goBackground->box.x = 60*0;
        goBackground->box.y = 60*15;
    cameraFollowerObjectArray.emplace_back(goBackground);
    
    GameObject* goBackground2 = new GameObject();
        goBackground2->depth = -8;
        bg = new Sprite(*goBackground2, "assets/img/Stage4/stg4_bgl03.png");
        goBackground2->AddComponent(bg);


        goBackground2->box.x = 0;
        goBackground2->box.y = 60*10;
    cameraFollowerObjectArray.emplace_back(goBackground2);

    


    //STAGEART
    GameObject* art1 = new GameObject();
        art1->depth = -1;
        bg = new Sprite(*art1, "assets/img/Stage4/stg4_fgltree01.png");
        art1->AddComponent(bg);


        art1->box.x = 60*38;
        art1->box.y = 60*10;
    cameraFollowerObjectArray.emplace_back(art1);

    GameObject* art2 = new GameObject();
        art2->depth = 101;
        bg = new Sprite(*art2, "assets/img/Stage4/stg4_fgltree02.png");
        bg->SetScaleX(1.2,1.2);
        art1->AddComponent(bg);


        art2->box.x = 60*2;
        art2->box.y = 60*48;
    cameraFollowerObjectArray.emplace_back(art1);

    //FOREGROUND
    GameObject* goForeground1 = new GameObject();
        goForeground1->depth = 101;
        bg = new Sprite(*goForeground1, "assets/img/Stage4/stg4_fgl01.png");
        goForeground1->AddComponent(bg);


        goForeground1->box.x = 60*30;
        goForeground1->box.y = 60*30;
    cameraFollowerObjectArray.emplace_back(goForeground1);

      GameObject* goForeground2 = new GameObject();
        goForeground2->depth = 102;
        bg = new Sprite(*goForeground2, "assets/img/Stage4/stg4_fgl02.png");
        goForeground2->AddComponent(bg);


        goForeground2->box.x = 60*30;
        goForeground2->box.y = 60*48;
    cameraFollowerObjectArray.emplace_back(goForeground2);
    
    //OBJECTS
    
    GameObject* fpsChecker = new GameObject();
        fpsChecker->depth = 9999;
        fpsChecker->AddComponent(new FpsPrinter(*fpsChecker));
        std::string file = "assets/font/Call me maybe.ttf";
        fpsChecker->AddComponent(new Text(*fpsChecker, file, 35, Text::TextStyle::BLENDED, "0", {0, 100, 255, 255}, 0.35));
        fpsChecker->AddComponent(new CameraFollower(*fpsChecker));
    cameraFollowerObjectArray.emplace_back(fpsChecker);
    //GATES
    GameObject* GO_Gate = new GameObject();
        int spawnpoint = 1;
        bool active = true;
        GO_Gate->AddComponent(new Gate(*GO_Gate,new Stage3(),spawnpoint,active));
        GO_Gate->box.x = 60*-1.8;
        GO_Gate->box.y = 60*55;
    terrainArray.emplace_back(GO_Gate);
    GameObject* GO_Gate2 = new GameObject();
        spawnpoint = 0;
        active = true;
        GO_Gate2->AddComponent(new Gate(*GO_Gate2,new StageFox(),spawnpoint,active));
        GO_Gate2->box.x = 60*64;
        GO_Gate2->box.y = 60*5;
    terrainArray.emplace_back(GO_Gate2);
    //PLAYER
    spawnList ={Vec2(2,53),Vec2(64,6)};
    GameObject* player_GO = new GameObject();
        player_GO->depth = 10;


        StateMachine* st = new StateMachine(*player_GO);
        player_GO->AddComponent(st);
        Collider* collider = new Collider(*player_GO);
        player_GO->AddComponent(collider);
        RigidBody* box = new RigidBody(*player_GO);
        player_GO->AddComponent(box);
        Player* pl = new Player(*player_GO);
        pl->Bounds = Rect(-70,-500, 60*tileMap->GetWidth()+140,60*tileMap->GetHeight()+500);
        player_GO->AddComponent(pl);
        
        
        player_GO->box.SetCenter(spawnList[GameData::spawn_ID].x*60, spawnList[GameData::spawn_ID].y*60);
      
    rigidArray.emplace_back(player_GO);

  

    backgroundMusic.Play();

    Camera::Follow(player_GO);
   

    Camera::SetCameraFunction(&Camera::FollowTarget);
    Camera::SetCameraTransition([]() -> bool {return Camera::pos.x >= 5967.5f;}, &Camera::Stationary);
    Camera::Bounds = Rect(0,0, 60*tileMap->GetWidth(), 60*tileMap->GetHeight());



    
}

void Stage4::Start(){
    
    LoadAssets();
    StartArray();
   
}

void Stage4::Update(float dt){
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

void Stage4::CollideVectors(std::vector<std::shared_ptr<GameObject>>& alpha, std::vector<std::shared_ptr<GameObject>>& beta)
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

void Stage4::Render(){
    RenderArray();
}

void Stage4::Pause(){
}

void Stage4::Resume(){
}

void Stage4::Collision(){
}


