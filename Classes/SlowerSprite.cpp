//
//  SlowerSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 11/5/15.
//
//

#include "SlowerSprite.hpp"

SlowerSprite* SlowerSprite::create()
{
    SlowerSprite *sprite = new (std::nothrow) SlowerSprite();
    if (sprite && sprite->initWithFile("slower.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void SlowerSprite::setup(b2World *world, b2CircleShape *shape) {
    this->setPosition(b2ToV(shape->m_p));
    this->setScale(shape->m_radius*PTM_RATIO/121);
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    _body = world->CreateBody(&bd);
    b2Fixture *fxr = _body->CreateFixture(shape, 0);
    fxr->SetSensor(true);
}

void SlowerSprite::update() {
    for(b2ContactEdge *ce = _body->GetContactList(); ce; ce = ce->next) {
        _b2Contact *contact = (_b2Contact *)ce->contact;
        b2Body *other = ce->other;
        auto other_userdata = (Entity *) other->GetUserData();
        
        bool wasTouching = (contact->getm_flags() & _b2Contact::touchingFlag) == _b2Contact::touchingFlag;
        
        if(other_userdata && other_userdata->type == UD_NANA) {
            log("slower event: wasTouching - %d, touching - %d", wasTouching, contact->IsTouching());
            // contact begin
            if(wasTouching == false && contact->IsTouching()) {
                log("slower touch begin: set grav 0");
                other->SetGravityScale(0);
            }
            // contact end
            if(wasTouching == true && !contact->IsTouching()) {
                log("slower touch end: set grav 1");
                other->SetGravityScale(1);
            }
        }
    }
}