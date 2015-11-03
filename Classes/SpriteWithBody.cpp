//
//  SpriteWithBody.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/25/15.
//
//

#include "SpriteWithBody.h"

SpriteWithBody* SpriteWithBody::create(const std::string& filename)
{
    SpriteWithBody *sprite = new (std::nothrow) SpriteWithBody();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
void SpriteWithBody::selfDestruct(b2World *world) {
    world->DestroyBody(_body);
}
void SpriteWithBody::removeFromVector(std::vector<SpriteWithBody *> &sprites, std::vector<SpriteWithBody *>::iterator &i, b2World *world) {
    (*i)->selfDestruct(world);
    (*i)->removeFromParent();
    i = sprites.erase(i);
}
void SpriteWithBody::update(){}