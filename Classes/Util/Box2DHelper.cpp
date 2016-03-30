//
//  Box2DHelper.cpp
//  nanatrip
//
//  Created by Macbook Air on 10/24/15.
//
//

#include "Box2DHelper.h"
b2Body *Box2DHelper::getStaticBall(b2World *_world, b2Vec2 pos, float radius) {
    // setup the shape
    b2CircleShape shape;
    shape.m_p = pos;
    shape.m_radius = radius;
    
    return getBallWithShape(_world, &shape, b2_staticBody);
}
b2Body *Box2DHelper::getBallWithShape(b2World *_world, b2CircleShape *shape, b2BodyType type) {
    // define the b2body
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = type;
    b2Body *b = _world->CreateBody(&bd);
    b->CreateFixture(shape, 0);
    return b;
}