#ifndef ENTITY_H
#define ENTITY_H

#include "Vec2f.h"
#include "Box2D/Box2D.h"
#include <string>

//we can add or remove as necessary
enum Type{BOY,GIRL,DOG,ENEMY,GROUND,WALL,OTHER};

class Entity {
    //current position of the body
    Vec2f pos;
    //name of the thing
    std::string name;
    //an array of the animations for this entity
    //if set to null, it is a non-animated object
//  Animation* animations;
//  Animation* currAnm = &animations[IDLE_ANIM];
    //animation class will probably have currentFrame, numFrames, some way to indicate where on a texture the first frame is, and an update function
public:

    Entity();
    Entity(std::string n, Vec2f pos, b2Body* body, Type t);//animations (defaults to 0), angle, etc
    virtual ~Entity();

    Type type;

    //call after updating b2d world, uses new position of the b2Body member
    void update(float time);

    void setPos(float x, float y);
    Vec2f getPos();

    Type getType();
    void setType(Type t);

    std::string getName();
    void setName(std::string n);

    //void setAnimation(int id); //possibly use string names for different animations stored in Animation class

protected:

    //float angle??
    b2Body* body;
};

#endif // ENTITY_H
