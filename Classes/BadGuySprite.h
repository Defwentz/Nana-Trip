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

class BadGuySprite : public cocos2d::Sprite
{
public:
    static BadGuySprite *create(const std::string& filename);
    BadGuySprite();
    
    b2Body *_body;
};

#endif /* defined(__nanatrip__BadGuySprite__) */
