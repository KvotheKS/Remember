#pragma once
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include "Resources.h"
#include "Component.h"
#include "GameObject.h"
#include <string>
#include <memory>
#include <iostream>

class Sound : public Component {
    private:
        std::shared_ptr<Mix_Chunk> chunk;
        int channel;
    public:
        Sound(GameObject& associated);
        Sound(GameObject& associated, std::string file);
        ~Sound();
        void Play(int times = 1);
        void Stop();
        void Open(std::string file);
        bool IsOpen();
        bool IsPlaying();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
};