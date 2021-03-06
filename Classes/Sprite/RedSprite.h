//
//  RedSprite.h
//  nanatrip
//
//  Created by Macbook Air on 10/24/15.
//
//

#ifndef __nanatrip__RedSprite__
#define __nanatrip__RedSprite__

#include "NanaTrip.h"
#include "SpriteWithBody.h"

class RedSprite : public SpriteWithBody
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
    
    void update();
};

#endif /* defined(__nanatrip__RedSprite__) */
