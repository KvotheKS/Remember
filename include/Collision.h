#pragma once
#include "Rect.h"
#include "Vec2.h"
#include <algorithm>




class Collision {
public:
    static bool IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB);

};