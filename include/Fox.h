#pragma once

#include "GameObject.h"

class Fox : public GameObject
{
public:
    enum ANM {IDLE, BULLETHELLANIM, DAMAGED, KONANIM, SNAPANIM, STUNNED};
    enum ATK {ARCBALLS, KON, LIONLASER, BULLETHELL, WÕE, COMETS};

public:
    Vec2 FOXSIZE;

    Vec2 ARCBALLSSIZE;
    int ARCBALLSQTT;

    Vec2 KONSIZE;
    float KONSETUPTIME;

    float BULLETHELLDURATION;

    float WÕEXSPEED; // Wõe eh o que cria 2 projeteis q vão dos pontos do estágio e quando os projeteis se encontram eles invertem o angulo 180graus

    float LIONPHASINGTIME;
    float LIONLASERDURATION;

    float COMETSSPACEBETWEEN;
    Vec2 COMETSIZE;
    Vec2 COMETSPEED;


    bool activated;
    bool moving;
    int currAttack;
    bool phase2;
    
    GameObject* player;

public:
    Fox(GameObject&);
    void Update(float);
    void SetVariables();
    void Phase2Transition();

    void ARCBALL_f();
    void KON_f();
    void BULLETHELL_f();
    void WÕE_f();
    void LIONLASERS_f();
    void COMET_f();
    
};