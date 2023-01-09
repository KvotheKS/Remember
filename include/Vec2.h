#pragma once
#include <cmath>
#include <iostream> //por que n ter? -m
using namespace std;

// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";

#define PI 3.14159265358979323846

class Vec2 {
    public:
        static Vec2 ZERO;
    public:
        float x, y;
        Vec2();
        Vec2(float x, float y);
        Vec2 operator+(Vec2 vec);
        Vec2& operator+=(Vec2 vec);
        Vec2 operator-(Vec2 vec);
        Vec2& operator-=(Vec2 vec);
        Vec2 operator*(float scalar);
        friend Vec2 operator*(float scalar, Vec2 vec);
        float Magnitude();
        Vec2 Normalize();
        float Distance(Vec2 vec);
        float AngleX();
        float AngleLine(Vec2 vec);
        Vec2 Rotate(float angle);
        float Dot(Vec2 vec);
        friend ostream& operator<<(ostream& out,Vec2 const& v);
};