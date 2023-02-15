#include "MaskBoss.h"
#include "Game.h"

MaskBoss::MaskBoss(GameObject& associated) : GameObject(associated){
    CURR_STATE = INACTIVED;
    CURR_PHASE = PHASE1;
    CURR_MASK = Rand::Get() % 3;

    MAX_HP = 500;
    hp = MAX_HP;

    MOVETIME = 4.5f;
    MOVELOOP = -1;
    CURVEIDX = 0;

    RESTTIME = 2.0f;

    SWAPTIME = 0.5f;
    SWAPED = false;

    Vec2 p1 = Vec2(947, 346);
    Vec2 p2 = Vec2(-106, 646);
    Vec2 p3 = Vec2(-106, 46);
    Vec2 p4 = Vec2(947, 346);
    std::vector<Vec2> points1 = {p1, p2, p3, p4};
    curves.push_back(new Bcurve(points1));

    Vec2 p5 = Vec2(1856, 646);
    Vec2 p6 = Vec2(1856, 46);
    std::vector<Vec2> points2 = {p1, p5, p6, p4};
    curves.push_back(new Bcurve(points2));

    auto stmac = new StateMachine(associated);
    associated.AddComponent(stmac);

    int mask[] = {GREEN, RED, YELLOW, GREEN+PHASE2, RED+PHASE2, YELLOW+PHASE2};
    std::vector<string> fileName {"Green", "Red", "Yellow", "GreenBroken", "RedBroken", "YellowBroken"};
    string path = "assets/img/Masks/", ext = ".png";
    for(int i = 0; i < 6; i++){
        auto anm = new AnimNode(path + fileName[i] + ext, 1, 1);
        anm->SetScaleX(1.5, 1.5);
        stmac->AddNode(mask[i], anm);

        anm = new AnimNode(path + fileName[i] + "D" + ext, 1, 1);
        anm->SetScaleX(1.5, 1.5);
        stmac->AddNode(mask[i] + DARK, anm);
    }

    stmac->ChangeState(CURR_MASK + DARK);
    timer.Restart();
}

void MaskBoss::Activate(){
    auto& currstate = Game::GetInstance().GetCurrentState();
    auto target = currstate.GetObject(C_ID::Player, &currstate.rigidArray);
    if(target.lock().get()->box.GetCenter().Distance(associated.box.GetCenter()) < 800){
        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK);
        CURR_STATE = MOVING;
    }
}

void MaskBoss::Moving(float dt){
    Vec2 pos;
    if(MOVELOOP == -1){
        MOVELOOP = 0;
        CURVEIDX = Rand::Get() % 2;
        timer.SetFinish(MOVETIME);
        timer.Restart();
    }

    if(timer.Update(dt) && MOVELOOP < 2){
        pos = curves[CURVEIDX]->GetNewPoint(1.0f);
        associated.box.SetCenter(pos.x, pos.y);
        CURVEIDX = !CURVEIDX;
        MOVELOOP++;
        timer.Restart();
    }
    else if(MOVELOOP < 2){
        float t = timer.Get() / MOVETIME;
        pos = curves[CURVEIDX]->GetNewPoint(t);
        associated.box.SetCenter(pos.x, pos.y);
    }
    else{
        MOVELOOP = -1;

        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK + CURR_PHASE + DARK);
        CURR_STATE = IDLE;

        timer.SetFinish(RESTTIME);
        timer.Restart();
    }
}

void MaskBoss::Resting(float dt){
    if(CURR_PHASE == PHASE1 && hp < MAX_HP/2)
        CURR_PHASE = PHASE2;
    if(timer.Update(dt)){
        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK + CURR_PHASE);
        CURR_STATE = SWAPPING;
    }
}

void MaskBoss::Swapping(float dt){
    if(!SWAPED){
        vector<int> swapMask;
        if(CURR_MASK == RED)
            swapMask.assign({GREEN, YELLOW});
        else if(CURR_MASK == GREEN)
            swapMask.assign({RED, YELLOW});
        else
            swapMask.assign({GREEN, RED});

        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        CURR_MASK = swapMask[Rand::Get() % 2];
        stmac->ChangeState(CURR_MASK + CURR_PHASE);

        timer.SetFinish(SWAPTIME);
        timer.Restart();
        SWAPED = true;
    }
    if(timer.Update(dt)){
        CURR_STATE = MOVING;
        SWAPED = false;
    }
}

void MaskBoss::Update(float dt){
    switch(CURR_STATE){
        case INACTIVED:
            Activate();
            break;
        case MOVING:
            Moving(dt);
            break;
        case IDLE:
            Resting(dt);
            break;
        case CHARGING:
            break;
        case ATKING:
            break;
        case SWAPPING:
            Swapping(dt);
            break;
        default:
            break;
    }
}

bool MaskBoss::Is(C_ID type){
    return type == C_ID::Mask;
}

void MaskBoss::TakeDamage(int damage){
    if((hp -= damage) < 0) hp = 0;
}