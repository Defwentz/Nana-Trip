#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "NanaTrip.h"
#include "NanaSprite.h"
#include "TerrainSprite.h"
#include "GLESDebugDraw.h"
#include "InfoLayer.h"

class HelloWorld : public cocos2d::Layer
{
    GLESDebugDraw *_debugDraw;
    
    b2World *_world;
    NanaSprite *_nana;
    TerrainSprite *_terrain;
    
    // currently score is -(y coordinate of nana's bodies[0])
    //cocos2d::Label *scoreLabel;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    static void initWinSiz();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void initPhysics();
    void update(float dt);
    
    void reset();
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t transformFlags)
    override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    ~HelloWorld();
};

#endif // __HELLOWORLD_SCENE_H__
