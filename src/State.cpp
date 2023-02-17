#include "State.h"

State::State(){
    popRequested = false;
    quitRequested = false;
    started = false;
    arrays = GetArrays();
}

State::~State(){
    objectArray.clear();
}

void State::StartArray()
{
    for(auto it : arrays)
        StartVector(*it);
}

void State::StartVector(std::vector<std::shared_ptr<GameObject>>& ObjectArr){
    for(unsigned i = 0; i < ObjectArr.size(); i++)
        ObjectArr[i]->Start();
    started = true;
}


void State::UpdateArray(float dt)
{
    for(auto it : arrays)
        UpdateVector(*it, dt);
}

void State::UpdateVector( std::vector<std::shared_ptr<GameObject>>& ObjectArr, float dt)
{
    
    for(unsigned i = 0; i < ObjectArr.size(); i++)
    {
        ObjectArr[i]->Update(dt);
        ObjectArr[i]->UpdateNodes(dt);
    }
}

void State::RenderVector(std::vector<std::shared_ptr<GameObject>>& ObjectArray)
{
    for(unsigned i = 0; i < ObjectArray.size(); i++)
    {
        ObjectArray[i]->Render();
        ObjectArray[i]->RenderNodes();
    }
}

std::vector<std::vector<std::shared_ptr<GameObject>>*> State::GetArrays()
{
    return {&objectArray,&rigidArray,&enemyArray, &terrainArray, &bulletArray, &cameraFollowerObjectArray};
}

void State::RenderArray()
{
    for(auto it : arrays)
        RenderVector(*it);
}

void State::KillVector(std::vector<std::shared_ptr<GameObject>>& ObjectArray)
{
    for(unsigned i = 0; i < ObjectArray.size(); i++)
    {
        if(ObjectArray[i]->IsDead())
        {
            // std::cout << ObjectArray.size() << ' ';
            ObjectArray.erase(ObjectArray.begin() + i);
            // std::cout << ObjectArray.size() << '\n';
            i--;
        }
    }
}

void State::KillDeads()
{
    for(auto it : arrays)
        KillVector(*it);
}

std::weak_ptr<GameObject> State::AddObject(GameObject* object, int idx){
    auto objArr = arrays[idx];
    std::shared_ptr<GameObject> shrd_obj(object);
    objArr->push_back(shrd_obj);
    if(started)
        shrd_obj->Start();
    return std::weak_ptr<GameObject>(shrd_obj);
}

void State::Collision(){
    
}
std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* object){
    for(auto objarr : arrays)
        for(unsigned i = 0; i < objarr->size(); i++)
            if((*objarr)[i].get() == object)
                return std::weak_ptr<GameObject>((*objarr)[i]);
    return std::weak_ptr<GameObject>();
}

std::weak_ptr<GameObject> State::GetObject(std::function<bool(GameObject&)> fnc)
{
    for(auto it : arrays)
        for(auto& jt : *it)
            if(fnc(*jt.get()))
                return std::weak_ptr<GameObject>(jt);
    return std::weak_ptr<GameObject>();
}

std::weak_ptr<GameObject> State::GetObject(C_ID type)
{
    for(auto it : arrays)
        for(auto& jt : *it)
            if(jt->GetComponent(type))
                return std::weak_ptr<GameObject>(jt);
    return std::weak_ptr<GameObject>();
}

std::weak_ptr<GameObject> State::GetObject(C_ID type, std::vector<std::shared_ptr<GameObject>>* ObjArr)
{
    for(auto& jt : *ObjArr)
        if(jt->GetComponent(type))
            return std::weak_ptr<GameObject>(jt);
    return std::weak_ptr<GameObject>();
}

bool State::PopRequested(){
    return popRequested;
}

bool State::QuitRequested(){
    return quitRequested;
}