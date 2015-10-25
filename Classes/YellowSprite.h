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

class RedSprite : public cocos2d::Sprite
{
    int _type;
public:
    enum
    {
        _static             = 0,
        _moving				= 1,	///< moving red
        _chasing            = 2
    };
    
    static RedSprite *create();
    RedSprite();
    void setup(b2World *world, b2CircleShape *shape, int type);
    
    void update(float dt);
};

#endif /* defined(__nanatrip__YellowSprite__) */
