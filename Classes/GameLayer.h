#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "NanaTrip.h"
#include "NanaSprite.h"
#include "TerrainSprite.h"
#include "GLESDebugDraw.h"
#include "InfoLayer.h"

class GameLayer : public cocos2d::Layer
{
    GLESDebugDraw *_debugDraw;
    
    b2World *_world;
    NanaSprite *_nana;
    TerrainSprite *_terrain;
    
    float after_height;
    std::vector<cocos2d::Sprite *> _bgSprites;
    
    InfoLayer *_infoLayer;
public:
    static cocos2d::Scene* createScene();
    
    void initListeners();
    void initBG();
    void initPhysics();
    void update(float dt);
    
    void gameOver();
    void reset();
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t transformFlags)
    override;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
    
    static GameLayer *create(InfoLayer *infoLayer);
    GameLayer();
    ~GameLayer();
};

#endif // __GAME_LAYER_H__
