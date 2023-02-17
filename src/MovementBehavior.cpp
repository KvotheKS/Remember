#include "MovementBehavior.h"

MovementBehavior::MovementBehavior(GameObject& associated, float speed , float duration , int type ):GameObject(associated){
    this->speed = speed;
    this->duration = duration;
    this->type = type;
    fase = 0;
    timer.SetFinish(duration);

}
MovementBehavior::~MovementBehavior(){
    
}
void MovementBehavior::Update(float dt){
    if(timer.Update(dt)){
        fase = !fase;
        timer.Restart();
    }

    switch(type){
        case 0:

            if(fase){

                associated.box.x -= speed*dt;
               
            }else{
                associated.box.x += speed*dt;
            }
        break;
        case 1:
            associated.angleDeg += speed*dt;
        break;
    }
}

void MovementBehavior::Render(){

}
bool MovementBehavior::Is(std::string type){

}
bool MovementBehavior::Is(C_ID){

}