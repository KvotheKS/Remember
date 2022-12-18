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


    GameObject* fpsChecker = new GameObject();
        fpsChecker->depth = 9999;
        fpsChecker->AddComponent(new FpsPrinter(*fpsChecker));
        std::string file = "assets/font/Call me maybe.ttf";
        fpsChecker->AddComponent(new Text(*fpsChecker, file, 35, Text::TextStyle::BLENDED, "0", {0, 100, 255, 255}, 0.35));
        fpsChecker->AddComponent(new CameraFollower(*fpsChecker));
    cameraFollowerObjectArray.emplace_back(fpsChecker);
    
    GameObject* player = new GameObject();
        player->depth = 999;
        RigidBody* box = new RigidBody(*player,1);
        player->AddComponent(box);
        player->box.SetCenter(100, 100);
    objectArray.emplace_back(player);

    backgroundMusic.Play();

    Camera::Follow(player);

    /*STAGE TERRAIN*/
    float tot = 50;
    //first platform
    for(int i = 0; i<20; i++){
        GameObject* terrainbox = new GameObject();
            terrainbox->depth = 999;
            TerrainBody* box2 = new TerrainBody(*terrainbox);
            terrainbox->AddComponent(box2);
            terrainbox->box.SetCenter(i*100-1000, 400);
            terrainbox->angleDeg = 0;
        objectArray.emplace_back(terrainbox);
    }
    // west celling
    for(int i = 0; i<10; i++){
        GameObject* terrainbox = new GameObject();
            terrainbox->depth = 999;
            TerrainBody* box2 = new TerrainBody(*terrainbox);
            terrainbox->AddComponent(box2);
            terrainbox->box.SetCenter(i*100-1300, 300);
            terrainbox->angleDeg = 0;
        objectArray.emplace_back(terrainbox);
    }
    // diagonal cubes
    for(int i = 0; i<20; i++){
        GameObject* terrainbox = new GameObject();
            terrainbox->depth = 999;
            TerrainBody* box2 = new TerrainBody(*terrainbox);
            terrainbox->AddComponent(box2);
            terrainbox->box.SetCenter(i*100+300, 000);
            terrainbox->angleDeg = 20*(i+1);
        objectArray.emplace_back(terrainbox);
    }
   
    // jump platforms
    for(int i = 0; i<5; i++){
        GameObject* terrainbox = new GameObject();
            terrainbox->depth = 999;
            TerrainBody* box2 = new TerrainBody(*terrainbox);
            terrainbox->AddComponent(box2);
            terrainbox->box.SetCenter(0, i*300 - 200);
            terrainbox->angleDeg = 0;
        objectArray.emplace_back(terrainbox);
    }
    // ramp 
    Vec2 p1 = Vec2(0,0), p2 = Vec2(100,100), p3 =Vec2(200,200) ,p4 =Vec2(300,100),p5 =Vec2(300,400),p6 =Vec2(200,500),p7 =Vec2(100,300),p8 =Vec2(300,300);
    
    for(int i = 0; i<(tot=5); i++){
        GameObject* terrainbox = new GameObject();
            terrainbox->depth = 999;
            TerrainBody* box2 = new TerrainBody(*terrainbox);
            terrainbox->AddComponent(box2);
            float t = i/tot ;
            Vec2 temp = Bcurve2(std::vector<Vec2> {Vec2(100,400),Vec2(500,170)},t);

            terrainbox->box.SetCenter(temp.x+600, temp.y);
            terrainbox->angleDeg = -30;
        objectArray.emplace_back(terrainbox);
    }
    

    // for(float i = 0;i<tot;i++){
    //     GameObject* terrainbox = new GameObject();
    //         terrainbox->depth = 100;
    //         float t = i/tot ;
    //         TerrainBody* box2 = new TerrainBody(*terrainbox);
    //         terrainbox->AddComponent(box2);
    //         Vec2 temp = Bcurve2(std::vector<Vec2> {p1,p2,p3,p4,p5,p6,p7,p8},t);

    //         terrainbox->box.SetCenter(temp.x, temp.y+200);
    //         terrainbox->angleDeg = 0;
    //     objectArray.emplace_back(terrainbox);
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