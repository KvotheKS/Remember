#include "MenuState.h"
#include "TestState.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "StageMask.h"
#include "StageFox.h"
#include "Game.h"
#include "MovementBehavior.h"
#include "ScreenFade.h"
#include "TutorialState.h"
#include "CreditsState.h"

MenuState::MenuState() : State(){
    GameObject* goTitle = new GameObject();
    Sprite* titleBG = new Sprite(*goTitle, "assets/img/menuimage.png");
    goTitle->AddComponent(titleBG);
    titleBG->SetScaleX(1,1);
    menu_pos = 0;

    goTitle->box.x = 0;
    goTitle->box.y = 0;
    objectArray.emplace_back(goTitle);

    
}

MenuState::~MenuState(){
    objectArray.clear();
}

void MenuState::LoadAssets(){
      
    
    //FADE IN EFFECT
    GameObject* GO_fade = new GameObject();
        GO_fade->depth = 1000;
        float fadeinTime = 0; float fadeoutTime = 0.8;float duration = 1;
        ScreenFade* effect = new ScreenFade(*GO_fade, fadeinTime, fadeoutTime, duration);
        
        GO_fade->AddComponent(effect);
    
        State& state = Game::GetInstance().GetCurrentState();
    state.AddObject(GO_fade);

    int mx_pos = 540; int my_pos = 550;
    int menu_sep = 45;
    int shadow_dist = 4;
    int menu_font_size = 25;


     GameObject* GO_title = new GameObject();
        GO_title->depth = 3;
        std::string file = "assets/font/OneBrush.ttf";
        std::string msg = "REMEMBER";
        
        Text * text = new Text(*GO_title, file, 150, Text::TextStyle::BLENDED, msg, {138, 49, 199,255}, 0.35);
        GO_title->AddComponent(text);

        GO_title->box.SetCenter(620, 100);
    objectArray.emplace_back(GO_title);

    GameObject* GO_title1 = new GameObject();
        GO_title1->depth = 1;
        file = "assets/font/OneBrush.ttf";
        msg = "REMEMBER";
        text = new Text(*GO_title1, file, 150, Text::TextStyle::BLENDED, msg, {69, 25, 100,90}, 0.35);
        GO_title1->AddComponent(text);
        MovementBehavior* mb2 = new MovementBehavior(*GO_title1,2,1,0);
        GO_title->AddComponent(mb2);
        GO_title1->box.SetCenter(615, 100);
    objectArray.emplace_back(GO_title1);

    GameObject* GO_title2 = new GameObject();
        GO_title2->depth = 1;
        file = "assets/font/OneBrush.ttf";
        msg = "REMEMBER";
        text = new Text(*GO_title2, file, 150, Text::TextStyle::BLENDED, msg, {69, 25, 100,90}, 0.35);
        GO_title->AddComponent(text);
        mb2 = new MovementBehavior(*GO_title2,2,1,0);
        GO_title->AddComponent(mb2);
        GO_title2->box.SetCenter(615, 100);
    objectArray.emplace_back(GO_title2);

    GameObject* GO_title3 = new GameObject();
        GO_title3->depth = 1;
        file = "assets/font/OneBrush.ttf";
        msg = "REMEMBER";
        text = new Text(*GO_title3, file, 150, Text::TextStyle::BLENDED, msg, {69, 25, 100,90}, 0.35);
        GO_title3->AddComponent(text);
        mb2 = new MovementBehavior(*GO_title3,2,1,0);
        GO_title->AddComponent(mb2);
        GO_title3->box.SetCenter(620, 95);
    objectArray.emplace_back(GO_title3);

    GameObject* GO_title4 = new GameObject();
        GO_title4->depth = 1;
        file = "assets/font/OneBrush.ttf";
        msg = "REMEMBER";
        text = new Text(*GO_title4, file, 150, Text::TextStyle::BLENDED, msg, {69, 25, 100,90}, 0.35);
        GO_title4->AddComponent(text);
        mb2 = new MovementBehavior(*GO_title4,2,1,0);
        GO_title->AddComponent(mb2);
        GO_title4->box.SetCenter(620, 105);
    objectArray.emplace_back(GO_title4);

    GameObject* goText = new GameObject();
        goText->depth = 2;
        file = "assets/font/Game of Brush.ttf";
        msg = "START GAME";
        op0 = new Text(*goText, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 193, 226, 255}, 0.35);
        goText->AddComponent(op0);

        goText->box.SetCenter(mx_pos, my_pos+menu_sep*0);
    objectArray.emplace_back(goText);
    GameObject* goText2 = new GameObject();
        goText->depth = 1;
        file = "assets/font/Game of Brush.ttf";
        msg = "START GAME";
        text = new Text(*goText2, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 12, 123, 123}, 0.35);
        goText2->AddComponent(text);

        goText2->box.SetCenter(mx_pos, my_pos+menu_sep*0+shadow_dist);
    objectArray.emplace_back(goText2);

    goText = new GameObject();
        goText->depth = 2;
        file = "assets/font/Game of Brush.ttf";
        msg = "CONTROLS";
        op1 = new Text(*goText, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 193, 226, 255}, 0.35);
        goText->AddComponent(op1);

        goText->box.SetCenter(mx_pos, my_pos+menu_sep*1);
    objectArray.emplace_back(goText);
    goText2 = new GameObject();
        goText->depth = 1;
        file = "assets/font/Game of Brush.ttf";
        msg = "CONTROLS";
        text = new Text(*goText2, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 12, 123, 123}, 0.35);
        goText2->AddComponent(text);

        goText2->box.SetCenter(mx_pos, my_pos+menu_sep*1+shadow_dist);
    objectArray.emplace_back(goText2);

    goText = new GameObject();
        goText->depth = 2;
        file = "assets/font/Game of Brush.ttf";
        msg = "CREDITS";
        op2 = new Text(*goText, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 193, 226, 255}, 0.35);
        goText->AddComponent(op2);

        goText->box.SetCenter(mx_pos, my_pos+menu_sep*2);
    objectArray.emplace_back(goText);
    goText2 = new GameObject();
        goText->depth = 1;
        file = "assets/font/Game of Brush.ttf";
        msg = "CREDITS";
        text = new Text(*goText2, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 12, 123, 123}, 0.35);
        goText2->AddComponent(text);

        goText2->box.SetCenter(mx_pos,my_pos +menu_sep*2+shadow_dist);
    objectArray.emplace_back(goText2);

    goText = new GameObject();
        goText->depth = 2;
        file = "assets/font/Game of Brush.ttf";
        msg = "QUIT";
        op3 = new Text(*goText, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 193, 226, 255}, 0.35);
        goText->AddComponent(op3);

        goText->box.SetCenter(mx_pos, my_pos+menu_sep*3);
    objectArray.emplace_back(goText);
    goText2 = new GameObject();
        goText->depth = 1;
        file = "assets/font/Game of Brush.ttf";
        msg = "QUIT";
        text = new Text(*goText2, file, menu_font_size, Text::TextStyle::BLENDED, msg, {0, 12, 123, 123}, 0.35);
        goText2->AddComponent(text);

        goText2->box.SetCenter(mx_pos,my_pos +menu_sep*3+shadow_dist);
    objectArray.emplace_back(goText2);


    GO_cursor = new GameObject();
        GO_cursor->depth = 0;
        Sprite* icon = new Sprite(*GO_cursor, "assets/img/menu_icon.png");
        GO_cursor->AddComponent(icon);
        icon->SetScaleX(2,2);
        
        MovementBehavior* mb = new MovementBehavior(*GO_cursor,30,1,0);
        MovementBehavior* mb1 = new MovementBehavior(*GO_cursor,30,1,1);
        GO_cursor->AddComponents({mb,mb1});
        GO_cursor->box.x = 300;
    cameraFollowerObjectArray.emplace_back(GO_cursor);
}

void MenuState::Start(){
    LoadAssets();
    StartArray();
}

void MenuState::Update(float dt){
   

    Menu(dt);
    InputManager& inManager = InputManager::GetInstance();

   

    if(inManager.KeyPress(ESCAPE_KEY) || inManager.QuitRequested())
        quitRequested = true;

    

    if(inManager.KeyPress(ENTER_KEY)){
        switch(menu_pos){
            case 0:
                Game::GetInstance().Push(new TestState());
                
            break;
            case 1:
                
                Game::GetInstance().Push(new TutorialState());
                
            break;
            case 2:
                Game::GetInstance().Push(new CreditsState());
                break;
            case 3:
                quitRequested = true;
            break;
        }
    }
       

    if(inManager.KeyPress(UP_ARROW_KEY)){
        menu_pos--;
        if(menu_pos < 0){
            menu_pos = 3;
        }
    }
    if(inManager.KeyPress(DOWN_ARROW_KEY)){
        menu_pos++;
        if(menu_pos > 3){
            menu_pos = 0;
        }
    }

    if(inManager.IsKeyDown(SPACE_KEY)){
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
            
    }
   
    UpdateArray(dt);

    KillDeads();
}

void MenuState::Render(){
    RenderArray();
}

void MenuState::Pause(){
}

void MenuState::Resume(){
    Camera::pos = {0, 0};
}

void MenuState::Menu(float dt){
    int l_pos = 530;
    SDL_Color normal = {0, 193, 226, 255};
    SDL_Color selected = {201, 92, 231,255};
    switch(menu_pos){
        case 0:
            GO_cursor->box.y = l_pos + 45*0;
            op0->SetColor(selected);
            op1->SetColor(normal);
            op2->SetColor(normal);
            op3->SetColor(normal);
        break;
        case 1:
            GO_cursor->box.y = l_pos + 45*1;
            op0->SetColor(normal);
            op1->SetColor(selected);
            op2->SetColor(normal);
            op3->SetColor(normal);
        break;
        case 2:
            GO_cursor->box.y = l_pos + 45*2;
            op0->SetColor(normal);
            op1->SetColor(normal);
            op2->SetColor(selected);
            op3->SetColor(normal);
        break;
        case 3:
            GO_cursor->box.y = l_pos + 45*3;
            op0->SetColor(normal);
            op1->SetColor(normal);
            op2->SetColor(normal);
            op3->SetColor(selected);
        break;
    }
}

void MenuState::Collision(){

}