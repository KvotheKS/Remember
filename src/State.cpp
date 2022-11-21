#include "State.h"

State::State(){
    popRequested = false;
    quitRequested = false;
    started = false;
}

State::~State(){
    objectArray.clear();
}

void State::StartArray(){
    for(unsigned i = 0; i < objectArray.size(); i++)
        objectArray[i]->Start();
    started = true;
}

void State::UpdateArray(float dt){
    for(unsigned i = 0; i < objectArray.size(); i++)
        objectArray[i]->Update(dt);
}

void State::RenderArray(){
    for(unsigned i = 0; i < objectArray.size(); i++)
        objectArray[i]->Render();
}

std::weak_ptr<GameObject> State::AddObject(GameObject* object){
    std::shared_ptr<GameObject> shrd_obj(object);
    objectArray.push_back(shrd_obj);
    if(started)
        shrd_obj->Start();
    return std::weak_ptr<GameObject>(shrd_obj);
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