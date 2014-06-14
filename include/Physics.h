#ifndef PHYSICS_H
#define PHYSICS_H

//Handles physics simulation
//Link library: Box2D
#include "Box2D/Box2D.h"

namespace Physics {

b2Body* createStaticBox(float cx, float cy, float w, float h, b2World &world);

b2Body* createDynamicBox(float cx, float cy, float w, float h, float density, b2World &world);

} //namespace Physics

#endif // PHYSICS_H
