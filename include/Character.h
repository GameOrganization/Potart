#ifndef CHARACTER_H
#define CHARACTER_H

#include <Entity.h>

class Character : public Entity{

    //may or may not need if we implement combat
    int health;

    Vec2f velocity;

    public:
        //constructor / destructor
        Character() : Entity(){}
        Character(std::string n, Vec2f pos0, b2Body* body0, Type t) : Entity(n,pos0, body0, t){}
        virtual ~Character();

        //characters go boing boing
        //assuming all characters jump in the same arch
        void jump();

        //for all your basic movement needs
        void moveLeft();
        void moveRight();
        void stop();

        //virtual since every character could interact differently with other entities
        virtual void use();
        virtual void use(Entity e);

        int getHealth();
        void setHealth(int h);
        Vec2f getVelocity();

        //can pass in a vector or 2 numbers
        void setVelocity(const Vec2f newVelocity);
        void setVelocity(float newX, float newY);
};

#endif // CHARACTER_H
