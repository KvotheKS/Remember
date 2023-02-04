#pragma once
#include <cmath>
#include <iostream> //por que n ter? -m
#include <tuple>
#include <cmath>

using namespace std;

// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";

#define PI 3.14159265358979323846
const float _f_leniancy = 0.005f;

class Vec2 {
    public:
        static Vec2 ZERO;
    public:
        float x, y;
        Vec2();
        Vec2(float x, float y);
        bool operator==(Vec2 vec);
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
        float Cross(Vec2 vec);
        friend ostream& operator<<(ostream& out,Vec2 const& v);
};

bool eq_s(float a, float b, float eps = _f_leniancy);

bool gt_s(float a, float b, float eps = _f_leniancy);

bool lt_s(float a, float b, float eps = _f_leniancy);

// xspeed deve ser a velocidade por segundos
// Vec2 eh (x,y) que no projectile deve ser traduzido para
// initialSpeed = Vec2.Magnitude() e angle = Vec2.AngleX()
// float eh gravity