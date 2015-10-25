//
//  RedSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 10/24/15.
//
//

#include "RedSprite.h"

RedSprite* RedSprite::create()
{
    RedSprite *sprite = new (std::nothrow) RedSprite();
    if (sprite && sprite->initWithFile("red.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
RedSprite::RedSprite() {
    this->runAction(RepeatForever::create(RotateBy::create(7, 360)));
}
void RedSprite::selfDestruct(b2World *world) {
    
}

void RedSprite::setup(b2World *world, b2CircleShape *shape, int type) {
    this->setScale(shape->m_radius*PTM_RATIO/36);
    b2Body *body = Box2DHelper::getBallWithShape(world, shape, b2_dynamicBody);
    body->SetGravityScale(0);
    body->SetUserData(new Entity(UD_BADGUY));
    if(type == _moving)
        body->ApplyForce(b2Vec2(random(-30, 30), random(-30, 30)), body->GetPosition(), true);
    this->_body = body;
    this->_type = type;
}
void RedSprite::update(float dt) {
    b2CircleShape *shape = (b2CircleShape *) _body->GetFixtureList()->GetShape();
    this->setPosition(b2ToV(shape->m_p + _body->GetPosition()));
}



