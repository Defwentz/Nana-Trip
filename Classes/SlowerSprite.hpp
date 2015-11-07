//
//  SlowerSprite.hpp
//  nanatrip
//
//  Created by Macbook Air on 11/5/15.
//
//

#ifndef SlowerSprite_hpp
#define SlowerSprite_hpp

#include "NanaTrip.h"
#include "SpriteWithBody.h"

// types:
// single rod
// single rod with motor
// double rod, like cross
// double rod, like cross, with motor
class SlowerSprite : public SpriteWithBody
{
public:
    enum
    {
        _slow             = 0,
        _bouncy             = 1
    };
    int _type;
    static SlowerSprite *create(int type);
    void setup(b2World *world, b2CircleShape *shape);
    
    void update();
};

#endif /* SlowerSprite_hpp */
