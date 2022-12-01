#include "GameObject.h"

GameObject::GameObject(GameObject& associated) : associated(associated) 
{
    isDead = false;
    started = false;
    angleDeg = 0.0;
    depth = 0;
}

GameObject::~GameObject(){
    components.clear();
}

void GameObject::Start(){
    for(unsigned i = 0; i < components.size(); i++)
        components[i]->Start();
    started = true;
}

void GameObject::UpdateNodes(float dt){
    for(unsigned i = 0; i < components.size(); i++)
        components[i]->Update(dt);
}

void GameObject::RenderNodes(){
    for(unsigned i = 0; i < components.size(); i++)
        components[i]->Render();
}

void GameObject::Update(float dt){}
void GameObject::Render(){}
void GameObject::Print(float x, float y){}
bool GameObject::Is(std::string type){ return type == "GameObject"; }
std::string GameObject::Is() { return "GameObject"; }
void NotifyCollision(GameObject& other){}

bool GameObject::IsDead(){
    return isDead;
}

void GameObject::RequestDelete(){
    isDead = true;
}

void GameObject::AddComponent(GameObject* cpt){
    components.emplace_back(cpt);
    if(started)
        cpt->Start();
}

void GameObject::RemoveComponent(GameObject* cpt){
    for(unsigned i = 0; i < components.size(); i++){
        if(components[i].get() == cpt){
            components.erase(components.begin()+i);
            break;
        }
    }
}

GameObject* GameObject::GetComponent(std::string type){
    for(unsigned i = 0; i < components.size(); i++)
        if(components[i]->Is(type))
            return components[i].get();
    return nullptr;
}

void GameObject::NotifyCollision(GameObject& other){
    for(unsigned i = 0; i < components.size(); i++)
        components[i]->NotifyCollision(other);
}