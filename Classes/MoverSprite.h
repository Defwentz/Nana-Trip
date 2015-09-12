//
//  MoverSprite.h
//  nanatrip
//
//  Created by Macbook Air on 9/12/15.
//
//

#ifndef __nanatrip__MoverSprite__
#define __nanatrip__MoverSprite__

#include "NanaTrip.h"

// types:
// single rod
// single rod with motor
// double rod, like cross
// double rod, like cross, with motor
class MoverSprite : public cocos2d::Sprite
{
public:
    static MoverSprite *create(const std::string& filename);
    MoverSprite();
    void setup(b2World *world, b2Body *body, const cocos2d::Vec2& p, float radius);
    void selfDestruct(b2World *world);
    
    void update();
    
    b2Body *_body;
    b2Joint *_joint;
};

#endif /* defined(__nanatrip__MoverSprite__) */
