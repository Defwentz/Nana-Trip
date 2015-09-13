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
    // scale for gasUp method, default 12
    // 10 is ok, 20 is a bit much, get stuck sometime
    int hardness = 12;
    bool enableUnbreakable = false;
    
    b2World *_world;
    
    cocos2d::Texture2D *_nanaFace;
    //cocos2d::DrawNode *_drawNode;
    cocos2d::CustomCommand _customCommand;
public:
    // the vec2 of _bodies[0]->getPosistion()
    cocos2d::Vec2 getPosition();
    cocos2d::Vec2 getCenter();
    bool isNana(b2Body *body);
    void gasUp();
    void ApplyForce(b2Vec2 force);
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
        override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    
    static NanaSprite *create(b2World *world);
    
    //static b2Body *createPhysicsObject(b2World *world);
    NanaSprite(b2World *world);
    ~NanaSprite();
    void initPhysics(b2World *world);
    
    std::vector<b2Body *> _bodies;
    std::vector<b2RevoluteJoint *> _joints;
};

#endif /* defined(__nanatrip__NanaSprite__) */
