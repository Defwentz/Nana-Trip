//
//  BadGuySprite.h
//  nanatrip
//
//  Created by Macbook Air on 7/25/15.
//
//

#ifndef __nanatrip__BadGuySprite__
#define __nanatrip__BadGuySprite__

#include "NanaTrip.h"

// sprite with b2Body
class SpriteWithBody : public cocos2d::Sprite
{
    
public:
    static SpriteWithBody *create(const std::string& filename);
    virtual void selfDestruct(b2World *world);
    virtual void update();
    
    static void removeFromVector(std::vector<SpriteWithBody *> &sprites, std::vector<SpriteWithBody *>::iterator &i, b2World *world);
    
    b2Body *_body;
};

#endif /* defined(__nanatrip__BadGuySprite__) */
