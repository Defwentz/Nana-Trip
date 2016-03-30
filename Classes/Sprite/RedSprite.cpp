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
    if (sprite && sprite->initWithFile("black.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
RedSprite::RedSprite() {
    float duration = random(6, 7);
    if(boolWithOdds(0.01)) {
        duration = 0.5;
    }
    this->runAction(RepeatForever::create(RotateBy::create(duration, 360)));
}

void RedSprite::setup(b2World *world, b2CircleShape *shape, int type) {
    this->setScale(shape->m_radius*PTM_RATIO/36.0);
    b2Body *body;
    if(type == _chasing) {
        body = Box2DHelper::getBallWithShape(world, shape, b2_kinematicBody);
        b2Filter filter = b2Filter();
        filter.groupIndex = -1;
        body->GetFixtureList()->SetFilterData(filter);
    } else {
        body = Box2DHelper::getBallWithShape(world, shape, b2_dynamicBody);
    }
    body->SetGravityScale(0);
    body->SetUserData(new Entity(UD_BADGUY));
    if(type == _moving) {
        body->ApplyForce(b2Vec2(random(-30, 30), random(-30, 30)), body->GetPosition(), true);
    }else if (type == _chasing) {
        body->SetLinearVelocity(b2Vec2(0, -6));
    }
        
    this->_body = body;
    this->_type = type;
}
void RedSprite::update() {
    b2CircleShape *shape = (b2CircleShape *) _body->GetFixtureList()->GetShape();
    this->setPosition(b2ToV(shape->m_p + _body->GetPosition()));
    
    for(b2ContactEdge *ce = _body->GetContactList(); ce; ce = ce->next) {
        b2Body *other = ce->other;
        auto other_userdata = (Entity *) other->GetUserData();
        if(other_userdata &&
           ce->contact->IsTouching() &&
           other_userdata->type == UD_NANA) {
            gameStatus = GAME_OVER;
            
            auto self_userdata = (Entity *) _body->GetUserData();
            if(self_userdata->type != UD_BADBOSS) {
                JavaOCer::reportAchievement(100.f, "nana_gameover_first");
                JavaOCer::showAchievementNotification("碰到了小红", "碰到了小红..挂掉了", "nana_gameover_first");
                return;
            }
            log("killed by bad boss");
            return;
        }
    }
}
