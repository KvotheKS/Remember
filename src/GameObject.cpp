#include "GameObject.h"
#include "Collider.h"
#include "TerrainBody.h"
#include <limits>

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

void GameObject::UpdateNodes(float dt)
{
    for(unsigned i = 0; i < components.size(); i++, deletedcpt=false)
    {
        components[i]->Update(dt);
        if(deletedcpt){ i--; continue;}
        components[i]->box = box + relative;
        components[i]->UpdateNodes(dt); 
    
    }
    // para garantir que o collider seja atualizado depois de sprite, ele esta sendo atualizado 2 vezes todo frame
    // pra concertar isso teria que ter um scheduler no Update() tambem.                                            -m
        if(Collider* collider = (Collider*)GetComponent("Collider")){
            collider->Update(dt);

        }
    
}

void GameObject::RenderNodes(){
    for(unsigned i = 0; i < components.size(); i++, deletedcpt=false)
    {   
        components[i]->Render();
        if(deletedcpt){ i--; continue;}
        
        components[i]->RenderNodes();
    }
}

void GameObject::Update(float dt){}
void GameObject::Render(){}
void GameObject::Print(float x, float y){}
bool GameObject::Is(std::string type){ return type == "GameObject"; }
bool GameObject::Is(C_ID type){ return C_ID::GameObject == type; }
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

void GameObject::AddComponent(GameObject* cpt, GameObject* base)
{
    for(auto it = components.begin(); ;it++)
    {
        if(it->get() == base || it == components.end())
            components.insert(it, std::unique_ptr<GameObject>(cpt));
    }
}

void GameObject::AddComponents(std::initializer_list<GameObject*> cpts)
{
    for(auto& it : cpts) AddComponent(it);
}

void GameObject::RemoveComponent(GameObject* cpt){
    for(unsigned i = 0; i < components.size(); i++){
        if(components[i].get() == cpt){
            components.erase(components.begin()+i);
            deletedcpt = true;
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

GameObject* GameObject::GetComponent(C_ID type){
    for(unsigned i = 0; i < components.size(); i++)
        if(components[i]->Is(type))
            return components[i].get();
    return nullptr;
}


std::vector<GameObject*> GameObject::GetComponents(std::string type)
{
    std::vector<GameObject*> ret;
    
    for(unsigned i = 0; i < components.size(); i++)
        if(components[i]->Is(type))
            ret.push_back(components[i].get());
    return ret;
}

std::vector<GameObject*> GameObject::GetComponents(C_ID type)
{
    std::vector<GameObject*> ret;
    
    for(unsigned i = 0; i < components.size(); i++)
        if(components[i]->Is(type))
            ret.push_back(components[i].get());
    return ret;
}

void GameObject::NotifyCollisionBehavior(GameObject& other,Vec2 sep){
    // if(Is(C_ID::Lion) && other.GetComponent(C_ID::Attack))
        // std::cout << "SYUUYUASRSUDSAU\n";
    NotifyCollision(other,sep);
    for(unsigned i = 0; i < components.size(); i++){
        
        components[i]->NotifyCollisionBehavior(other,sep);
        // components[i]->NotifyCollisionBehavior(other,sep);
    }
}
void GameObject::NotifyCollision(GameObject& other,Vec2 sep){ 
        
}