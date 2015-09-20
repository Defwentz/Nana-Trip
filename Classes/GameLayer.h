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
#define ZORDER_HANDDRAW 4


class GameLayer : public cocos2d::Layer
{
    b2Vec2 stdGrav = b2Vec2(0, -8);
    
    GLESDebugDraw *_debugDraw;
    
    b2World *_world;
    NanaSprite *_nana;
    TerrainSprite *_terrain;
    
    // using it in this way is because, the background has to be consistent with nana's position
    // and simply put it in another layer statically just isn't gonna do.
    float after_height;
    std::vector<cocos2d::Sprite *> _bgSprites;
    
    // hand draw system trying
    b2Body *_drawBody;
    std::vector<cocos2d::Vec2> _drawVertices;
    std::vector<b2Fixture *> _drawFixtures;
    bool isCounting2Destroy;
    void destroyDrawFixtures(float dt);
    cocos2d::DrawNode *_drawNode;
public:
    static cocos2d::Scene* createScene();
    static GameLayer *create(InfoLayer *infoLayer);
    GameLayer();
    ~GameLayer();
    
    InfoLayer *_infoLayer;
    void initListeners();
    void initPhysics();
    void update(float dt);
    void gameOver();
    void reset();
    
    void initBG();
    void updateBG(float topY, float bottomY);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t transformFlags)
    override;
};

#endif // __GAME_LAYER_H__
