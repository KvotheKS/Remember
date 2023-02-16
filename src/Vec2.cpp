#include "Vec2.h"

Vec2 Vec2::ZERO(0,0);

Vec2::Vec2() : Vec2(0, 0){
}

Vec2::Vec2(float x, float y){
    this->x = x;
    this->y = y;
}

bool Vec2::operator==(Vec2 vec){
    return (this->x == vec.x && this->y == vec.y);
}
Vec2 Vec2::operator+(Vec2 vec){
    return Vec2(this->x + vec.x, this->y + vec.y);
}

Vec2& Vec2::operator+=(Vec2 vec){
    *this = *this + vec;
    return *this;
}

Vec2 Vec2::operator-(Vec2 vec){
    return Vec2(this->x - vec.x, this->y - vec.y);
}

Vec2& Vec2::operator-=(Vec2 vec){
    *this = *this - vec;
    return *this;
}

Vec2 Vec2::operator*(float scalar){
    return Vec2(this->x * scalar, this->y * scalar);
}

Vec2 operator*(float scalar, Vec2 vec){
    return Vec2(scalar * vec.x, scalar * vec.y);
}

float Vec2::Magnitude(){
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

Vec2 Vec2::Normalize(){
    float magni = this->Magnitude();
    if(magni != 0)
        return Vec2(this->x / magni, this->y / magni);
    return *this;
}

float Vec2::Distance(Vec2 vec){
    Vec2 aux = *this - vec;
    return aux.Magnitude();
}

float Vec2::AngleX(){
    return atan2(this->y, this->x);
}

float Vec2::AngleLine(Vec2 vec){
    Vec2 aux = vec - *this;
    return atan2(aux.y, aux.x);
}

Vec2 Vec2::Rotate(float angle){
    float cosTh = cos(angle);
    float sinTh = sin(angle);
    float xr = x * cosTh - y * sinTh;
    float yr = x * sinTh + y * cosTh;
    return Vec2(xr, yr);

}

float Vec2::Dot(Vec2 vec){
    return this->x * vec.x + this->y * vec.y;
}

float Vec2::Cross(Vec2 vec){
    return this->x * vec.y - this->y * vec.x;
}

ostream& operator<<(ostream& out,Vec2 const& v){
    out << "Vec2: "<< (float)v.x << " , "<< (float)v.y << "\n";
    return out ;
}

bool eq_s(float a, float b, float eps)
{
    return a >= (b-eps) && a <= (b+eps);
}

bool gt_s(float a, float b, float eps)
{
    return a >= (b + eps);
}

bool lt_s(float a, float b, float eps)
{
    return a <= (b - eps);
}