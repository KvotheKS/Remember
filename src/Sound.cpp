#include "Sound.h"

Sound::Sound(GameObject& associated) : GameObject(associated){
    chunk = nullptr;
}

Sound::Sound(GameObject& associated, std::string file) : Sound(associated){
    Open(file);
}

Sound::~Sound(){
}

void Sound::Play(int times){
    if(!IsOpen()){
        std::cerr << "Cannot play Sound Object with null chunk" << std::endl;
        exit(EXIT_FAILURE);
    }
    channel = Mix_PlayChannel(-1, chunk.get(), times-1);
    Mix_VolumeChunk(chunk.get(), 50);
}

void Sound::Stop(){
    if(!IsOpen()){
        std::cerr << "Cannot stop Sound Object with null chunk" << std::endl;
        exit(EXIT_FAILURE);
    }
    Mix_HaltChannel(channel);
}

void Sound::Open(std::string file){
    chunk = Resources::GetSound(file);
    if(!IsOpen()){
        std::cerr << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool Sound::IsOpen(){
    return chunk != nullptr;
}

bool Sound::IsPlaying(){
    return Mix_Playing(channel);
}

void Sound::Update(float dt){
}

void Sound::Render(){
}

bool Sound::Is(std::string type){
    return type == "Sound";
}

bool Sound::Is(C_ID type)
{ return type == C_ID::Sound; }