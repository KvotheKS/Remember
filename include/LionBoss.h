#pragma once
#include "GameObject.h"


/*
    isso aq vai ser o "Gerador" do boss.
    Nele que inicializaremos todas as coisas
*/

class LionBoss : public GameObject
{   
    enum {LASER, BALLS, TOWER, SHOCKWAVE};
    enum {IDLE, LASERING, BALLING, TOWERING, SHOCKWAVING};

    Vec2 LIONSIZE;

    float CLOSEX;
    float MIDDLEX;
    float FARX;
    
    float LASERDURATION;
    float TOWERDURATION;
    float BALLSDURATION;
    float SHOCKWAVEDURATION;
    
    Vec2 LASERSIZE;
    int LASERDAMAGE;
    Vec2 LASERKNOCK;

    int BALLSDAMAGE;
    Vec2 BALLSKNOCK;
    float BALLSSPEED, BALLSYSPEED;
    Vec2 BALLSIZE;

    int TOWERDAMAGE;
    Vec2 TOWERKNOCK;

    float SHOCKWAVESPEED; 
    Vec2 WAVESIZE;
    int SHOCKWAVEDAMAGE;
    Vec2 SHOCKWAVEKNOCK;
    
    bool activated;
    
private:
    void Laser();
    void FlameBalls();
    void FlameTower();
    void ShockWave();

public:
    LionBoss(GameObject&); // Devemos settar sua posicao manualmente por seu associated.
    void Update(float);
    bool Is(C_ID);
};