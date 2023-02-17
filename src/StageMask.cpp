
#include "FpsPrinter.h"
#include "Game.h"
#include "StateMac.h"
#include "SpriteSheetNode.h"
#include "ActionMachine.h"
#include "Player.h"
#include "Enemy.h"
#include "LionBoss.h"
#include "MaskBoss.h"
#include "StageMask.h"
#include "ScreenFade.h"



StageMask::StageMask() : State(){
    backgroundMusic.Open("assets/audio/StageState.ogg");
    tileSet = new TileSet(60, 60, "assets/img/tilesetcave.png");
}

StageMask::~StageMask(){
    delete tileSet;
    for(auto it : GetArrays())
        it->clear();
}

void StageMask::LoadAssets(){

    //FADE IN EFFECT
    GameObject* GO_fade = new GameObject();
        GO_fade->depth = 1000;
        float fadeinTime = 0; float fadeoutTime = 0.8;float duration = 1;
        ScreenFade* effect = new ScreenFade(*GO_fade, fadeinTime, fadeoutTime, duration);
        
        GO_fade->AddComponent(effect);
    
        State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(GO_fade);

    // TILEMAP
    GameObject* goTileMap = new GameObject();
        goTileMap->depth = 1;
        TileMap* tileMap = new TileMap(*goTileMap, "assets/map/tilemap_stagemask.txt", tileSet);
        goTileMap->AddComponent(tileMap);
        goTileMap->box.x = 0;
        goTileMap->box.y = 0;
    objectArray.emplace_back(goTileMap);

    // BACKGROUND
    GameObject* goBackground = new GameObject();
        goBackground->depth = -15;
        Sprite* bg = new Sprite(*goBackground, "assets/img/StageMask/caverna_bgfundo.png");
        goBackground->AddComponent(bg);
        goBackground->AddComponent(new CameraFollower(*goBackground));
        goBackground->box.x = 60*-5;
        goBackground->box.y = 60*-1;
    cameraFollowerObjectArray.emplace_back(goBackground);
    
    GameObject* goBackground2 = new GameObject();
        goBackground2->depth = -5;
        bg = new Sprite(*goBackground2, "assets/img/StageMask/caverna_bgl01.png");
        goBackground2->AddComponent(bg);


        goBackground2->box.x = 60*-3;
        goBackground2->box.y = 60*-3;
    cameraFollowerObjectArray.emplace_back(goBackground2);


    GameObject* goBackground3 = new GameObject();
        goBackground3->depth = -6;
        bg = new Sprite(*goBackground3, "assets/img/StageMask/caverna_bgl02.png");
        goBackground3->AddComponent(bg);


        goBackground3->box.x = 60*-3;
        goBackground3->box.y = 60*0;
    cameraFollowerObjectArray.emplace_back(goBackground3);

     GameObject* goBackground4 = new GameObject();
        goBackground4->depth = -7;
        bg = new Sprite(*goBackground4, "assets/img/StageMask/caverna_bgl03.png");
        goBackground4->AddComponent(bg);


        goBackground4->box.x = 60*-3;
        goBackground4->box.y = 60*-1;
    cameraFollowerObjectArray.emplace_back(goBackground4);

    GameObject* goBackground5 = new GameObject();
        goBackground5->depth = -8;
        bg = new Sprite(*goBackground5, "assets/img/StageMask/caverna_bgl04.png");
        goBackground2->AddComponent(bg);


        goBackground5->box.x = 60*-3.5;
        goBackground5->box.y = 60*-5;
    cameraFollowerObjectArray.emplace_back(goBackground5);

    //STAGEART
    
    //FOREGROUND


    GameObject* fpsChecker = new GameObject();
        fpsChecker->depth = 9999;
        fpsChecker->AddComponent(new FpsPrinter(*fpsChecker));
        std::string file = "assets/font/Call me maybe.ttf";
        fpsChecker->AddComponent(new Text(*fpsChecker, file, 35, Text::TextStyle::BLENDED, "0", {0, 100, 255, 255}, 0.35));
        fpsChecker->AddComponent(new CameraFollower(*fpsChecker));
    cameraFollowerObjectArray.emplace_back(fpsChecker);
    
    //PLAYER 
    
    spawnList ={Vec2(1,7)};
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

    GameObject* enemy_GO = new GameObject();
        enemy_GO->depth = 3;
        enemy_GO->AddComponent(new MaskBoss(*enemy_GO));
        enemy_GO->box.SetCenter(900, 330);
    rigidArray.emplace_back(enemy_GO);
    

    backgroundMusic.Play();

    Camera::Follow(player_GO);

    Camera::SetCameraFunction(&Camera::FollowTarget);
    Camera::SetCameraTransition([]() -> bool {return Camera::pos.x >= 5967.5f;}, &Camera::Stationary);
    Camera::Bounds = Rect(0,0, 60*tileMap->GetWidth(), 60*tileMap->GetHeight());



    
}

void StageMask::Start(){
    
    LoadAssets();
    StartArray();
   
}

void StageMask::Update(float dt){
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

void StageMask::CollideVectors(std::vector<std::shared_ptr<GameObject>>& alpha, std::vector<std::shared_ptr<GameObject>>& beta)
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

void StageMask::Render(){
    RenderArray();
}

void StageMask::Pause(){
}

void StageMask::Resume(){
}

void StageMask::Collision(){
}


