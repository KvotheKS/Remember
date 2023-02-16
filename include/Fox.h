#pragma once

#include "GameObject.h"

class Fox : public GameObject
{
public:
    enum ANM {IDLE, BULLETHELLANIM, DAMAGED, KONANIM, SNAPANIM, STUNNED};
    enum ATK {ARCBALLS, KON, LIONLASER, BULLETHELL, WÕE, COMETS, TORNADO};

public:
    
    Vec2 FOXSIZE;

    Vec2 ARCBALLSSIZE;
    int ARCBALLSQTT;
    float ARCBALLSDURATION;
    float ARCBALLSTIMEBETWEEN;
    float ARCBALLSINITIALYSPEED;
    float ARCBALLSFIRSTTIME; 
    float ARCBALLSSPACEBETWEEN;

    Vec2 KONSIZE;
    float KONSETUPTIME;
    float KONDURATION;

    float BULLETHELLDURATION;
    
    float WÕEXSPEED; // Wõe eh o que cria 2 projeteis q vão dos pontos do estágio e quando os projeteis se encontram eles invertem o angulo 180graus
    float WÕEDURATION;
    Vec2 WÕESIZE;

    float LIONPHASINGTIME;
    float LIONLASERDURATION;

    Rect StageLBound;
    Rect StageRBound;
    float COMETSSPACEBETWEEN;
    Vec2 COMETSIZE;
    float COMETLANGLE;
    float COMETRANGLE;
    float COMETSPEED;
    float COMETFIRSTTIME;
    float COMETDELAY;
    float COMETDURATION;

    float TORNADODURATION;
    Vec2 TORNADOSIZE;
    float TORNADOLIVEDURATION;


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
    void TORNADO_f();
};