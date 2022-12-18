#include "RigidBody.h"
#include "TerrainBody.h"
#include <iostream>
#include "Game.h"
// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x <<" ";



RigidBody::RigidBody(GameObject& associated, int modo):GameObject(associated), modo(modo){
    Sprite* pbody = new Sprite(associated, "assets/img/Zidle.png");
    associated.AddComponent(pbody);
    pbody->SetScaleX(2,2);
    Collider* collider = new Collider(associated);
    collider->SetScale(Vec2(1,1));
    collider->SetOffset(Vec2(0,0));
    
    associated.AddComponent(collider);

    speed =  Vec2(0,0);
    oldbox = Vec2(0,0);
    isGrounded = false;
    hasDoubleJump = true;
    inputDone = false;


    JUMP_TIMER = 0.2;
    // jumpTimer.Update(0);
    MAX_GLOBAL_SPEED = 3000;

    JUMP_FORCE = 1200;
    MAX_FALL_SPEED = 800;
    FALL_ACCELERATION = 100;

    MAX_MOVE_SPEED = 600;
    MOVE_ACCELERATION = 100;
    LATERAL_FRICTION = 80;
    LATERAL_SPEED_THRESHOLD = 100;

}
RigidBody::~RigidBody(){

}

void RigidBody::Start(){

}
void RigidBody::Update(float dt){


    Animation(dt);
    Controls(dt);
    Physics(dt);//!!check weird hitbox when this is off -m


    isGrounded = false;
    inputDone = false;
    
   
}
void RigidBody::Render(){

}
bool RigidBody::Is(std::string type){
    return type == "RigidBody";
}


void RigidBody::NotifyCollision(GameObject& other,Vec2 sep){
    
    Collider * ass_collider = (Collider*)associated.GetComponent("Collider");
    Collider * terrain_collider = (Collider*)other.GetComponent("Collider");
    auto GoUp = [&](float d){
        associated.box.y -= d;
    };
    auto GoDown = [&](float d){
        associated.box.y += d;
    };
    auto GoLeft = [&](float d){
        associated.box.x -= d;
    };
    auto GoRight = [&](float d){
        associated.box.x += d;
    };
    if(TerrainBody * terrain = (TerrainBody*)other.GetComponent("TerrainBody")){
        // Lidar com Contato em cima -m
        Vec2 top = (Vec2(0,-1).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.h/2) + terrain_collider->box.GetCenter();
        Vec2 right = (Vec2(1,0).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.w/2)+ terrain_collider->box.GetCenter();
        Vec2 down = (Vec2(0,1).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.h/2)+ terrain_collider->box.GetCenter();
        Vec2 left = (Vec2(-1,0).Rotate(other.angleDeg*PI/180))*(terrain_collider->box.w/2)+ terrain_collider->box.GetCenter();
       
        
        
       
        vector<float> distances;
        
        distances.push_back(top.Distance(ass_collider->box.GetCenter()));
        distances.push_back(right.Distance(ass_collider->box.GetCenter()));
        distances.push_back(down.Distance(ass_collider->box.GetCenter()));
        distances.push_back(left.Distance(ass_collider->box.GetCenter()));

        
        // for(auto dis : distances){
        //     cout << dis << " - ";
        // }
        
        min_element(distances.begin(), distances.end());
        auto it = std::min_element(std::begin(distances), std::end(distances));
        int idx = std::distance(std::begin(distances), it);


        int ang = (int)other.angleDeg % 360;
        ang = (ang+45)/90;
        idx += ang;

        idx = idx%4;
        
        float C = abs(180 - ((int)other.angleDeg%45) - 90) ;
        float b = sin(C*PI/180);
        if(b == 0){
            b =  sep.Magnitude();
        }else{
            b = sin(90*PI/180) * sep.Magnitude()/b;
        }
        p(C)

        // float largura_tranversal_terreno = sqrt(pow((terrain_collider->box.h/2),2)*2);
        // float height = largura_tranversal_terreno * sin((45-other.angleDeg)*PI/180)/sin(90*PI/180);
        if((((int)other.angleDeg+45)%90)==0){
            if(idx == 0 || idx == 1){
             
                cout << "up1\n";
                GoUp(b);
                
                isGrounded = true; 
                speed.y =0;
            }else{
                GoDown(b);
                cout << "down1\n";
            }
        }
        else if(idx == 0){
            cout << "up\n";
            GoUp(b);
                
            isGrounded = true; 
            speed.y =0;
        }
        else if(idx == 1){
            GoRight(b);
            cout << "right\n";
        }
        else if(idx == 2){
            GoDown(b);
            cout << "down\n";
            
        }
        else{
            GoLeft(b);
            cout << "left\n";
        }
     



        // // Lidar com Contato na Direita -m
        // if (associated.box.GetCenter().x > other.box.GetCenter().x){
            

        //     if(abs(sep.x) >= abs(sep.y)){
                
        //         float C = abs(180 - other.angleDeg - 90) ;
        //         if(C == 0){
        //             associated.box.x += sep.x;
                    
        //         }else{
        //             float b = sin(90*PI/180) * sep.Magnitude() /sin(C*PI/180);

                 
                    
        //             float largura_tranversal_terreno = sqrt(pow((terrain_collider->box.h/2),2)*2);
        //             float height = largura_tranversal_terreno * sin((45-other.angleDeg)*PI/180)/sin(90*PI/180);
                
        //             if(( (other.box.GetCenter().y -height) > associated.box.GetCenter().y + ass_collider->box.h/2*ass_collider->GetScale().y)){
                                      
        //                 GoUp(b);
        //                 cout << "up\n";
        //                 isGrounded = true; 
        //                 speed.y =0;
        //             }
        //             else if(other.box.GetCenter().y < associated.box.GetCenter().y - ass_collider->box.h/2*ass_collider->GetScale().y ){
        //                 GoDown(b);
        //                 cout<< "down\n";
        //             }
        //             else{
        //                 GoRight(sep.x);
        //                 cout<< "side\n";
        //             }
                    
        //         }
        //     }
            
           
        // }

        // // // Lidar com Contato na Esquerda -m
        // // else{
        // //     if(abs(sep.x) >= abs(sep.y)){
        // //         float C = abs(180 - other.angleDeg - 90) ;
        // //         if(C == 0){
        // //             associated.box.x -= sep.x;
                    
        // //         }else{
        // //             float b = sin(90*PI/180) * sep.Magnitude() /sin(C*PI/180);
        // //             if(other.angleDeg==45 && (associated.box.GetCenter().y < other.box.GetCenter().y)){
        // //                 associated.box.y -= b;
        // //                 isGrounded = true; 
        // //             }else if(other.angleDeg>45){
        // //                 associated.box.x += b;
        // //             }else{
        // //                 associated.box.x -= b;
        // //             }
        // //         }
        // //     }
            
        // // }


        // // Lidar com Contato no Topo -m
        // if(associated.box.GetCenter().y < other.box.GetCenter().y){
        //     float C2 = 180 - (45-associated.angleDeg) - 90;
        //     float b2 = sin((45-associated.angleDeg)*PI/180) * sep.x /sin(C2*PI/180);

        //     float largura_tranversal_terreno = sqrt(pow((terrain_collider->box.h/2),2)*2);
        //     float height = largura_tranversal_terreno * sin((45-other.angleDeg)*PI/180)/sin(90*PI/180);
        //     if((abs(sep.y)> abs(sep.x)) && (associated.box.GetCenter().y - height <  associated.box.GetCenter().y + ass_collider->box.h/2*ass_collider->GetScale().y)){
        //         cout << "it was me \n";
        //         isGrounded = true; 
                
        //         float C = abs(180 - other.angleDeg - 90) ;
                
        //         if(C == 0  ){//cubo de ponta cabeca
        //             GoDown(sep.y);
                    
        //         }else{
        //             float b = sin(90*PI/180) * sep.Magnitude() /sin(C*PI/180);
        //             GoUp(b);
                    
        //         }
                
                
        //         // associated.box.y += sep.y;
        //         speed.y = 0;
        //     }
            

        // }
        // // Lidar com Contato a baixo -m
        // else{

        //     // associated.box.y -= sep.y;
            
        // }
        Camera::Update(0);
        if(Collider* collider = (Collider*)associated.GetComponent("Collider")){
            collider->Update(0);
        }
            
        }
        else{
           
        }
}

void RigidBody::Animation(float dt){
    Sprite * spr = (Sprite*) associated.GetComponent(SPRITE_T);
    
    if(speed.y < 0 && !isGrounded){
        spr->Open("assets/img/Zjump.png");
    }
    if(speed.y > 2 && !isGrounded){
        spr->Open("assets/img/Zfall.png");
    }
    if(speed.x != 0 && isGrounded){   
        spr->Open("assets/img/Zrun.png");
    }
    if(speed.x == 0 && isGrounded){
        spr->Open("assets/img/Zidle.png");
        speed.x = 0;
    }
    if(speed.x < 0){
        spr->SetFliped(true);
    }else if (speed.x > 0){
        spr->SetFliped(false);
    }
}

void RigidBody::Controls(float dt){
    // p(jumpTimer.Get())cout << endl;
    jumpTimer.Update(dt);
    InputManager& inManager = InputManager::GetInstance();
    if(inManager.IsKeyDown(W_KEY)){  
        if (isGrounded ){
            jumpTimer.Restart();
            isGrounded = false;
            Jump(dt);
        }else if (jumpTimer.Get()<JUMP_TIMER){
            
            Jump(dt);
        }
    }else{
        jumpTimer.Update(JUMP_TIMER);
    }

    if(inManager.IsKeyDown(S_KEY)){
        
    }
    if(inManager.IsKeyDown(A_KEY)){
        speed.x -= MOVE_ACCELERATION*dt;
        inputDone = true;
    }
    if(inManager.IsKeyDown(D_KEY)){
       
        speed.x += MOVE_ACCELERATION*dt;
        inputDone = true;
        
    }
    
    if(inManager.MousePress(LEFT_MOUSE_BUTTON) && modo == 1){
        associated.box.x = inManager.GetMouseX() + Camera::pos.x;
        associated.box.y = inManager.GetMouseY() + Camera::pos.y;
        speed = Vec2(0,0);
    }

    if(inManager.MousePress(RIGHT_MOUSE_BUTTON) && modo == 1){
        associated.box.x = 0;
        associated.box.y = 0;
       
        
    }
}

void RigidBody::Physics(float dt){
      
    // Queda 
    speed.y += FALL_ACCELERATION*dt; 

    // desacelerar
    if(!inputDone){   
        if(abs(speed.x) < LATERAL_SPEED_THRESHOLD*dt)
            speed.x = 0;
        else if(speed.x > 0){
            speed.x = speed.x - LATERAL_FRICTION*dt; 
        }else{
            speed.x = speed.x + LATERAL_FRICTION*dt; 
        }
    }

    // // limitar velocidade global
    // if(speed.Magnitude() > MAX_GLOBAL_SPEED*dt){         
    //     speed = speed.Normalize()*MAX_GLOBAL_SPEED*dt;          
    // }

    //limitar velocidade lateral
    if(abs(speed.x) > MAX_MOVE_SPEED*dt){ 
        speed = Vec2((speed.x/abs(speed.x))*MAX_MOVE_SPEED*dt, speed.y );          
    }

    //limitar velocidade de queda
    if(speed.y > MAX_FALL_SPEED*dt){         
        speed.y =  MAX_FALL_SPEED*dt;          
    }

     
    // mova-se de acordo com a velocidade 
    if(isGrounded) speed.y = 0;

    Vec2 center = Vec2(associated.box.GetCenter() + speed);
    associated.box.SetCenter(center.x,center.y);

}
void RigidBody::MoveonTopof(GameObject& target){

}

void RigidBody::Jump (float dt){
    speed.y = -JUMP_FORCE*dt;
}

Vec2 RigidBody::Bcurve(Vec2 a ,Vec2 b, Vec2 c, Vec2 d,float t) {
    return Vec2( std::pow((1 - t), 3) * a.x + 3 * std::pow((1 - t), 2) * t * b.x + 3 * std::pow((1 - t), 1) * std::pow(t, 2) * c.x + std::pow(t, 3) * d.x
                ,std::pow((1 - t), 3) * a.y + 3 * std::pow((1 - t), 2) * t * b.y + 3 * std::pow((1 - t), 1) * std::pow(t, 2) * c.y + std::pow(t, 3) * d.y);
}

Vec2 RigidBody::Bcurve(std::vector<Vec2> vec, float t) {
    
   
    auto fac = [](const int n){
        int ret = n;
        if(n == 0) return 1;
        for(int i = n-1; i>0 ;i--)ret = ret * i;
        return ret;
    };

    int n = vec.size();
    float x,y = 0;
    
    for (int k = 0; k<n; k++){
        x += fac(n)/fac(k)*fac(n-k) * std::pow((1-t),(n-k)) * std::pow((t),(k)) * vec[k].x;
        y += fac(n)/fac(k)*fac(n-k) * std::pow((1-t),(n-k)) * std::pow((t),(k)) * vec[k].y;
    }

    return Vec2(x,y);

}

