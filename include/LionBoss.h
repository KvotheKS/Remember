#pragma once
#include "GameObject.h"


/*
    isso aq vai ser o "Gerador" do boss.
    Nele que inicializaremos todas as coisas
*/

class LionBoss : public GameObject
{   
public:
    enum {LASER, BALLS, TOWER, SHOCKWAVE};
    enum {IDLE, LASERING, BALLING, TOWERING, SHOCKWAVING, SHOCKWAVINGIDLE};
    enum {LASERBEGIN, LASERLOOP, LASEREND};
    Vec2 LIONSIZE;

    float CLOSEX;
    float MIDDLEX;
    float FARX;
    
    float LASERDURATION;
    float LASERCHARGE;
    float LASERCOOLDOWN;
    float TOWERDURATION;
    float BALLSDURATION;
    float SHOCKWAVEDURATION;
    
    Vec2 LASERSIZE;
    Vec2 LASERPREP;
    int LASERDAMAGE;
    Vec2 LASERKNOCK;
    float LASERINGRESTFRAMES;
    float LASERINGTOCOLLIDE;
    float LASERINGACTIVE;
    float LASERFRAMETIME;

    int BALLSDAMAGE;
    Vec2 BALLSKNOCK;
    float BALLSSPEED, BALLSYSPEED;
    Vec2 BALLSIZE;
    float BALLSFRAMES;
    float BALLSFRAMETIME;

    int TOWERDAMAGE;
    Vec2 TOWERKNOCK;
    Vec2 TOWERSIZE;
    float TOWERTOTALFRAMES;
    float TOWERFRAMESTOCREATE;
    float TOWERFRAMETIME;

    float SHOCKWAVESPEED; 
    Vec2 WAVESIZE;
    int SHOCKWAVEDAMAGE;
    Vec2 SHOCKWAVEKNOCK;
    float SHOCKWAVINGFRAMES;
    float SHOCKWAVINGDURATION;
    float SHOCKWAVINGXFRAMES;

    int LIONMAXHEALTH;

    float EXPLOSIONDURATION;
    Vec2 LIONCONTACTKNOCK;

    int currentHealth;

    bool activated;
    
private:
    void Laser();
    void FlameBalls();
    void FlameTower();
    void ShockWave();

public:
    LionBoss(GameObject&); // Devemos settar sua posicao manualmente por seu associated.
    void NotifyCollision(GameObject&, Vec2);
    void TakeDamage(int damage);
    void TakeDamageVisuals();
    void DIEEE();
    void Update(float);
    // void Start();
    bool Is(C_ID);
};