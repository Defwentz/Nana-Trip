//
//  FurSprite.hpp
//  nana
//
//  Created by Macbook Air on 2/26/16.
//
//

#ifndef FurSprite_hpp
#define FurSprite_hpp

#include "NanaTrip.h"
#include "SpriteWithBody.h"
class FurSprite : public SpriteWithBody
{
    int parts = 3;
    int _isRight;
    std::vector<b2Body *> _bodies;
    std::vector<b2DistanceJoint *> _joints;
    b2RevoluteJoint *_rjoint;

    cocos2d::CustomCommand _customCommand;
public:
    static FurSprite *create();
    void setup(b2World *world, b2Body *ground, b2Vec2 root, int isRight, int length = 100);
    ~FurSprite();
    void selfDestruct(b2World *world);
    void update();
    
//    Vec2 getPosition();
    // draw
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
};

#endif /* FurSprite_hpp */
