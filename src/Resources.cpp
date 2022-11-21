#include "Resources.h"
#include "Game.h"

std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> Resources::imageTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Music>> Resources::musicTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> Resources::soundTable;
std::unordered_map<std::string, std::shared_ptr<TTF_Font>> Resources::fontTable;

std::shared_ptr<SDL_Texture> Resources::GetImage(std::string file){
    if(imageTable.find(file) == imageTable.end()){
        SDL_Texture* image = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
        if(image != nullptr)
            imageTable[file] = std::shared_ptr<SDL_Texture>(image, [](SDL_Texture* ptr){SDL_DestroyTexture(ptr);});
        else
            return nullptr;
    }
    return imageTable[file];
}

void Resources::ClearImages(){
    for(auto img = imageTable.begin(); img != imageTable.end(); ){
        if(img->second.unique())
            img = imageTable.erase(img);
        else
            img++;
    }
}

std::shared_ptr<Mix_Music> Resources::GetMusic(std::string file){
    if(musicTable.find(file) == musicTable.end()){
        Mix_Music* music = Mix_LoadMUS(file.c_str());
        if(music != nullptr)
            musicTable[file] = std::shared_ptr<Mix_Music>(music, [](Mix_Music* ptr){Mix_FreeMusic(ptr);});
        else
            return nullptr;
    }
    return musicTable[file];
}

void Resources::ClearMusics(){
    for(auto msc = musicTable.begin(); msc != musicTable.end(); ){
        if(msc->second.unique())
            msc = musicTable.erase(msc);
        else
            msc++;
    }
}

std::shared_ptr<Mix_Chunk> Resources::GetSound(std::string file){
    if(soundTable.find(file) == soundTable.end()){
        Mix_Chunk* sound = Mix_LoadWAV(file.c_str());
        if(sound != nullptr)
            soundTable[file] = std::shared_ptr<Mix_Chunk>(sound, [](Mix_Chunk* ptr){Mix_FreeChunk(ptr);});
        else
            return nullptr;
    }
    return soundTable[file];
}

void Resources::ClearSounds(){
    for(auto snd = soundTable.begin(); snd != soundTable.end(); ){
        if(snd->second.unique())
            snd = soundTable.erase(snd);
        else
            snd++;
    }
}

std::shared_ptr<TTF_Font> Resources::GetFont(std::string file, int size){
    std::string key = file + std::to_string(size);
    if(fontTable.find(key) == fontTable.end()){
        TTF_Font* font = TTF_OpenFont(file.c_str(), size);
        if(font != nullptr)
            fontTable[key] = std::shared_ptr<TTF_Font>(font, [](TTF_Font* ptr){TTF_CloseFont(ptr);});
        else
            return nullptr;
    }
    return fontTable[key];
}

void Resources::ClearFonts(){
    for(auto fnt = fontTable.begin(); fnt != fontTable.end(); ){
        if(fnt->second.unique())
            fnt = fontTable.erase(fnt);
        else
            fnt++;
    }
}

void Resources::ClearResources(){
    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();
}