#include "TutorialState.h"
#include "TestState.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "StageMask.h"
#include "StageFox.h"
#include "Game.h"
#include "MovementBehavior.h"
#include "ScreenFade.h"

TutorialState::TutorialState() : State(){

    
    GameObject* goTitle = new GameObject();
    Sprite* titleBG = new Sprite(*goTitle, "assets/img/TutorialState.png");
    goTitle->AddComponent(titleBG);
    titleBG->SetScaleX(1,1);
    menu_pos = 0;

    goTitle->box.x = 0;
    goTitle->box.y = 0;
    objectArray.emplace_back(goTitle);

    
}

TutorialState::~TutorialState(){
    objectArray.clear();
}

void TutorialState::LoadAssets(){
      
    
    //FADE IN EFFECT
    GameObject* GO_fade = new GameObject();
        GO_fade->depth = 1000;
        float fadeinTime = 0; float fadeoutTime = 0.8;float duration = 1;
        ScreenFade* effect = new ScreenFade(*GO_fade, fadeinTime, fadeoutTime, duration);
        
        GO_fade->AddComponent(effect);
    
        State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(GO_fade);

    int mx_pos = 540; int my_pos = 530;
    int menu_sep = 45;
    int shadow_dist = 4;
    int menu_font_size = 25;


    
    GameObject* goText = new GameObject();
        goText->depth = 2;
        std::string file = "assets/font/Game of Brush.ttf";
        std::string msg = "PRESS ENTER TO RETURN";
        Text *text = new Text(*goText, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 193, 226, 255}, 0.35);
        goText->AddComponent(text);

        goText->box.SetCenter(mx_pos, my_pos+menu_sep*2);
    objectArray.emplace_back(goText);
    GameObject* goText2 = new GameObject();
        goText->depth = 1;
        file = "assets/font/Game of Brush.ttf";
        msg = "PRESS ENTER TO RETURN";
         text = new Text(*goText2, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 12, 123, 123}, 0.35);
        goText2->AddComponent(text);

        goText2->box.SetCenter(mx_pos, my_pos+menu_sep*2+shadow_dist);
    objectArray.emplace_back(goText2);

    GameObject* goText3 = new GameObject();
        goText3->depth = 2;
        file = "assets/font/Game of Brush.ttf";
        msg = "USE A S D AND ARROWS TO CONTROL";
        text = new Text(*goText3, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 193, 226, 255}, 0.35);
        goText3->AddComponent(text);

        goText3->box.SetCenter(mx_pos, my_pos+menu_sep*0);
    objectArray.emplace_back(goText3);
    GameObject* goText4 = new GameObject();
        goText4->depth = 1;
        file = "assets/font/Game of Brush.ttf";
        msg = "USE A S D AND ARROWS TO CONTROL";
        text = new Text(*goText4, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 12, 123, 123}, 0.35);
        goText4->AddComponent(text);

        goText4->box.SetCenter(mx_pos, my_pos+menu_sep*0+shadow_dist);
    objectArray.emplace_back(goText4);

}

void TutorialState::Start(){
    LoadAssets();
    StartArray();
}

void TutorialState::Update(float dt){
   

    // Menu(dt);
    InputManager& inManager = InputManager::GetInstance();

   

    if( inManager.QuitRequested())
        quitRequested = true;

    

    if(inManager.KeyPress(ENTER_KEY)||inManager.KeyPress(ESCAPE_KEY) ){
        popRequested = true;
    }
       

        
    UpdateArray(dt);

    KillDeads();
}

void TutorialState::Render(){
    RenderArray();
}

void TutorialState::Pause(){
}

void TutorialState::Resume(){
    Camera::pos = {0, 0};
}

void TutorialState::Menu(float dt){
    int l_pos = 530;
    SDL_Color normal = {0, 193, 226, 255};
    SDL_Color selected = {0, 201, 92, 231};
    switch(menu_pos){
        case 0:
            GO_cursor->box.y = l_pos + 45*0;
            op0->SetColor(selected);
            op1->SetColor(normal);
            op2->SetColor(normal);
        break;
        case 1:
            GO_cursor->box.y = l_pos + 45*1;
            op0->SetColor(normal);
            op1->SetColor(selected);
            op2->SetColor(normal);
        break;
        case 2:
            GO_cursor->box.y = l_pos + 45*2;
            op0->SetColor(normal);
            op1->SetColor(normal);
            op2->SetColor(selected);
        break;
    }
}

void TutorialState::Collision(){

}