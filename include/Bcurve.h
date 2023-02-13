#pragma once
#include "Vec2.h"
#include <vector>
#include <algorithm>
#include <cmath>

class Bcurve {
    private:
        std::vector<int> binom;
        std::vector<Vec2> points;
        void binomial(int n);
    public:
        Bcurve(std::vector<Vec2> points);
        Vec2 GetNewPoint(float t);
};