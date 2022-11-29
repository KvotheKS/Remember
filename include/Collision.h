#pragma once
#include "Rect.h"
#include "Vec2.h"
#include <algorithm>




class Collision {
public:
    static pair<bool,Vec2> IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB);

};