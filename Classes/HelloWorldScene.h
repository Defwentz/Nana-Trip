#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "NanaTrip.h"
#include "GLESDebugDraw.h"
#include "NanaSprite.h"
#include "Terrain.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void initPhysics();
    void update(float dt);
    //void bounce();
    //b2Vec2 &getNormal(b2Vec2 src);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    ~HelloWorld();
protected:
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t transformFlags)
        override;
    
    GLESDebugDraw *_debugDraw;
    
    b2World* _world;
    NanaSprite* nanaSprite;
    //std::vector<b2Body *> bodies;
    
    TerrainNode* terrain;
};

#endif // __HELLOWORLD_SCENE_H__
