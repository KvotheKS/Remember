#include "MaskBoss.h"
#include "Game.h"

MaskBoss::MaskBoss(GameObject& associated) : GameObject(associated){
    CURR_STATE = INACTIVED;
    CURR_PHASE = PHASE1;
    CURR_MASK = Rand::Get() % 3;

    MAX_HEALTH = 500;
    currentHealth = MAX_HEALTH;

    MOVE_TIME = 4.5f;
    MOVE_LOOP = -1;
    CURVE_IDX = 0;

    REST_TIME = 2.0f;

    SWAP_TIME = 0.5f;
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
    if(MOVE_LOOP == -1){
        MOVE_LOOP = 0;
        CURVE_IDX = Rand::Get() % 2;
        timer.SetFinish(MOVE_TIME);
        timer.Restart();
    }

    if(timer.Update(dt) && MOVE_LOOP < 2){
        pos = curves[CURVE_IDX]->GetNewPoint(1.0f);
        associated.box.SetCenter(pos.x, pos.y);
        CURVE_IDX = !CURVE_IDX;
        MOVE_LOOP++;
        timer.Restart();
    }
    else if(MOVE_LOOP < 2){
        float t = timer.Get() / MOVE_TIME;
        pos = curves[CURVE_IDX]->GetNewPoint(t);
        associated.box.SetCenter(pos.x, pos.y);
    }
    else{
        MOVE_LOOP = -1;

        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK + CURR_PHASE + DARK);
        CURR_STATE = IDLE;

        timer.SetFinish(REST_TIME);
        timer.Restart();
    }
}

void MaskBoss::Resting(float dt){
    if(CURR_PHASE == PHASE1 && currentHealth < MAX_HEALTH/2)
        CURR_PHASE = PHASE2;
    if(timer.Update(dt)){
        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK + CURR_PHASE);
        CURR_STATE = ATTAKING;
    }
}

void MaskBoss::Attacking(){
    // auto& st = Game::GetInstance().GetCurrentState();
    // auto goSpike = new GameObject();
    // goSpike->depth = 3;
    // auto spike = new Spike(*goSpike, Spike::SMALL, 1.0f, 3.0f, 660);
    // goSpike->AddComponent(spike);
    // goSpike->box.y = 660 - goSpike->box.h;
    // goSpike->box.x = 600;
    // st.objectArray.emplace_back(goSpike);
    // CURR_STATE = CHARGING;
    auto& st = Game::GetInstance().GetCurrentState();
    auto goFlames = new GameObject();
    goFlames->depth = -3;
    auto flameSpike = new FlameSpike(*goFlames, st.GetObject(C_ID::Mask), 2.0f * MOVE_TIME);
    goFlames->AddComponents({flameSpike});
    st.objectArray.emplace_back(goFlames);
    CURR_STATE = MOVING;
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

        timer.SetFinish(SWAP_TIME);
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
        case ATTAKING:
            Attacking();
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
    if((currentHealth -= damage) < 0) currentHealth = 0;
}