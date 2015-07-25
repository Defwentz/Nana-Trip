//
//  BadGuySprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/25/15.
//
//

#include "BadGuySprite.h"

BadGuySprite* BadGuySprite::create(const std::string& filename)
{
    BadGuySprite *sprite = new (std::nothrow) BadGuySprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
BadGuySprite::BadGuySprite(){}