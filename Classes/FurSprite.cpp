//
//  FurSprite.cpp
//  nana
//
//  Created by Macbook Air on 2/26/16.
//
//

#include "FurSprite.h"
FurSprite* FurSprite::create()
{
    FurSprite *sprite = new (std::nothrow) FurSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void FurSprite::setup(b2World *world, b2Vec2 root, int length, int isRight) {
    // __ __ __
    //   |  |  |
    // __|__|__|
    float width = length / PTM_RATIO / 3.;
    float hheight = 30. / PTM_RATIO / 2.;
    int sign = -2 * isRight + 1;
    
    for(int i = 0; i < 3; i++) {
        b2BodyDef bd;
        bd.position = root;
        if(i == 0) {
            bd.type = b2_staticBody;
        } else {
            bd.type = b2_dynamicBody;
        }
        b2Body *body = world->CreateBody(&bd);
        
        b2PolygonShape stick;
        b2Vec2 vertices[4] = {
            b2Vec2(sign*i*width, -hheight),
            b2Vec2(sign*i*width, hheight),
            b2Vec2(sign*(i*width+width), hheight),
            b2Vec2(sign*(i*width+width), -hheight)
        };
        stick.Set(vertices, 4);
        body->CreateFixture(&stick, 0.2f);   // trying something
        _bodies.push_back(body);
    }
    b2DistanceJointDef jointDef;
    for(int i = 1; i < 3; i++) {
        // Get the current body and the neighbor
        b2Body *currentBody = _bodies[i-1];
        b2Body *neighborBody = _bodies[i];
        
        b2Vec2 joint_1 = root + b2Vec2(sign*i*width, hheight);
        b2Vec2 joint_2 = root + b2Vec2(sign*i*width, -hheight);
        
        // Connect
        jointDef.Initialize(currentBody, neighborBody,
                            joint_1,
                            joint_1 );
        jointDef.collideConnected = true;
        jointDef.frequencyHz = 4.0f;
        jointDef.dampingRatio = 0.5f;
        
        _joints.push_back((b2DistanceJoint*)(world->CreateJoint(&jointDef)));

        jointDef.Initialize(currentBody, neighborBody,
                            joint_2,
                            joint_2 );
        
        _joints.push_back((b2DistanceJoint*)(world->CreateJoint(&jointDef)));
    }
    
    
//    float round_edge_radius_px = random(15, 20);
//    if(boolWithOdds(0.01)) {
//        round_edge_radius_px = 5;
//    }
//    float round_edge_radius = round_edge_radius_px/PTM_RATIO;
//    
//    this->setScale(radius*PTM_RATIO/216.5, round_edge_radius_px/59.5);
//    this->setPosition(p);
//    
//    b2BodyDef bd;
//    bd.position = vToB2(p);
//    bd.type = b2_dynamicBody;
//    b2Body *mover = world->CreateBody(&bd);
//    
//    b2PolygonShape stick_1;
//    b2Vec2 vertices[4] = {
//        b2Vec2(round_edge_radius - radius, -round_edge_radius),
//        b2Vec2(round_edge_radius - radius, round_edge_radius),
//        b2Vec2(radius - round_edge_radius, round_edge_radius),
//        b2Vec2(radius - round_edge_radius, -round_edge_radius)
//    };
//    stick_1.Set(vertices, 4);
//    mover->CreateFixture(&stick_1, 0.2f);   // trying something
//    {
//        b2CircleShape round_edge;
//        round_edge.m_p.Set(round_edge_radius - radius, 0);
//        round_edge.m_radius = round_edge_radius;
//        mover->CreateFixture(&round_edge, 0.2f);
//    }
//    {
//        b2CircleShape round_edge;
//        round_edge.m_p.Set(radius - round_edge_radius, 0);
//        round_edge.m_radius = round_edge_radius;
//        mover->CreateFixture(&round_edge, 0.2f);
//    }
//    b2RevoluteJointDef jd;
//    jd.Initialize(mother, mover, vToB2(p));
//    if(_flags & _motorBit) {
//        jd.maxMotorTorque = 3.5f;
//        jd.motorSpeed = 3.0f;
//        jd.enableMotor = true;
//    }
//    _joint = world->CreateJoint(&jd);
//    _body = mover;
}
FurSprite::~FurSprite(){}
void FurSprite::selfDestruct(b2World *world){
}
void FurSprite::update(){
}