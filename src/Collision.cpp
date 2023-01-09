#include "Collision.h"
#include <vector>
#include <iostream>


//pair( bool = se ha colisao , Vec2() vetor com a distancia minima entre os Rect) -m
pair<bool,Vec2> Collision::IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB){
    auto c_a = a.GetCenter(), c_b = b.GetCenter();
    {
        Vec2 s_a = Vec2(a.w,a.h)*0.5f, s_b = Vec2(b.w, b.h)*0.5f;
        if((c_a-c_b).Magnitude() > (s_a+s_b).Magnitude())
            return make_pair(false, Vec2::ZERO);
    }
    Vec2 A[] = {Vec2(a.x, a.y + a.h), Vec2(a.x + a.w, a.y + a.h), Vec2(a.x + a.w, a.y), Vec2(a.x, a.y)};
    Vec2 B[] = {Vec2(b.x, b.y + b.h), Vec2(b.x + b.w, b.y + b.h), Vec2(b.x + b.w, b.y), Vec2(b.x, b.y)};

    for(auto& v : A)
        v = (v - c_a).Rotate(angleOfA) + c_a;
    for(auto& v : B)
        v = (v - c_b).Rotate(angleOfB) + c_b;


    Vec2 axes[] = {(A[0] - A[1]).Normalize(), (A[1] - A[2]).Normalize(),
                   (B[0] - B[1]).Normalize(), (B[1] - B[2]).Normalize()};

    
    bool collide = true;
    float sep = std::numeric_limits<float>::lowest();

    Vec2 sep_vector = Vec2(std::numeric_limits<float>::max(),std::numeric_limits<float>::max());

    for(auto& axis : axes){
        float P[4];
        
        for(int i = 0; i < 4; i++) P[i] = A[i].Dot(axis);

        float minA = *std::min_element(P, P + 4);
        float maxA = *std::max_element(P, P + 4);

        for(int i = 0; i < 4; i++) P[i] = B[i].Dot(axis);

        float minB = *std::min_element(P, P + 4);
        float maxB = *std::max_element(P, P + 4);

        
        //pegar valor de separacao menor pra projecao do loop atual -m
      
        if(sep < minB-maxA){
            sep = minB-maxA;              
            sep_vector = (axis*sep);
        }

        if(sep < minA-maxB){
            sep = minA-maxB;       
            sep_vector = (axis*sep);
        }
      
        

        // diferente da função base precisamos olhar todos os vertices pra pegar o vetor de distancia minima -m
        if(maxA < minB || minA > maxB){
            collide = false;
            return make_pair(collide,sep_vector);
        }  
    }
    

    // cout << "sep:" << sep << " - Axis:" <<  " - ";
    // cout << "sep_vector" << ": (" << sep_vector.x << " ," << sep_vector.y<< ")\n";

    return make_pair(collide,sep_vector);
}