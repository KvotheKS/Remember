#include "EndState.h"

EndState::EndState(){
    std::string fileBg;
    GameObject* goEnd = new GameObject();

    if(GameData::playerVictory){
        fileBg = "assets/img/win.jpg";
        backgroundMusic.Open("assets/audio/endStateWin.ogg");
    }
    else{
        fileBg = "assets/img/lose.jpg";
        backgroundMusic.Open("assets/audio/endStateLose.ogg");
    }
    Sprite* endBG = new Sprite(*goEnd, fileBg);
    goEnd->AddComponent(endBG);

    goEnd->box.x = 0;
    goEnd->box.y = 0;
    objectArray.emplace_back(goEnd);
}

EndState::~EndState(){
    objectArray.clear();
}

void EndState::LoadAssets(){
    GameObject* goText = new GameObject();

    std::string file = "assets/font/Call me maybe.ttf";
    std::string msg = "Press SPACEBAR to play again or ESC to quit";
    Text* text;
    if(GameData::playerVictory){
        text = new Text(*goText, file, 35, Text::TextStyle::BLENDED, msg, {0, 110, 0, 255}, 0.35);
        goText->box.SetCenter(540, 550);
    }
    else{
        text = new Text(*goText, file, 35, Text::TextStyle::BLENDED, msg, {180, 0, 30, 255}, 0.35);
        goText->box.SetCenter(540, 570);
    }
    goText->AddComponent(text);
    objectArray.emplace_back(goText);

    backgroundMusic.Play();
}

void EndState::Start(){
    LoadAssets();
    StartArray();
}

void EndState::Update(float dt){
    InputManager& inManager = InputManager::GetInstance();

    if(inManager.KeyPress(ESCAPE_KEY) || inManager.QuitRequested())
        quitRequested = true;

    if(inManager.KeyPress(SPACE_KEY))
        popRequested = true;

    UpdateArray(dt);
}

void EndState::Render(){
    RenderArray();
}

void EndState::Pause(){
}

void EndState::Resume(){
}

void EndState::Collision(){
    
}