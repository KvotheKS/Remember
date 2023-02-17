#pragma once

#include "GameObject.h"

class Kon : public GameObject
{
public:
    Kon(GameObject&, float, float);
    void ResetWorld();
    bool Is(C_ID);
};

bool OpenMouth(GameObject&);

void KonShootLaser(GameObject&);
void KonFade(GameObject&,float);
void KonDisappear(GameObject&);