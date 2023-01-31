#pragma once

class Timer {
    private:
        float time;
        float finish;
    public:
        Timer(float finish = -1.0f);
        bool Update(float dt);
        void Restart();
        float Get();
        void Set(float t);
        void SetFinish(float);
        float GetFinish();
};