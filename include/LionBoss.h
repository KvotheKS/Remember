#pragma once
#include "GameObject.h"


/*
    isso aq vai ser o "Gerador" do boss.
    Nele que inicializaremos todas as coisas
*/

class LionBoss : public GameObject
{   
    const float CLOSEX = 50.0f;
    const float MIDDLEX = 150.0f;
    const float FARX = 300.f;
    
public:
    LionBoss(GameObject&); // Devemos settar sua posicao manualmente por seu associated.
    void Update(float);
};