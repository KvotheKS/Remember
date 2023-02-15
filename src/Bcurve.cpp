#include "Bcurve.h"

void Bcurve::binomial(int n){
    int bin = 1;
    for(int i = 1; i <= n; i++){
        binom.push_back(bin);
        bin = bin * (n - i) / i;
    }
}

Bcurve::Bcurve(std::vector<Vec2> points, int n) : Bcurve(points){
    std::transform(binom.begin(), binom.end(), binom.begin(),
        std::bind(std::multiplies<int>(), std::placeholders::_1, n));
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

    for(unsigned i = 0; i < points.size(); i++){
        newPoint.x += binom[i] * std::pow((1-t), (n-i)) * std::pow(t, i) * points[i].x;
        newPoint.y += binom[i] * std::pow((1-t), (n-i)) * std::pow(t, i) * points[i].y;
    }

    return newPoint;
}

Bcurve* Bcurve::GetDerivate(){
    std::vector<Vec2> newPoints;
    int n = points.size() - 1;

    for(unsigned i = 0; i < points.size() - 1; i++)
        newPoints.push_back(points[i+1] - points[i]);
    
    Bcurve* derivate = new Bcurve(newPoints, n);
    return derivate;
}