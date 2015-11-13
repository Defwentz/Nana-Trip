//
//  SlowerSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 11/5/15.
//
//

#include "SlowerSprite.hpp"

SlowerSprite* SlowerSprite::create(int type)
{
    SlowerSprite *sprite = new (std::nothrow) SlowerSprite();
    sprite->_type = type;
    std::string filename = "slower_2.png";
    if(type == _bouncy) {
        filename = "slower.png";
    }
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void SlowerSprite::setup(b2World *world, b2CircleShape *shape) {
    this->setPosition(b2ToV(shape->m_p));
    if(_type == _slow) {
        this->setScale(shape->m_radius*PTM_RATIO/61);
    }else if(_type == _bouncy) {
        this->setScale(shape->m_radius*PTM_RATIO/121);
    }
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    _body = world->CreateBody(&bd);
    b2Fixture *fxr = _body->CreateFixture(shape, 0);
    fxr->SetSensor(true);
}

void SlowerSprite::update() {
    for(b2ContactEdge *ce = _body->GetContactList(); ce; ce = ce->next) {
        b2Body *other = ce->other;
        auto other_userdata = (Entity *) other->GetUserData();
        
        if(other_userdata && other_userdata->type == UD_NANA && ce->contact->IsTouching()) {
            if(_type == _slow) {
                b2Vec2 v = other->GetLinearVelocity();
                other->SetLinearVelocity(b2Vec2(v.x*0.5, v.y*0.5));
            } else if (_type == _bouncy) {
                other->ApplyForceToCenter(b2Vec2(0, stdGrav.y*-1), true);
            }
        }
    }
}