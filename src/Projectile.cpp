#include "Projectile.h"

/*
    Função recebe 14 argumentos:
        rotSprt -> Se o sprite do projetil será rotacionado a medida que ele viaja
        angle -> ângulo em graus da direção do projetil
        initialSpeed -> Velocidade inicial do projetil
        homingRadius -> Raio de atração
        homingAccMax -> Aceleração máxima que do homing
        prftHmg -> Se vai ser um homing perfeito
        accHmg -> Se o projetil vai acelerando ao medida que se aproxima do centro
        acceleration -> aceleração extra
*/
Projectile::Projectile(GameObject& associated, float lifeTime, GameObject* target, bool rotSprt,
                float angle, float initialSpeed, float maxSpeed, float gravity, float homingRadius,
                float homingAccMax, bool prftHmg, bool accHmg, float pace) : GameObject(associated){
    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
    // associated.angleDeg = angle;
    this->target = target;
    this->rotSprt = rotSprt;

    // Gravidade sempre para "baixo"
    this->gravity = Vec2(0, 1) * gravity;

    lifeTimeCount.Restart(); lifeTimeCount.SetFinish(lifeTime);

    float radAngle = angle * PI / 180.0;
    this->maxSpeed = maxSpeed;
    // Rotaciona a velocidade para direção correta
    this->velocity = Vec2(1, 0).Rotate(radAngle) * initialSpeed;
    this->pace = pace;

    this->homingRadius = homingRadius;
    this->homingAccMax = homingAccMax;

    this->prftHmg = prftHmg;
    this->accHmg  = accHmg;
}

Projectile::Projectile(GameObject& associated, float lifeTime, float angle, float initialSpeed,
                float maxSpeed , float gravity, float pace)
    : Projectile(associated, lifeTime, nullptr, false, angle, initialSpeed, maxSpeed, gravity, 0.0f, 0.0f, false, false, pace)
{}
        
Projectile::Projectile(GameObject& associated, float lifeTime, GameObject* target, float angle, float initialSpeed, 
        float maxSpeed , float homingRadius,
            float homingAccMax, bool accelerated, float gravity, float pace)
    : Projectile(associated, lifeTime, target, false, angle, initialSpeed, maxSpeed, gravity, homingRadius, homingAccMax, false, accelerated, pace)
{}

void Projectile::Update(float dt){
    // std::cout << lifeTimeCount.Get()<< endl;
    // std::cout << lifeTimeCount.GetFinish()<< endl;
    if(lifeTimeCount.Update(dt))
    {
    
        associated.RequestDelete();
    }
    else{
        // std::cout << associated.box;
        Vec2 oldVelocity = velocity;
        Vec2 position = associated.box.GetCenter();

        if(homingRadius > 0.0){
            Vec2 target = this->target->box.GetCenter(); // Placeholder para o target real
            float dist = position.Distance(target);
            if(homingRadius > dist){ //Dentro da área do homing
                float magni;
                // Aceleração que altera -> (Raio - Distância) / Raio = Valor entre 0 e 1
                if(accHmg) magni = ((homingRadius - dist) / homingRadius) * homingAccMax;
                // Perfeito -> Velocidade Constante
                // else if(prftHmg) magni = velocity.Magnitude();
                // Aceleração Constante
                else magni = homingAccMax;

                // Aceleração para o centro da área do homing
                Vec2 homingForce = (target - position).Normalize() * magni * dt;
                // Se perfeito, a velocidade é recalculada
                // if(prftHmg){ velocity = homingForce; std::cout << velocity.Magnitude() << ' ' << velocity.AngleX() << '\n';}
                // Se não for, soma-se a aceleração na velocidade
                velocity += homingForce;
            }
        }
        velocity = velocity.Normalize() * velocity.Magnitude() * pace;
        velocity += gravity * dt;
        // Limita a velocidade
        if(velocity.Magnitude() > maxSpeed)
            velocity = velocity.Normalize() * maxSpeed;

        // position += velocity * dt;
        associated.box+=velocity * dt;
        // std::cout << associated.box;
        // std::cout << associated.box;
        // std::cout << velocity.Magnitude() * dt << '\n';
        // associated.box.SetCenter(position.x, position.y);

        // Rotaciona o Sprite pela direção da velocidade
        if(rotSprt){
            float product = velocity.Dot(oldVelocity);
            float a = velocity.Magnitude();
            float b = oldVelocity.Magnitude();
            // Angulo entre dois vetores: A.B / (mag(A) * mag(B))
            if(a > 0.0 && b > 0.0){
                float degAngle = std::acos(product / (a * b)) * 180.0 / PI;
                // Se o cross product for negativo o vetor foi no sentido horário no circulo trigonometrico,
                // positivo é anti-horário
                // Se estiver trocado, só trocar += por -= e vice-versa
                if(oldVelocity.Cross(velocity) > 0)
                    associated.angleDeg += degAngle;
                else
                    associated.angleDeg -= degAngle;
            }
        }
    }
}

void Projectile::Render(){
}

bool Projectile::Is(C_ID type)
{ return type == C_ID::Projectile; }

bool Projectile::Is(std::string type){
    return type == "ProjectileF";
}

int Projectile::GetDamage(){
    return damage;
}

void Projectile::SetMaxSpeed(float maxSpeed){
    this->maxSpeed = maxSpeed;
}