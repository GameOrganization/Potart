#include "Entity.h"

Entity::Entity() : pos(), body(NULL){
    name = "UNNAMED";
    type=Type(OTHER);
    //currAnm = animations[0];
}

Entity::Entity(std::string n, Vec2f pos0, b2Body* body0, Type t) : pos(pos0), body(body0), type(t) {
    //currAnm = animations[0];
}

Entity::~Entity() {
    //use some global instance of b2World
    //world.DestroyBody(body);
}

void Entity::reset(float cx, float cy, float w, float h, float density, float fric, b2World &world){
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(cx, cy);
    body = world.CreateBody(&def);
    b2PolygonShape shape;
    shape.SetAsBox(w/2.0f, h/2.0f);
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = density;
    fixture.friction = fric;
    body->SetFixedRotation(true);
    body->CreateFixture(&fixture);
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

b2Body* Entity::getBody(){
    return body;
}
