#include "Collision.h"
#include <vector>
#include <iostream>

// define pra test -m
using namespace std;
#define p(x) cout << #x << ": " << x << endl;


bool Collision::IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB){
            
            Vec2 A[] = {Vec2(a.x, a.y + a.h), Vec2(a.x + a.w, a.y + a.h), Vec2(a.x + a.w, a.y), Vec2(a.x, a.y)};
            Vec2 B[] = {Vec2(b.x, b.y + b.h), Vec2(b.x + b.w, b.y + b.h), Vec2(b.x + b.w, b.y), Vec2(b.x, b.y)};

            for(auto& v : A)
                v = (v - a.GetCenter()).Rotate(angleOfA) + a.GetCenter();
            for(auto& v : B)
                v = (v - b.GetCenter()).Rotate(angleOfB) + b.GetCenter();

            Vec2 axes[] = {(A[0] - A[1]).Normalize(), (A[1] - A[2]).Normalize(),
                           (B[0] - B[1]).Normalize(), (B[1] - B[2]).Normalize()};

            
            bool collide = true;
            float sep = std::numeric_limits<float>::max();

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
                sep = min(sep, maxA-minB);
                sep = min(sep, maxB-minA);

                //pegar vetor de separacao.
                if(sep_vector.Magnitude() > (axis*sep).Magnitude()){
                    sep_vector = (axis*sep);
                }
                

                // diferente da função base precisamos olhar todos os vertices pra pegar o vetor de distancia minima -m
                if(maxA < minB || minA > maxB){
                    collide = false;
                    // return false;
                }  
            }
          
            // p(sep)
            // cout << "sep_vector.x" << ": " << sep_vector.x ;
            // p(sep_vector.y)

            return collide;
        }