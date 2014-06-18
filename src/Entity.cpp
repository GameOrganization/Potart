#include "Entity.h"

Entity::Entity() : pos(), body(NULL) {
    //currAnm = animations[0];
}

Entity::Entity(Vec2f pos0, b2Body* body0) : pos(pos0), body(body0) {
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
