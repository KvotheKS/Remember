#include "Text.h"
#include "Game.h"
#include "Scheduler.h"

Text::Text(GameObject& associated, std::string fontFile, int fontSize, TextStyle style, std::string text,
           SDL_Color color, float cooldown) : GameObject(associated){
    this->fontFile = fontFile;
    this->fontSize = fontSize;
    this->style = style;
    this->text = text;
    this->color = color;
    texture = nullptr;

    this->cooldown = cooldown;
    timer.Restart();
    showText = true;

    RemakeTexture();
}

Text::~Text(){
    if(texture != nullptr)
        SDL_DestroyTexture(texture);
}

void Text::RemakeTexture(){
    if(texture != nullptr)
        SDL_DestroyTexture(texture);

    font = Resources::GetFont(fontFile, fontSize);
    if(font == nullptr){
        std::cerr << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Surface* surface = nullptr;
    if(style == TextStyle::SOLID)
        surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);
    else if(style == TextStyle::SHADED)
        surface = TTF_RenderUTF8_Shaded(font.get(), text.c_str(), color, {0, 0, 0, 0});
    else if(style == TextStyle::BLENDED)
        surface = TTF_RenderText_Blended(font.get(), text.c_str(), color);

    if(surface == nullptr){
        std::cerr << "Cannot create Font Texture with null Surface" << std::endl;
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    if(texture == nullptr){
        std::cerr << "Cannot render null Font Texture" << std::endl;
        exit(EXIT_FAILURE);
    }

    associated.box.w = surface->w;
    associated.box.h = surface->h;
    SDL_FreeSurface(surface);
}

void Text::Update(float dt){
    timer.Update(dt);
    if(timer.Get() > cooldown){
        showText = !showText;
        timer.Restart();
    }
}

void Text::Render(){
    if(texture != nullptr && showText)
        Scheduler::Push(this, associated.depth);
}

void Text::Print(float x, float y)
{
    SDL_Rect ClipRect = {0, 0, (int) associated.box.w, (int) associated.box.h};
        SDL_Rect DestRect;
        DestRect.x = associated.box.x - Camera::pos.x;
        DestRect.y = associated.box.y - Camera::pos.y;
        DestRect.w = ClipRect.w;
        DestRect.h = ClipRect.h;
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &ClipRect, &DestRect,
            associated.angleDeg, nullptr, SDL_FLIP_NONE);
}

bool Text::Is(std::string type){
    return type == "Text";
}

bool Text::Is(C_ID type)
{ return type == C_ID::Text; }

void Text::SetText(std::string text){
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color){
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style){
    this->style = style;
    RemakeTexture();
}

void Text::SetFontFile(std::string fontFile){
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize){
    this->fontSize = fontSize;
    RemakeTexture();
}