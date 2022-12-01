#pragma once
#define INCLUDE_SDL
#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "Resources.h"
#include "Component.h"
#include "GameObject.h"
#include "Camera.h"
#include "Timer.h"
#include <string>
#include <memory>
#include <iostream>

class Text : public GameObject {
    public:
        enum TextStyle {SOLID, SHADED, BLENDED};
    private:
        std::shared_ptr<TTF_Font> font;
        SDL_Texture* texture;
        std::string text;
        TextStyle style;
        std::string fontFile;
        int fontSize;
        SDL_Color color;
        Timer timer;
        float cooldown;
        bool showText;
        void RemakeTexture();
    public:
        Text(GameObject& associated, std::string fontFile, int fontSize, TextStyle style,
             std::string text, SDL_Color color, float cooldown);
        ~Text();
        void Update(float dt);
        void Render();
        void Print(float x = -1, float y = -1);
        bool Is(std::string type);
        void SetText(std::string text);
        void SetColor(SDL_Color color);
        void SetStyle(TextStyle style);
        void SetFontFile(std::string fontFile);
        void SetFontSize(int fontSize);
};