#include "ShakeBehavior.h"
#include "Rand.h"

ShakeBehavior::ShakeBehavior(GameObject& associated, Vec2 amplitude, float duration, float frequency)
    : GameObject(associated), amplitude(amplitude), duration(duration), frequency(frequency)
{}

void ShakeBehavior::Start()
{
    destination = box.GetCenter();
    duration.Restart();
    frequency.Restart();
    ShakeYoBooty();
}

void ShakeBehavior::ShakeYoBooty()
{
    float xrand = Rand::FloatRange(-amplitude.x, amplitude.x);
    float yrand = Rand::FloatRange(-amplitude.y, amplitude.y);
    std::cout << xrand << '\n';
    Vec2 lastdestination = destination;
    destination = box.GetCenter() + Vec2(xrand, yrand);
    dstdx = (destination - lastdestination).Normalize();
}

void ShakeBehavior::Update(float dt)
{
    float frqd = frequency.Get();
    if(comeBack)
    {
        if(frequency.Update(dt))
        {
            frqd = (frequency.GetFinish() - frqd)/frequency.GetFinish();
            associated.relative += fromOgn*frqd;
            associated.RemoveComponent(this);
            return;
        }
        associated.relative += fromOgn*(dt/frequency.GetFinish());
    }
    else if(duration.Update(dt))
    {
        frequency.Restart();
        duration.SetFinish(0.0f);
        comeBack = true;
    }
    else if(frequency.Update(dt))
    {
        frqd = (frequency.GetFinish() - frqd)/frequency.GetFinish();
        Vec2 currdx = dstdx*frqd;
        associated.relative += currdx;
        fromOgn -= currdx;
        frequency.Restart();
        ShakeYoBooty();
    }
    else
    {
        Vec2 currdx = dstdx*(dt/frequency.GetFinish());
        associated.relative += currdx;
        fromOgn -= currdx;
    }
}

bool ShakeBehavior::Is(C_ID type)
{
    return C_ID::ShakeBehavior == type;
}