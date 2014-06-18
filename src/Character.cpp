#include "Character.h"

void Character::jump(){
    //Vec2f j = Vec2f(0, 20);
    setVelocity(velocity+Vec2f(0,20));
}

void Character::use(){
    //perhaps play an animation like 'shrug' to show that there isn't anything nearby to use
}

void Character::use(Entity e){
    Type t = e.getType();

    if(t==BOY){

    }else if(t==GIRL){

    }else if(t==DOG){

    }else if(t==ENEMY){

    }else if(t==WALL){

    }else if(t==GROUND){

    }else if(t==OTHER){

    }
}

int Character::getHealth(){
    return health;
}

Vec2f Character::getVelocity(){
    return velocity;
}

void Character::setVelocity(Vec2f newVelocity){
    velocity.x = newVelocity.x;
    velocity.y = newVelocity.y;
}

void Character::setVelocity(float newX, float newY){
    velocity.x = newX;
    velocity.y = newY;
}

void Character::setHealth(int h){
    health = h;
}

