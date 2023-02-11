#include "TestState.h"
#include "FpsPrinter.h"
#include "Game.h"
#include "StateMac.h"
#include "SpriteSheetNode.h"
#include "ActionMachine.h"
#include "Player.h"
#include "Enemy.h"
#include "LionBoss.h"
#include "Gate.h"


//test stage pra mecher em collision

TestState::TestState() : State(){
    backgroundMusic.Open("assets/audio/StageState.ogg");
    tileSet = new TileSet(60, 60, "assets/img/Tileset01.png");
}

TestState::~TestState(){
    delete tileSet;
    for(auto it : GetArrays())
        it->clear();
}

void TestState::LoadAssets(){

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
        goTileMap->depth = 1;
        TileMap* tileMap = new TileMap(*goTileMap, "assets/map/testmap.txt", tileSet);
        goTileMap->AddComponent(tileMap);
        goTileMap->box.x = 0;
        goTileMap->box.y = 0;
    objectArray.emplace_back(goTileMap);

    //BACKGROUD
    GameObject* goBackground = new GameObject();
        goBackground->depth = -1;
        Sprite* bg = new Sprite(*goBackground, "assets/img/stage1/stg01bgl01.png");
        goBackground->AddComponent(bg);

        goBackground->box.x = 0;
        goBackground->box.y = 0;
    cameraFollowerObjectArray.emplace_back(goBackground);
    GameObject* goBackground2 = new GameObject();
        goBackground2->depth = -2;
        bg = new Sprite(*goBackground2, "assets/img/stage1/stg01bgl02.png");
        goBackground2->AddComponent(bg);


        goBackground2->box.x = 0;
        goBackground2->box.y = 0;
    cameraFollowerObjectArray.emplace_back(goBackground2);
    GameObject* goBackground3 = new GameObject();
        goBackground3->depth = -3;
        bg = new Sprite(*goBackground3, "assets/img/stage1/stg01bgl03.png");
        goBackground3->AddComponent(bg);


        goBackground3->box.x = 0;
        goBackground3->box.y = 0;
    cameraFollowerObjectArray.emplace_back(goBackground3);

    GameObject* goBackground4 = new GameObject();
        goBackground4->depth = -4;
        bg = new Sprite(*goBackground4, "assets/img/stage1/stg01bgl04.png");
        goBackground4->AddComponent(bg);


        goBackground4->box.x = 0;
        goBackground4->box.y = 0;
    cameraFollowerObjectArray.emplace_back(goBackground4);

    GameObject* goBackground5 = new GameObject();
        goBackground5->depth = -5;
        bg = new Sprite(*goBackground5, "assets/img/stage1/stg01bgl05.png");
        goBackground4->AddComponent(bg);


        goBackground5->box.x = 0;
        goBackground5->box.y = 0;
    cameraFollowerObjectArray.emplace_back(goBackground5);
    //STAGEART
            
            
    GameObject* goArt1 = new GameObject();
        goArt1->depth = 0;
        bg = new Sprite(*goArt1, "assets/img/stage1/bridge_bg.png");
        goBackground4->AddComponent(bg);


        goArt1->box.x = 60*93;
        goArt1->box.y = 60;

        goArt1->box.h = 60;
        goArt1->box.w = 60*10;

    
    cameraFollowerObjectArray.emplace_back(goArt1);

    
    for(int i = 0;i < 10;i++){
        GameObject* goArt1terrain = new GameObject();
        goArt1terrain->depth = 1;
        Collider* collider = new Collider(*goArt1terrain);
        auto terr = new TerrainBody(*goArt1terrain, false);
        goArt1terrain->box.x = 60*(93+i);
        goArt1terrain->box.y = 60*10 ;

        goArt1terrain->box.h = 60;
        goArt1terrain->box.w = 60;

        goArt1terrain->AddComponents({collider, terr});
        terrainArray.emplace_back(goArt1terrain);
    }
           
        

    
    //FOREGROUND

              
    GameObject* goForeground1 = new GameObject();
        goForeground1->depth = 101;
        bg = new Sprite(*goForeground1, "assets/img/stage1/stg01fgl01.png");
        goForeground1->AddComponent(bg);


        goForeground1->box.x = 60*50;
        goForeground1->box.y = 0;


    
    cameraFollowerObjectArray.emplace_back(goForeground1);

    //OTHER
    GameObject* GO_Gate = new GameObject();

        GO_Gate->AddComponent(new Gate(*GO_Gate));
    terrainArray.emplace_back(GO_Gate);

    GameObject* fpsChecker = new GameObject();
        fpsChecker->depth = 9999;
        fpsChecker->AddComponent(new FpsPrinter(*fpsChecker));
        std::string file = "assets/font/Call me maybe.ttf";
        fpsChecker->AddComponent(new Text(*fpsChecker, file, 35, Text::TextStyle::BLENDED, "0", {0, 100, 255, 255}, 0.35));
        fpsChecker->AddComponent(new CameraFollower(*fpsChecker));
    cameraFollowerObjectArray.emplace_back(fpsChecker);
    
    //PLAYER
    spawnList = {Vec2(2,8),Vec2(90,8)}; 
    GameObject* player_GO = new GameObject();
        player_GO->depth = 10;
        //morte ao primbus

        StateMachine* st = new StateMachine(*player_GO);
        player_GO->AddComponent(st);
        Collider* pl_collider = new Collider(*player_GO);
        player_GO->AddComponent(pl_collider);
        RigidBody* box = new RigidBody(*player_GO);
        player_GO->AddComponent(box);
        Player* pl = new Player(*player_GO);
        player_GO->AddComponent(pl);
        
        
        player_GO->box.SetCenter(spawnList[GameData::spawn_ID].x*60, spawnList[GameData::spawn_ID].y*60);
      
    rigidArray.emplace_back(player_GO);

    GameObject* enemy_GO = new GameObject();
        enemy_GO->depth = 3;
        enemy_GO->AddComponent(new LionBoss(*enemy_GO));
        enemy_GO->box.x = 7151.86 + 48 - enemy_GO->box.w;
        enemy_GO->box.y = 488 + 112 - enemy_GO->box.h;
    rigidArray.emplace_back(enemy_GO);

    backgroundMusic.Play();

    Camera::Follow(player_GO);

    Camera::SetCameraFunction(&Camera::FollowTarget);
    Camera::SetCameraTransition([]() -> bool {return Camera::pos.x >= 5967.5f;}, &Camera::Stationary);
    Camera::Bounds = Rect(0,0, 60*tileMap->GetWidth(), 60*tileMap->GetHeight());


    /*STAGE TERRAIN*/
   
    // first platform

    // for(int i = 0; i<10; i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth = 999;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         terrainbox->box.SetCenter(i*100-1000, 400);
    //         terrainbox->angleDeg = 0;
    //     terrainArray.emplace_back(terrainbox);
    // }

    // west celling

    // for(int i = 0; i<10; i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth = 999;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         terrainbox->box.SetCenter(i*100-1300, 300);
    //         terrainbox->angleDeg = 0;
    //     terrainArray.emplace_back(terrainbox);
    // }

    // diagonal cubes
    // for(int i = 0; i<20; i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth = 999;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         terrainbox->box.SetCenter(i*100+300, 000);
    //         terrainbox->angleDeg = 20*(i+1);
    //     terrainArray.emplace_back(terrainbox);
    // }
   
    // jump platforms
    // for(int i = 0; i<1; i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth = 999;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         terrainbox->box.SetCenter(0, i*300 +300);
    //         terrainbox->angleDeg = 0;
    //     terrainArray.emplace_back(terrainbox);
    // }
    // ramp 
    // Vec2 p1 = Vec2(0,0), p2 = Vec2(100,100), p3 =Vec2(200,200) ,p4 =Vec2(300,100),p5 =Vec2(300,400),p6 =Vec2(200,500),p7 =Vec2(100,300),p8 =Vec2(300,300);
    
    // for(int i = 0; i<(tot=5); i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth = 999;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         float t = i/tot ;
    //         Vec2 temp = Bcurve2(std::vector<Vec2> {Vec2(100,400),Vec2(500,170)},t);

    //         terrainbox->box.SetCenter(temp.x+600, temp.y);
    //         terrainbox->angleDeg = -30;
    //     terrainArray.emplace_back(terrainbox);
    // }
    

    // for(float i = 0;i<tot;i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth = 100;
    //         float t = i/tot ;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         Vec2 temp = Bcurve2(std::vector<Vec2> {p1,p2,p3,p4,p5,p6,p7,p8},t);

    //         terrainbox->box.SetCenter(temp.x, temp.y+200);
    //         terrainbox->angleDeg = 0;
    //     terrainArray.emplace_back(terrainbox);
    // }

    
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
    }
    

    /* ordem de update necessaria
    */
    // std::cout << "ENTRA";
    UpdateArray(dt);
    // std::cout << "SAI";

    CollideVectors(rigidArray, terrainArray);
    CollideVectors(rigidArray, bulletArray);
    CollideVectors(rigidArray, rigidArray);
    CollideVectors(bulletArray, terrainArray);
    // std::cout << "colidiu";
    
    Camera::Update(dt);
    

    KillDeads();
}

void TestState::CollideVectors(std::vector<std::shared_ptr<GameObject>>& alpha, std::vector<std::shared_ptr<GameObject>>& beta)
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
            if(&betaObj == &alphaObj) continue;

            Collider* colliderB = (Collider*) betaObj->GetComponent(C_ID::Collider);
            
            if(!colliderB) continue;
            
            float angleOfB = betaObj->angleDeg * (PI / 180.0);
            
            auto [flag, sep] = Collision::IsColliding(colliderA->box, colliderB->box, angleOfA, angleOfB);
            // std::cout << "\nCOLLISION TESTED";
            if(flag){    
                // if(alphaObj->GetComponent(C_ID::Lion) && betaObj->GetComponent(C_ID::Attack))
                    // std::cout << "KLKKKK";
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

void TestState::Render(){
    RenderArray();
}

void TestState::Pause(){
}

void TestState::Resume(){
}

void TestState::Collision(){
}

Vec2 TestState::Bcurve(Vec2 a ,Vec2 b, Vec2 c, Vec2 d,float t) {
    return Vec2( std::pow((1 - t), 3) * a.x + 3 * std::pow((1 - t), 2) * t * b.x + 3 * std::pow((1 - t), 1) * std::pow(t, 2) * c.x + std::pow(t, 3) * d.x
                ,std::pow((1 - t), 3) * a.y + 3 * std::pow((1 - t), 2) * t * b.y + 3 * std::pow((1 - t), 1) * std::pow(t, 2) * c.y + std::pow(t, 3) * d.y);
}

Vec2 TestState::Bcurve2(std::vector<Vec2> vec, float t) {
    auto fac = [](const int n){
        int ret = n;
        if(n == 0) return 1;
        for(int i = n-1; i>0 ;i--)ret = ret * i;
        return ret;
    };

    int v = vec.size();
    float x = 0,y = 0;
    int n = v -1;
    for (int k = 0; k<v; k++){
        x += fac(n)/(fac(k)*fac(n-k)) * std::pow((1-t),(n-k)) * std::pow((t),(k)) * vec[k].x;
        y += fac(n)/(fac(k)*fac(n-k)) * std::pow((1-t),(n-k)) * std::pow((t),(k)) * vec[k].y;
       
    }

    return Vec2(x,y);

}//!!find better place