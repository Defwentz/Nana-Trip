//
//  MoverSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 9/12/15.
//
//

#include "MoverSprite.h"

MoverSprite* MoverSprite::create(uint32 flags)
{
    MoverSprite *sprite = new (std::nothrow) MoverSprite();
    
    sprite->_flags = flags;
    std::string filename;
    if(flags & _randomBit) {
        filename = res_mover[0];
        if(boolWithOdds(0.5))
            flags += _motorBit;
        // randomly generate flags -> mover
    }
    else if(flags & _normalBit) {
        filename = res_mover[0];
    }
    
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void MoverSprite::setup(b2World *world, b2Body *body, const cocos2d::Vec2 &p, float radius) {
    float round_edge_radius = 25.0/PTM_RATIO;
    
    this->setScale(radius*PTM_RATIO/216.5, 25/99.5);
    this->setPosition(p);
    
    b2BodyDef bd;
    bd.position = vToB2(p);
    bd.type = b2_dynamicBody;
    b2Body *mover = world->CreateBody(&bd);
    
    b2PolygonShape stick_1;
    b2Vec2 vertices[4] = {
        b2Vec2(round_edge_radius - radius, -round_edge_radius),
        b2Vec2(round_edge_radius - radius, round_edge_radius),
        b2Vec2(radius - round_edge_radius, round_edge_radius),
        b2Vec2(radius - round_edge_radius, -round_edge_radius)
    };
    stick_1.Set(vertices, 4);
    mover->CreateFixture(&stick_1, 0.2f);   // trying something
    {
        b2CircleShape round_edge;
        round_edge.m_p.Set(round_edge_radius - radius, 0);
        round_edge.m_radius = round_edge_radius;
        mover->CreateFixture(&round_edge, 0.2f);
    }
    {
        b2CircleShape round_edge;
        round_edge.m_p.Set(radius - round_edge_radius, 0);
        round_edge.m_radius = round_edge_radius;
        mover->CreateFixture(&round_edge, 0.2f);
    }
    b2RevoluteJointDef jd;
    jd.Initialize(body, mover, vToB2(p));
    if(_flags & _motorBit) {
        // TODO: set motor, or have it relevent to size
        jd.maxMotorTorque = 3.0f;
        jd.motorSpeed = 3.0f;
        jd.enableMotor = true;
    }
    _joint = world->CreateJoint(&jd);
    _body = mover;
}

void MoverSprite::selfDestruct(b2World *world) {
    world->DestroyJoint(_joint);
    SpriteWithBody::selfDestruct(world);
}

void MoverSprite::update() {
    this->setRotation(-_body->GetAngle()*180/M_PI);
}