#include "Component.h"

Component::Component(GameObject& associated) : associated(associated){
}

Component::~Component(){
}

void Component::Start(){
}

void Component::NotifyCollision(GameObject& other,Vec2 sep){
}

void Component::Print(float x, float y){}