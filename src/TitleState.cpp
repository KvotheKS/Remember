#include "TitleState.h"
#include "TestState.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "StageMask.h"
#include "StageFox.h"
#include "Game.h"

TitleState::TitleState() : State(){
    GameObject* goTitle = new GameObject();
    Sprite* titleBG = new Sprite(*goTitle, "assets/img/title.jpg");
    goTitle->AddComponent(titleBG);

    goTitle->box.x = 0;
    goTitle->box.y = 0;
    objectArray.emplace_back(goTitle);
}

TitleState::~TitleState(){
    objectArray.clear();
}

void TitleState::LoadAssets(){
    GameObject* goText = new GameObject();

    std::string file = "assets/font/Call me maybe.ttf";
    std::string msg = "Press SPACEBAR to play";
    Text* text = new Text(*goText, file, 35, Text::TextStyle::BLENDED, msg, {0, 100, 255, 255}, 0.35);
    goText->AddComponent(text);

    goText->box.SetCenter(540, 550);
    objectArray.emplace_back(goText);
}

void TitleState::Start(){
    LoadAssets();
    StartArray();
}

void TitleState::Update(float dt){
    InputManager& inManager = InputManager::GetInstance();

    if(inManager.KeyPress(ESCAPE_KEY) || inManager.QuitRequested())
        quitRequested = true;

    if(inManager.KeyPress(SPACE_KEY))
        Game::GetInstance().Push(new StageState());

    if(inManager.KeyPress(ENTER_KEY))
        Game::GetInstance().Push(new TestState());

    if(inManager.KeyPress(W_KEY))
        Game::GetInstance().Push(new StageMask());
    if(inManager.KeyPress(D_KEY))
        Game::GetInstance().Push(new StageFox());
    if(inManager.KeyPress(S_KEY))
        Game::GetInstance().Push(new Stage3());
    if(inManager.KeyPress(A_KEY))
        Game::GetInstance().Push(new Stage2());
    if(inManager.KeyPress(Q_KEY))
        Game::GetInstance().Push(new Stage4());

    UpdateArray(dt);
}

void TitleState::Render(){
    RenderArray();
}

void TitleState::Pause(){
}

void TitleState::Resume(){
    Camera::pos = {0, 0};
}

void TitleState::Collision(){

}