#include "Entity.h"

Entity::Entity() : pos(), body(NULL){
    name = "UNNAMED";
    type=Type(OTHER);
    //currAnm = animations[0];
}

Entity::Entity(std::string n, Vec2f pos0, b2Body* body0, Type t) : pos(pos0), body(body0) {
    type = t;
    //currAnm = animations[0];
}

Entity::~Entity() {
    //use some global instance of b2World
    //world.DestroyBody(body);
}

void Entity::update(float time) {
    //assumes
    const b2Vec2& center = body->GetWorldCenter();
    pos.x = center.x;  //*conversionFactor
    pos.y = center.y;  //*conversionFactor
    //currAnm.update(time);
}

void Entity::setPos(float x, float y) {
    pos = Vec2f(x, y);
    body->SetTransform(b2Vec2(x, y), body->GetAngle());
}

Vec2f Entity::getPos() {
    return pos;
}

Type Entity::getType(){
    return type;
}

std::string Entity::getName(){
    return name;
}

void Entity::setName(std::string n){
    name = n;
}
