#pragma once

#include "GameObject.h"

class Fox : public GameObject
{
public:
    enum ANM {IDLE, BULLETHELLANIM, BULLETHELLIDLEANIM, DAMAGED, KONANIM, KONIDLEANIM, SNAPANIM, STUNNED, SWIPEANIM, SNAPIDLEANIM};
    enum ATK {ARCBALLS, KON, LIONLASER, BULLETHELL, WÕE, COMETS, TORNADO};

public:
    GameObject* RealFox;
    float REALFOXFRAMES;
    float REALFOXFRAMETIME;

    float PHASE2TRANSITION;
    float WAITTRANSITION;

    Vec2 FOXSIZE;
    float IDLEFRAMES;
    float IDLEFRAMETIME;

    float SNAPTRANSITIONFRAMES;
    float SNAPTRANSITIONFRAMETIME;

    float SNAPMOVEMENTFRAMES;
    float SNAPMOVEMENTFRAMETIME;

    float WARNINGCIRCLEFRAMES;

    Vec2 ARCBALLSSIZE;
    int ARCBALLSQTT;
    float ARCBALLSDURATION;
    float ARCBALLSTIMEBETWEEN;
    float ARCBALLSINITIALYSPEED;
    float ARCBALLSFIRSTTIME; 
    float ARCBALLSSPACEBETWEEN;
    int ARCBALLSDAMAGE;
    int ARCBALLSKNOCKBACK;

    Vec2 KONSIZE;
    float KONSETUPTIME;
    float KONDURATION;
    float KONTRANSITIONFRAMES;
    float KONTRANSITIONFRAMETIME;
    float KONMOVEMENTFRAMES;
    float KONMOVEMENTFRAMETIME;
    float KONFRAMES;
    float KONFRAMETIME;
    float KONLASERDURATION;
    float KONFADEOUT;
    Vec2 KONLASERSIZE;
    int KONDAMAGE;
    int KONKNOCKBACK;

    float BULLETHELLDURATION;
    float BULLETHELLWAVES;
    float BULLETHELLTIMEBETWEEN;
    float BULLETHELLMOVEMENTFRAMES;
    float BULLETHELLMOVEMENTFRAMETIME;
    float BULLETHELLTRANSITIONFRAMES;
    float BULLETHELLTRANSITIONFRAMETIME;
    
    float WÕEXSPEED; // Wõe eh o que cria 2 projeteis q vão dos pontos do estágio e quando os projeteis se encontram eles invertem o angulo 180graus
    float WÕEDURATION;
    Vec2 WÕESIZE;
    int WÕEDAMAGE;
    int WÕEKNOCKBACK;

    float LIONPHASINGTIME;
    float LIONLASERDURATION;
    float LIONLASERFULLDURATION;
    float LIONLASERDAMAGEDURATION;
    Vec2  LIONLASERSIZE;
    int LIONLASERDAMAGE;
    int LIONLASERKNOCKBACK;

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
    int COMETDAMAGE;
    int COMETKNOCKBACK;

    float TORNADODURATION;
    Vec2 TORNADOSIZE;
    float TORNADOLIVEDURATION;
    int TORNADODAMAGE;
    int TORNADOKNOCKBACK;

    int PHASE1LIFE;
    int PHASE2LIFE;
    int currentLife;

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
    void NotifyCollision(GameObject& other, Vec2 sep);
    void DIEEEE();
    bool Is(C_ID);
};

void PhasingLion(GameObject&, float);
void UnphasingLion(GameObject&, float);
void UnphasingLionTrg(GameObject& associated);
void SpawnLionLaser(GameObject&);

bool FinishedSnap(GameObject&);
void ToPhase2Time(GameObject&);
