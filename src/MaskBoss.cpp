#include "MaskBoss.h"
#include "Game.h"
#include "GameData.h"

MaskBoss::MaskBoss(GameObject& associated) : GameObject(associated){
    CURR_STATE = INACTIVED;
    CURR_PHASE = PHASE1;
    CURR_MASK = Rand::Get() % 3;
    CURR_ATK = Rand::Get() % 2;

    MAX_HEALTH = 500;
    currentHealth = MAX_HEALTH;

    MOVE_TIME = 4.5f;
    MOVE_LOOP = -1;
    CURVE_IDX = 0;

    REST_TIME = 1.5f;

    CHARGE_TIME = 1.5f;
    CHARGED = false;

    SWAP_TIME = 1.5f;

    DOUBLE_HIT = false;

    SPIKE_NUM = (int) Rand::FloatRange(4.0f, 7.0f);
    SPIKE_DAMAGE = 8;

    FLAMES_DAMAGE = 4;

    SHOOTING_TIME = 1.5f;
    SHOOTING_COOLDOWN = 1.0f;
    SHOOT_NUM = 24;
    SHOOT_DAMAGE = 6;
    SHOOT_CHARGE = false;

    LASERING_TIME = 1.0f;
    LASERING_COOLDOWN = 1.0f;
    LASER_DAMAGE = 16;

    RAINING_TIME = 2.5f;
    RAIN_DELAY = 0.265f;
    RAIN_DAMAGE = 6;

    Vec2 p1 = Vec2(900, 330);
    Vec2 p2 = Vec2(0, 630);
    Vec2 p3 = Vec2(0, 30);
    Vec2 p4 = Vec2(900, 330);
    std::vector<Vec2> points1 = {p1, p2, p3, p4};
    curves.push_back(new Bcurve(points1));

    Vec2 p5 = Vec2(1800, 630);
    Vec2 p6 = Vec2(1800, 30);
    std::vector<Vec2> points2 = {p1, p5, p6, p4};
    curves.push_back(new Bcurve(points2));

    auto stmac = new StateMachine(associated);
    auto collider = new Collider(associated);
    collider->SetScale(Vec2(0.5, 0.7));
    collider->SetOffset(Vec2(0, 30));
    associated.AddComponents({stmac, collider});

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
    auto& st = Game::GetInstance().GetCurrentState();
    auto target = st.GetObject(C_ID::Player, &st.rigidArray);
    if(target.lock().get()->box.GetCenter().Distance(associated.box.GetCenter()) < 500){
        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK);
        CURR_STATE = CHARGING;
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

        if(CURR_PHASE == PHASE1 && currentHealth < MAX_HEALTH/2){
            CURR_PHASE = PHASE2;
            auto& st = Game::GetInstance().GetCurrentState();
            auto goBreaking = new GameObject();
                goBreaking->depth = 5;
                auto breakingSpt = new Sprite(*goBreaking, "assets/img/Masks/breaking.png", 10, 0.15, 1.5f);
                auto breakingDisp = new DisappearOnDeadOwner(*goBreaking, st.GetObject(C_ID::Mask, &st.enemyArray));
                goBreaking->AddComponents({breakingSpt, breakingDisp});
                goBreaking->box.SetCenter(903, 330);
            st.objectArray.emplace_back(goBreaking);
        }

        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK + CURR_PHASE + DARK);
        CURR_STATE = IDLE;

        timer.SetFinish(REST_TIME);
        timer.Restart();
    }
}

void MaskBoss::Resting(float dt){
    if(timer.Update(dt)){
        auto& st = Game::GetInstance().GetCurrentState();
        auto goChanging = new GameObject();
            goChanging->depth = 5;
            auto changingSpt = new Sprite(*goChanging, "assets/img/Masks/changing.png", 15, 0.1, 1.5f);
            auto changingDisp = new DisappearOnDeadOwner(*goChanging, st.GetObject(C_ID::Mask, &st.enemyArray));
            goChanging->AddComponents({changingSpt, changingDisp});
            goChanging->box.SetCenter(903, 330);
        st.objectArray.emplace_back(goChanging);

        timer.SetFinish(SWAP_TIME + 0.2f);
        timer.Restart();

        auto stmac = (StateMachine*) associated.GetComponent(C_ID::StateMachine);
        stmac->ChangeState(CURR_MASK + CURR_PHASE);
        CURR_STATE = SWAPPING;
    }
}

void MaskBoss::Charging(float dt){
    if(!CHARGED){
        auto& st = Game::GetInstance().GetCurrentState();
        auto goWarning = new GameObject();
            goWarning->depth = 5;
            auto warningSpt = new Sprite(*goWarning, "assets/img/Masks/warning_circle.png", 20, 0.075, 1.5f);
            auto warningDisp = new DisappearOnDeadOwner(*goWarning, st.GetObject(C_ID::Mask, &st.enemyArray));
            goWarning->AddComponents({warningSpt, warningDisp});
            goWarning->box.SetCenter(903, 280);
        st.objectArray.emplace_back(goWarning);

        timer.SetFinish(CHARGE_TIME);
        timer.Restart();
        CHARGED = true;
    }
    else if(timer.Update(dt)){
        CURR_ATK = Rand::Get() % 2;
        CURR_STATE = ATTAKING;
        CHARGED = false;
    }
}

void MaskBoss::Spiking(int spikeSize, int spikeNum){
    vector<int> position;
    vector<bool> occupedPos(30, false);
    int count = 0;

    while(count < spikeNum){
        if(spikeSize == Spike::SMALL){
            int div = Rand::IntRange(0, 1680) / 60;
            if(!occupedPos[div] && !occupedPos[div + 1]){
                occupedPos[div] = true;
                occupedPos[div+1] = true;
                position.push_back(div * 60);
                count++;
            }
        }
        else{
            int div = Rand::IntRange(0, 1740) / 60;
            if(!occupedPos[div]){
                occupedPos[div] = true;
                position.push_back(div * 60);
                count++;
            }
        }
    }

    auto& st = Game::GetInstance().GetCurrentState();
    for(int i = 0; i < spikeNum; i++){
        auto goSpike = new GameObject();
            goSpike->depth = 3;
            Spike* spike;
            if(spikeSize == Spike::SMALL)
                spike = new Spike(*goSpike, spikeSize, 1.0f, 6.0f, 660);
            else
                spike = new Spike(*goSpike, spikeSize, 1.0f, 3.0f, 660);
            auto spikeAtk = new Attack(*goSpike, SPIKE_DAMAGE, 1, &associated);
            auto spikeDisp = new DisappearOnDeadOwner(*goSpike, st.GetObject(C_ID::Mask, &st.enemyArray));
            goSpike->AddComponents({spike, spikeAtk, spikeDisp});
            goSpike->box.y = 660 - goSpike->box.h;
            goSpike->box.x = position[i];
        st.bulletArray.emplace_back(goSpike);
    }
    timer.Restart();
    timer.SetFinish(0.2f);
    CURR_STATE = COOLING;
}

void MaskBoss::Flaming(){
    auto& st = Game::GetInstance().GetCurrentState();
    auto goFlames = new GameObject();
        goFlames->depth = -3;
        auto flameSpike = new FlameSpike(*goFlames, st.GetObject(C_ID::Mask), 2.0f * MOVE_TIME);
        auto flamesAtk = new Attack(*goFlames, FLAMES_DAMAGE, 1, &associated);
        auto flamesDisp = new DisappearOnDeadOwner(*goFlames, st.GetObject(C_ID::Mask, &st.enemyArray));
        goFlames->AddComponents({flameSpike, flamesAtk, flamesDisp});
    st.bulletArray.emplace_back(goFlames);
    timer.Restart();
    timer.SetFinish(0.2f);
    CURR_STATE = COOLING;
}

void MaskBoss::Shooting(float dt){
    auto& st = Game::GetInstance().GetCurrentState();
    if(!SHOOT_CHARGE){
        auto goChargeShoot = new GameObject();
            goChargeShoot->depth = 5;
            auto chargeShoot = new Sprite(*goChargeShoot, "assets/img/Mask_Atks/projectile_birth.png", 5, 0.2f, 1.0f);
            auto chargeDisp = new DisappearOnDeadOwner(*goChargeShoot, st.GetObject(C_ID::Mask, &st.enemyArray));
            chargeShoot->SetScaleX(0.5, 0.5);
            goChargeShoot->AddComponents({chargeShoot, chargeDisp});
            goChargeShoot->box.SetCenter(903, 280);
        st.objectArray.emplace_back(goChargeShoot);

        timer.Restart();
        timer.SetFinish(1.0f);

        SHOOT_CHARGE = true;
    }
    else if(timer.Update(dt)){
        float angle = 360.0f / (float) SHOOT_NUM;
        for(int i = 1; i <= SHOOT_NUM; i++){
            auto goShootProj = new GameObject();
                goShootProj->depth = 5;
                auto spt = new Sprite(*goShootProj, "assets/img/Mask_Atks/projectile.png");
                spt->SetScaleX(0.5, 0.5);
                auto proj = new Projectile(*goShootProj, 5.0f, angle*i, 250, 250);
                auto projAtk = new Attack(*goShootProj, SHOOT_DAMAGE, 1, &associated);
                auto projDsp = new DisappearOnHit(*goShootProj, &associated);
                auto projDspo = new DisappearOnDeadOwner(*goShootProj, st.GetObject(C_ID::Mask, &st.enemyArray));
                goShootProj->AddComponents({spt, proj, projAtk, projDsp, projDspo});
                goShootProj->box.SetCenter(903, 280);
            st.bulletArray.emplace_back(goShootProj);
        }
        timer.Restart();
        timer.SetFinish(SHOOTING_COOLDOWN + 0.2f);

        SHOOT_CHARGE = false;
        CURR_STATE = COOLING;
    }
}

void MaskBoss::Lasering(float dt){
    auto& st = Game::GetInstance().GetCurrentState();
    auto goLaser = new GameObject();
        goLaser->depth = 12;
        auto laser = new Laser(*goLaser, LASERING_TIME);
        goLaser->box.SetCenter(903, 280);
        auto laserAtk = new Attack(*goLaser, LASER_DAMAGE, 1, &associated);
        auto laserDspo = new DisappearOnDeadOwner(*goLaser, st.GetObject(C_ID::Mask, &st.enemyArray));
        goLaser->AddComponents({laser, laserAtk, laserDspo});
    st.bulletArray.emplace_back(goLaser);

    timer.Restart();
    timer.SetFinish(LASERING_TIME + LASERING_COOLDOWN + 0.2f);
    CURR_STATE = COOLING;
}

void MaskBoss::Raining(){
    auto& st = Game::GetInstance().GetCurrentState();

    Vec2 p1 = Vec2(900, -60);
    for(int i = 0; i < 12; i++){
        Vec2 p2a = Vec2((12 - (i + 1)) * 60 + 30, -720);
        Vec2 p3a = Vec2((12 - (i + 1)) * 60 + 30, 720);
        std::vector<Vec2> points = {p1, p2a, p3a};
        auto goProj = new GameObject();
            goProj->depth = -1;
            auto spt = new Sprite(*goProj, "assets/img/Mask_Atks/fire_rain.png", 4, 0.1);
            auto proj = new ProjectileB(*goProj, new Bcurve(points), 8, RAINING_TIME + RAIN_DELAY * i, true);
            auto collider = new Collider(*goProj);
            collider->SetScale(Vec2(0.5, 0.5));
            auto attack = new Attack(*goProj, RAIN_DAMAGE, 1, &associated);
            auto disp = new DisappearOnHit(*goProj, &associated);
            auto dispo = new DisappearOnDeadOwner(*goProj, st.GetObject(C_ID::Mask, &st.enemyArray));
            goProj->AddComponents({spt, proj, collider, attack, disp, dispo});
        st.bulletArray.emplace_back(goProj);

        Vec2 p2b = Vec2((i + 18) * 60 + 30, -720);
        Vec2 p3b = Vec2((i + 18) * 60 + 30, 720);
        std::vector<Vec2> points1 = {p1, p2b, p3b};
        auto goProj1 = new GameObject();
            goProj1->depth = -1;
            auto spt1 = new Sprite(*goProj1, "assets/img/Mask_Atks/fire_rain.png", 4, 0.1);
            auto proj1 = new ProjectileB(*goProj1, new Bcurve(points1), 8, RAINING_TIME + RAIN_DELAY * i, true);
            auto collider1 = new Collider(*goProj1);
            collider->SetScale(Vec2(0.5, 0.5));
            auto attack1 = new Attack(*goProj1, RAIN_DAMAGE, 1, &associated);
            auto disp1 = new DisappearOnHit(*goProj1, &associated);
            auto dispo1 = new DisappearOnDeadOwner(*goProj1, st.GetObject(C_ID::Mask, &st.enemyArray));
            goProj1->AddComponents({spt1, proj1, collider1, attack1, disp1, dispo1});
        st.bulletArray.emplace_back(goProj1);
    }

    timer.Restart();
    timer.SetFinish(RAINING_TIME + RAIN_DELAY * 11 + 0.2f);
    CURR_STATE = COOLING;
}

void MaskBoss::Attacking(float dt){
    if(CURR_MASK == YELLOW)
        CURR_ATK ? Lasering(dt) : Raining();
    else if(CURR_MASK == RED)
        CURR_ATK ? Flaming() : Shooting(dt);
    else
        Spiking(CURR_ATK, SPIKE_NUM);
}

void MaskBoss::Cooling(float dt){
    if(timer.Update(dt)){
        if(CURR_PHASE == PHASE1 || DOUBLE_HIT){
            DOUBLE_HIT = false;
            CURR_STATE = MOVING;
        }
        else{
            DOUBLE_HIT = true;
            CURR_STATE = IDLE;
        }
    }
}

void MaskBoss::Swapping(float dt){
    if(timer.Update(dt)){
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

        CURR_STATE = CHARGING;
    }
}

void MaskBoss::Update(float dt){
    auto& st = Game::GetInstance().GetCurrentState();
    auto target = st.GetObject(C_ID::Player, &st.rigidArray);
    if(target.expired()){
        Die();
        return;
    }
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
            Charging(dt);
            break;
        case ATTAKING:
            Attacking(dt);
            break;
        case COOLING:
            Cooling(dt);
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

void MaskBoss::NotifyCollision(GameObject& other, Vec2 sep){
    auto attack = (Attack*)other.GetComponent(C_ID::Attack);
    if(!attack || attack->OwnedBy(&associated))
        return;
    
    TakeDamage(attack->GetDamage());
}

void MaskBoss::TakeDamage(int damage){
    if(CURR_STATE == INACTIVED)
        return;

    currentHealth -= damage;
    if(currentHealth <= 0) 
        Die();
}

void MaskBoss::Die(){
    GameData::isAlive[1]=false;
    associated.RequestDelete();
}