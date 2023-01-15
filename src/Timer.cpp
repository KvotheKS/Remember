#include "Timer.h"

Timer::Timer(float finish){
    time = 0.0;
    finish = 0.0f;
}

bool Timer::Update(float dt){
    return (time += dt) > finish;
}

void Timer::Restart(){
    time = 0.0;
}

float Timer::Get(){
    return time;
}
void Timer::Set(float t){
    time = t;
}

void Timer::SetFinish(float finish)
{ this->finish = finish; }

float Timer::GetFinish()
{ return finish; }