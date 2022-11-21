#pragma once
#include "Vec2.h"

class Rect {
    public:
        float x, y, w, h;
        Rect();
        Rect(float x, float y, float w, float h);
        Rect operator+(Vec2 vec);
        Rect& operator+=(Vec2 vec);
        friend Rect operator+(Vec2 vec, Rect rect);
        Vec2 GetCenter();
        void SetCenter(float x, float y);
        float CenterDistance(Rect rect);
        bool IsInside(Vec2 vec);
};