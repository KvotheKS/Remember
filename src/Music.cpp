#include "Music.h"

Music::Music(){
    music = nullptr;
}

Music::Music(std::string file) : Music(){
    Open(file);
}

Music::~Music(){
    Stop();
}

void Music::Play(int times){
    if(!IsOpen()){
        std::cerr << "Cannot play Music Object with null music" << std::endl;
        exit(EXIT_FAILURE);
    }
    Mix_PlayMusic(music.get(), times);
    Mix_VolumeMusic(20);
}

void Music::Stop(int msToStop){
    Mix_FadeOutMusic(msToStop);
}

void Music::Open(std::string file){
    music = Resources::GetMusic(file);
    if(!IsOpen()){
        std::cerr << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool Music::IsOpen(){
    return music != nullptr;
}