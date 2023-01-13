#include "State.h"

State::State(){
    popRequested = false;
    quitRequested = false;
    started = false;
}

State::~State(){
    objectArray.clear();
}

void State::StartArray()
{
    StartVector(objectArray);
    StartVector(rigidArray);
    StartVector(terrainArray);
    StartVector(bulletArray);
    StartVector(cameraFollowerObjectArray);
}

void State::StartVector(std::vector<std::shared_ptr<GameObject>>& ObjectArr){
    for(unsigned i = 0; i < ObjectArr.size(); i++)
        ObjectArr[i]->Start();
    started = true;
}


void State::UpdateArray(float dt)
{
    UpdateVector(objectArray, dt);
    UpdateVector(rigidArray, dt);
    UpdateVector(terrainArray, dt);
    UpdateVector(bulletArray, dt);
    UpdateVector(cameraFollowerObjectArray, dt);
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

void State::RenderArray()
{
    // std::cout << "1";
    RenderVector(objectArray);
    // std::cout << "2";
    RenderVector(rigidArray);
    // std::cout << "3";
    RenderVector(terrainArray);
    // std::cout << "4";
    RenderVector(bulletArray);
    // std::cout << "5";
    RenderVector(cameraFollowerObjectArray);
}

void State::KillVector(std::vector<std::shared_ptr<GameObject>>& ObjectArray)
{
    for(unsigned i = 0; i < ObjectArray.size(); i++)
    {
        if(ObjectArray[i]->IsDead())
        {
            ObjectArray.erase(ObjectArray.begin() + i);
            i--;
        }
    }
}

void State::KillDeads()
{
    KillVector(objectArray);
    KillVector(rigidArray);
    KillVector(terrainArray);
    KillVector(bulletArray);
    KillVector(cameraFollowerObjectArray);
}

std::weak_ptr<GameObject> State::AddObject(GameObject* object){
    std::shared_ptr<GameObject> shrd_obj(object);
    objectArray.push_back(shrd_obj);
    if(started)
        shrd_obj->Start();
    return std::weak_ptr<GameObject>(shrd_obj);
}

void State::Collision(){
    
}
std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* object){
    for(unsigned i = 0; i < objectArray.size(); i++)
        if(objectArray[i].get() == object)
            return std::weak_ptr<GameObject>(objectArray[i]);
    return std::weak_ptr<GameObject>();
}

bool State::PopRequested(){
    return popRequested;
}

bool State::QuitRequested(){
    return quitRequested;
}