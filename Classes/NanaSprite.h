//
//  NanaSprite.h
//  nanatrip
//
//  Created by Macbook Air on 7/19/15.
//
//

#ifndef __nanatrip__NanaSprite__
#define __nanatrip__NanaSprite__

#include "NanaTrip.h"

class NanaSprite : public cocos2d::Sprite
{
public:
    static NanaSprite *create();
    
    //static b2Body *createPhysicsObject(b2World *world);
    NanaSprite();
    
    void initPhysics(b2World *world);
    // the vec2 of _bodies[0]->getPosistion()
    cocos2d::Vec2 getPosition();
    bool isNana(b2Body *body);
    void gasUp();
    void ApplyForce(b2Vec2 force);
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
        override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
protected:
    // scale for gasUp method, default 12
    // 10 is ok, 20 is a bit much, get stuck sometime
    int hardness = 12;
    
    b2World *_world;
    std::vector<b2Body *> _bodies;
    
    //cocos2d::DrawNode *_drawNode;
    cocos2d::CustomCommand _customCommand;
};

#endif /* defined(__nanatrip__NanaSprite__) */
