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
#include "SpriteWithBody.h"

// types:
// single rod
// single rod with motor
// double rod, like cross
// double rod, like cross, with motor
class MoverSprite : public SpriteWithBody
{
    uint32 _flags;
public:
    enum
    {
        _motorBit				= 0x0001,	///< mover with motor
        _normalBit				= 0x0002,	///< mover with single rod
        _randomBit				= 0x0004,	///< randomly generate mover
//        e_pairBit				= 0x0008,	///< draw broad-phase pairs
//        e_centerOfMassBit		= 0x0010	///< draw center of mass frame
    };
    
    /**
     *  the flags is by default _randomBit, which means randomly generate a mover
     */
    static MoverSprite *create(uint32 flags = _randomBit);
    // parameter body is needed, for fixed the mover with the terrain
    void setup(b2World *world, b2Body *mother, const cocos2d::Vec2& p, float radius);
    void selfDestruct(b2World *world);
    
    void update();
    
    b2Joint *_joint;
};

#endif /* defined(__nanatrip__MoverSprite__) */
