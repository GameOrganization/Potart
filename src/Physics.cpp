#include "Physics.h"

b2Body* Physics::createStaticBox(float cx, float cy, float w, float h, b2World &world) {
    b2BodyDef def;
    def.position.Set(cx, cy);
    b2Body* body = world.CreateBody(&def);
    b2PolygonShape shape;
    shape.SetAsBox(w/2.0f, h/2.0f);
    body->CreateFixture(&shape, 0.0f);
    return body;
}

b2Body* Physics::createDynamicBox(float cx, float cy, float w, float h, float density, b2World &world) {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(cx, cy);
    b2Body* body = world.CreateBody(&def);
    b2PolygonShape shape;
    shape.SetAsBox(w/2.0f, h/2.0f);
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = density;
    //fixture.friction = 0.8f;
    body->CreateFixture(&fixture);
    return body;
}

