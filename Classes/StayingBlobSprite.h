//
//  StayingBlobSprite.h
//  nanatrip
//
//  Created by Macbook Air on 10/24/15.
//
//

#ifndef __nanatrip__StayingBlobSprite__
#define __nanatrip__StayingBlobSprite__

#include "NanaTrip.h"

class StayingBlobSprite : public cocos2d::Sprite
{
    /**
     * Scale used in gasUp method.
     * default 12, 10 is ok, 20 is a bit much, get stuck sometime.
     */
    int hardness = 12;
    
    Color4F _nanaColor = Color4F(0.347656f, 0.68f, 0.8086f, 1);
    float radius = screenSiz.width*0.0016f;
    float inner_radius = screenSiz.width*0.0012f;
    float nub_pos = screenSiz.width*1.4/750;
    float nub_size = screenSiz.width*8./750;
    float face_offset = -screenSiz.width*28./750;
    
    b2Body *_center_body;
    std::vector<b2Body *> _bodies;
    std::vector<b2RevoluteJoint *> _joints;
    
    cocos2d::CustomCommand _customCommand;
public:
    static StayingBlobSprite *create();
    StayingBlobSprite();
    
    /**
     * _bodies[0]->getPosistion()
     */
    cocos2d::Vec2 getPosition();
    cocos2d::Vec2 getCenter();
    /**
     * Gas up the body, apply force with a scale, which is a member variable "hardness".
     * Currently only work with 12 segments. If change, need adjustment.
     */
    void gasUp();
    
    // draw
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
};

#endif /* defined(__nanatrip__StayingBlobSprite__) */
