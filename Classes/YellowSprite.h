//
//  YellowSprite.h
//  nanatrip
//
//  Created by Macbook Air on 10/24/15.
//
//

#ifndef __nanatrip__YellowSprite__
#define __nanatrip__YellowSprite__

#include "NanaTrip.h"

class YellowSprite : public cocos2d::Sprite
{
    cocos2d::CustomCommand _customCommand;
public:
    
    static YellowSprite *create();
    YellowSprite();
    void setup(b2World *world, b2CircleShape *shape, int type);
    
    void update(float dt);
    // draw
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
};

#endif /* defined(__nanatrip__YellowSprite__) */
