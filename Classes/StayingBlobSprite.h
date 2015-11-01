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
#include "SpriteWithBody.h"

#define ZORDER_FACE 1
#define ZORDER_EYE 2

class StayingBlobSprite : public SpriteWithBody
{
    /**
     * Scale used in gasUp method.
     * default 12, 10 is ok, 20 is a bit much, get stuck sometime.
     */
    int hardness = 12;
    
    Color4F _nanaColor = Color4F(0.1171875f, 0.125f, 0.15625f, 1);
    float radius;
    float inner_radius;
    float nub_pos = screenSiz.width*1.4/750;
    float nub_size;//screenSiz.width*8./750;
    float face_offset = -screenSiz.width*328/750;
    float eye_radius;
    
    std::vector<b2Body *> _bodies;
    std::vector<b2RevoluteJoint *> _joints;
    
    Color4F blobColor = Color4F(0.1171875f, 0.125f, 0.15625f, 1);
    cocos2d::DrawNode *_eyedrawer;
    cocos2d::Sprite *_face;
    cocos2d::CustomCommand _customCommand;
public:
    static StayingBlobSprite *create();
    void setup(b2World *world, b2CircleShape *shape);
    ~StayingBlobSprite();
    void selfDestruct(b2World *world);
    
    cocos2d::Vec2 getCenter();
    /**
     * Gas up the body, apply force with a scale, which is a member variable "hardness".
     * Currently only work with 12 segments. If change, need adjustment.
     */
    void update();
    
    // draw
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    void updateEye();
};

#endif /* defined(__nanatrip__StayingBlobSprite__) */
