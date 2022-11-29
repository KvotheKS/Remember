#include "GameObject.h"
#include "Collider.h"
GameObject::GameObject(){
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

void GameObject::Update(float dt){
    for(unsigned i = 0; i < components.size(); i++)
        components[i]->Update(dt);
    if(Collider* collider = (Collider*)GetComponent("Collider")){
        collider->Update(dt);
    }
}

void GameObject::Render(){
    for(unsigned i = 0; i < components.size(); i++)
        components[i]->Render();
}

bool GameObject::IsDead(){
    return isDead;
}

void GameObject::RequestDelete(){
    isDead = true;
}

void GameObject::AddComponent(Component* cpt){
    components.emplace_back(cpt);
    if(started)
        cpt->Start();
}

void GameObject::RemoveComponent(Component* cpt){
    for(unsigned i = 0; i < components.size(); i++){
        if(components[i].get() == cpt){
            components.erase(components.begin()+i);
            break;
        }
    }
}

Component* GameObject::GetComponent(std::string type){
    for(unsigned i = 0; i < components.size(); i++)
        if(components[i]->Is(type))
            return components[i].get();
    return nullptr;
}

void GameObject::NotifyCollision(GameObject& other,Vec2 sep){
    for(unsigned i = 0; i < components.size(); i++)
        components[i]->NotifyCollision(other,sep);
}