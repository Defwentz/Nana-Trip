#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "NanaTrip.h"
#include "NanaSprite.h"
#include "TerrainSprite.h"
#include "GLESDebugDraw.h"
#include "InfoLayer.h"

#define ZORDER_GAMELAYER 0
#define ZORDER_INFOLAYER 1

#define ZORDER_BG 0
#define ZORDER_TERRAIN 1
#define ZORDER_NANA 2


class GameLayer : public cocos2d::Layer
{
    GLESDebugDraw *_debugDraw;
    
    b2World *_world;
    NanaSprite *_nana;
    TerrainSprite *_terrain;
    
    // using it in this way is because, the background has to be consistent with nana's position
    // and simply put it in another layer statically just isn't gonna do.
    float after_height;
    std::vector<cocos2d::Sprite *> _bgSprites;
public:
    static cocos2d::Scene* createScene();
    static GameLayer *create(InfoLayer *infoLayer);
    GameLayer();
    ~GameLayer();
    
    InfoLayer *_infoLayer;
    void initListeners();
    void initBG();
    void initPhysics();
    void update(float dt);
    void gameOver();
    void reset();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t transformFlags)
    override;
};

#endif // __GAME_LAYER_H__
