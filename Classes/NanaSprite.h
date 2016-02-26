//
//  NanaSprite.h
//  nanatrip
//
//  Created by Macbook Air on 7/19/15.
//
//

#ifndef __nanatrip__NanaSprite__
#define __nanatrip__NanaSprite__

#include "NanaTrip.h"

class NanaSprite : public cocos2d::Sprite
{
//    b2Body *innerCircleBody;
    /**
     * Scale used in gasUp method.
     * default 12, 10 is ok, 20 is a bit much, get stuck sometime.
     */
    int hardness = 12;
    /**
     * When true, the chance of getting deformed would be slimer, I think.
     * default true.
     */
    bool enableUnbreakable = false;
    
    Color4F _nanaColor = Color4F(0.347656f, 0.68f, 0.8086f, 1);
    float radius = screenSiz.width*0.0016f;
    float inner_radius = screenSiz.width*0.0012f;
    float nub_pos = screenSiz.width*1.4/750;
    float nub_size = screenSiz.width*8./750;
    float face_offset = -screenSiz.width*28./750;
    
    cocos2d::Texture2D *_nanaFace;
    cocos2d::CustomCommand _customCommand;
public:
//    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) ;
//    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) ;
//    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) ;
    /**
     * _bodies[0]->getPosistion()
     */
    cocos2d::Vec2 getPosition0();
    float getpy();
    float getpx();
    cocos2d::Vec2 getCenter();
    bool isNana(b2Body *body);
    
    /**
     * Gas up the body, apply force with a scale, which is a member variable "hardness".
     * Currently only work with 12 segments. If change, need adjustment.
     */
    void gasUp();
    void ApplyForce(b2Vec2 force);
    
    static NanaSprite *create(b2World *world);
    
    NanaSprite(b2World *world);
    ~NanaSprite();
    void initPhysics(b2World *world);
    
    std::vector<b2Body *> _bodies;
    std::vector<b2RevoluteJoint *> _joints;
    
    // draw
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
};

#endif /* defined(__nanatrip__NanaSprite__) */
