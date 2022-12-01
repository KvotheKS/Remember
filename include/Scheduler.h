#pragma once
#define INCLUDE_SDL
#include "SDL_include.h"
#include "Component.h"
#include <memory>
#include <string>
#include <vector>

struct __info
{
    GameObject* cpt;
    float depth;
    float x, y;
};

/*
    Pra printar algo pelo scheduler (que sera executado apos todos os Render() dos components),
    basta dar um push(); como dita abaixo. Tomar cuidado com o fato de que depth eh de baixo pra cima,
    ou seja, quanto menor o depth mais cedo ele eh printado.
    
    Como detalhe final (e talvez mais importante): Component agora tem a funcao Print. No caso de uma funcao chamar 
    o Scheduler, ela deve ter um overload da funcao print, de forma que ela printe as coisas.
    (Veja TileMap como exemplo mais complexo disso, e Sprite para o exemplo mais simples).
    
    Nota: as coordenadas x e y foram inicialmente pensadas pra compensar o Render(x,y) da Sprite, porem 
    tambem podem ser utilizados para passar outras informacoes para os components. Por exemplo, no TileMap,
    a coordenada x eh utilizada para especificar qual layer do mapa q deve ser printado, de forma que eu passo
    o mesmo tilemap varias vezes para o Scheduler, porem toda vez eu passo um depth e um x diferentes.
*/

class Scheduler
{
    static std::vector<__info> textureVec;
public:
    static void Render();
    static void Push(GameObject* cpt, float depth = 0.0f, float x = -1, float y = -1);
    static void Clear();
};