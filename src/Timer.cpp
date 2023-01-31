#include "Timer.h"

Timer::Timer(float finish){
    time = 0.0;
    this->finish = finish;
}

bool Timer::Update(float dt){
    bool rs = (time += dt) > finish;
    return finish > 0.0f && rs;
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