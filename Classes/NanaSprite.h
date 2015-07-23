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
    cocos2d::Vec2 getPosition();
    void gasUp();
    void ApplyForce(b2Vec2 force);
    
    //void visit(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
        //override;
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
        override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    //static std::vector<b2Body *> createPhysicsObject_(b2World *world);
    //void bounce();
protected:
    int hardness;
    
    b2World *_world;
    std::vector<b2Body *> _bodies;
    
    //cocos2d::DrawNode *_drawNode;
    cocos2d::CustomCommand _customCommand;
};

#endif /* defined(__nanatrip__NanaSprite__) */
