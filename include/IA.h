#pragma once

#include "GameObject.h"
#include <vector>
#include "Rect.h"
#include "Timer.h"
#include <cmath>

struct ActionInfo
{
    Vec2 range;
    float rarity;
    float cooldown;
    float weight;
};

/*
    Para ver exemplo de como usar, veja a Enemy.cpp  //

    Neste calculo, 2 coisas sao customizaveis na heuristica:
        - positionWeight : Determina o quanto o boss se importa com a distancia entre os ataques
                           e o target. Essa variavel altera EXPONENCIALMENTE o resultado, entao
                           a diferenca de 1 e 1.5, eh mt maior do que parece.
        - ActionInfo.rarity: Isso eh uma variavel unica de CADA ataque. ela determina LINEARMENTE
                             o quao raro um ataque deveria ser. Entao se o boss tem algum ataque
                             que eh mt forte e n deveria spammar tanto, bota ele como raridade 5,
                             o que fara com q ele fique 5 vezes mais raro que um raride 1 !
    ActionInfo.cooldown eh uma variavel que determina por quanto tempo cada ataque "cansa" o boss.
    Como assim? depois da IA fazer uma decisao, o cooldown da acao escolhida e jogado em um TIMER.
    Enquanto timer < ActionInfo.cooldown, a IA NAO FAZ ESCOLHAS.Ela, porem, eh opcional, no sentido de: 
    Algum outro component pode chamar a funcao SetActionTimer(float) e fazer a IA descansar por um tempo arbitrario.
    Caso voce queira que as acoes NUNCA bloqueiem a IA, basta settar esta variavel a algum numero negativo.
    ActionInfo.weight eh utilizado localmente, nao deve ser settado, e mesmo q seja, nao vai alterar nada.
    target: eh o GameObject alvo (duuuh). Como voce vai conseguir ele eh problema seu. No Enemy.cpp esta um jeito
    modularizado de encontrar o player (como exemplo).
*/

class IA : public GameObject
{
public:
    Timer tm;
    std::vector<ActionInfo> actions;
    GameObject* target;
    float positionWeight;
    int selectedAction;
    bool rendered;
public:
    IA(GameObject&, GameObject* = nullptr, float = 1.0f);
    void Update(float);
    void Render();
    void SetActionTimer(int);
    void SetActionTimer(float);
    void SetActions(std::vector<ActionInfo>);
    void AddAction(std::vector<ActionInfo>);
    void AddAction(ActionInfo);
    void Heuristic(ActionInfo&);
    void SetTarget(GameObject*);
    bool Cooling();
    bool Is(C_ID);
};