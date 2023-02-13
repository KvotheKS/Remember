#include "Bcurve.h"

void Bcurve::binomial(int n){
    int bin = 1;
    for(int i = 1; i <= n; i++){
        binom.push_back(bin);
        bin = bin * (n - i) / i;
    }
}

Bcurve::Bcurve(std::vector<Vec2> points){
    int n = points.size();
    if(n > 0){
        this->points = points;
        binomial(n);
    }
}

Vec2 Bcurve::GetNewPoint(float t){
    Vec2 newPoint = Vec2(0, 0);
    int n = points.size() - 1;

    for(int i = 0; i < points.size(); i++){
        newPoint.x += binom[i] * std::pow((1-t), (n-i)) * std::pow(t, i) * points[i].x;
        newPoint.y += binom[i] * std::pow((1-t), (n-i)) * std::pow(t, i) * points[i].y;
    }

    return newPoint;
}