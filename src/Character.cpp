#include "Character.h"

Character::~Character() {
    //use some global instance of b2World
    //world.DestroyBody(body);
}

void Character::jump(){
    if(Entity::getBody()->GetLinearVelocity().y < 0.1 && Entity::getBody()->GetLinearVelocity().y > -0.1){
        Entity::getBody()->SetLinearVelocity(b2Vec2(Entity::getBody()->GetLinearVelocity().x,5));
    }
}

void Character::moveLeft(){
    Entity::getBody()->SetLinearVelocity(b2Vec2(-5,Entity::getBody()->GetLinearVelocity().y));
}

void Character::moveRight(){
    Entity::getBody()->SetLinearVelocity(b2Vec2(5,Entity::getBody()->GetLinearVelocity().y));
}

void Character::stop(){
    Entity::getBody()->SetLinearVelocity(b2Vec2(0,Entity::getBody()->GetLinearVelocity().y));
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

