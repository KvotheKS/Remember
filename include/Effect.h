#pragma once
#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"
#include "Collider.h"
#include "Vec2.h"
#include <string>

class Effect : public GameObject {
    private:
        Sprite* sprite;
        Vec2 speed;
        float distanceLeft;
        float duration;
        Timer duration_timer;

    public:
        bool targetsPlayer;
        Effect(GameObject& associated, float angle, float speed, float maxDistance,float duration,
               std::string sprite, int frameCount, float frameTime);
        Effect(GameObject& associated, float angle, float duration,
                std::string sprite, int frameCount, float frameTime);
        Effect(GameObject& associated, float angle, float duration,
                std::string sprite);
        void Update(float dt);
        void SetColor(Uint8 r , Uint8 g ,Uint8 b, Uint8 a);
        void Render();
        bool Is(std::string type);
        bool Is(C_ID);
     
};