#include "Rect.h"

Rect::Rect() : Rect(0, 0, 0, 0){
}

Rect::Rect(float x, float y, float w, float h){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

Rect Rect::operator+(Vec2 vec){
    return Rect(this->x + vec.x, this->y + vec.y, this->w, this->h);
}

Rect Rect::operator+(Rect rct)
{
    return Rect(this->x + rct.x, this->y + rct.y, this->w, this->h);
}

Rect operator+(Vec2 vec, Rect rect){
    return Rect(vec.x + rect.x, vec.y + rect.y, rect.w, rect.h);
}

Rect Rect::operator-(Vec2 vec){
    return Rect(this->x - vec.x, this->y - vec.y, this->w, this->h);
}

Rect Rect::operator-(Rect rct)
{
    return Rect(this->x - rct.x, this->y - rct.y, this->w, this->h);
}

Rect operator-(Vec2 vec, Rect rect){
    return Rect(vec.x - rect.x, vec.y - rect.y, rect.w, rect.h);
}

Rect& Rect::operator+=(Vec2 vec){
    *this = *this + vec;
    return *this;
}

Rect& Rect::operator+=(Rect rct)
{
    *this = *this + rct;
    return *this;
}
Rect& Rect::operator-=(Vec2 vec){
    *this = *this - vec;
    return *this;
}
Rect& Rect::operator-=(Rect rct){
    *this = *this - rct;
    return *this;
}

Vec2 Rect::GetCenter(){
    return Vec2(this->x + (this->w / 2), this->y + (this-> h / 2));
}

void Rect::SetCenter(float x, float y){
    this->x = x - (this->w / 2);
    this->y = y - (this->h / 2);
}

float Rect::CenterDistance(Rect rect){
    Vec2 center1 = this->GetCenter();
    Vec2 center2 = rect.GetCenter();
    return center1.Distance(center2);
}

bool Rect::IsInside(Vec2 vec){
    if(vec.x < this->x || vec.x > this->x + this->w)
        return false;
    else if(vec.y < this->y || vec.y > this->y + this->h)
        return false;
    return true;
}