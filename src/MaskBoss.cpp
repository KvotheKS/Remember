#include "MaskBoss.h"
#include "Game.h"

MaskBoss::MaskBoss(GameObject& associated) : GameObject(associated){
    MAXMOVETIME = 5.0f;
    flag = 0;

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

    idx = 0;

    auto stmac = new StateMachine(associated);
    associated.AddComponent(stmac);

    auto anm = new AnimNode("assets/img/Masks/Red.png", 1, 1, Vec2(1,1), false, false);
    anm->SetScaleX(1.5,1.5);
    stmac->AddNode(RED, anm);

    anm = new AnimNode("assets/img/Masks/Yellow.png", 1, 1, Vec2(1,1), false, false);
    anm->SetScaleX(1.5,1.5);
    stmac->AddNode(YELLOW, anm);

    anm = new AnimNode("assets/img/Masks/Green.png", 1, 1, Vec2(1,1), false, false);
    anm->SetScaleX(1.5,1.5);
    stmac->AddNode(GREEN, anm);
    stmac->ChangeState(GREEN);
    t.SetFinish(MAXMOVETIME);
    t.Restart();
}

void MaskBoss::Update(float dt){
    auto& currstate = Game::GetInstance().GetCurrentState();
    auto stmac = (StateMachine*)associated.GetComponent(C_ID::StateMachine);
    Vec2 pos;
    if(t.Update(dt) && flag < 2){
        pos = curves[idx]->GetNewPoint(1.0f);
        GameObject* goSpt = new GameObject();
        associated.box.SetCenter(pos.x, pos.y);
        t.Restart();
        idx = !idx;
        flag++;
        if(flag == 2){
            stmac->ChangeState(Rand::Get() % 3);
            flag = 0;
        }
    }
    else if(flag < 2){
        float x = t.Get() / MAXMOVETIME;
        pos = curves[idx]->GetNewPoint(x);
        associated.box.SetCenter(pos.x, pos.y);
    }
    cout << associated.box.GetCenter() << endl;
}

bool MaskBoss::Is(C_ID type)
{ return type == C_ID::Mask; }