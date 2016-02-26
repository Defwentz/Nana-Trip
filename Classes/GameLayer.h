#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "NanaTrip.h"
#include "NanaSprite.h"
#include "TerrainSprite.h"
#include "GLESDebugDraw.h"
#include "InfoLayer.h"

#define ZORDER_GAMELAYER 0
#define ZORDER_INFOLAYER 1
#define ZORDER_OVERLAYER 2

#define ZORDER_BG 0
#define ZORDER_TERRAIN 1
#define ZORDER_NANA 2
#define ZORDER_HANDDRAW 4

void changeTexture(Sprite *sp, Texture2D* txture);
class GameLayer : public cocos2d::Layer
{
    
    float acc_parameter = 0.0234375 * screenSiz.width;
    
    /**
     * helper class for Box2D, to debug
     */
    GLESDebugDraw *_debugDraw;
    
    /**
     * Box2D world
     */
    b2World *_world;
    
    /**
     * Nana, aka Nano
     */
    NanaSprite *_nana;
    
    /**
     * Terrain
     */
    TerrainSprite *_terrain;
    
    /*** background sprites ***/
    // Using it in this way is because, the background has to be consistent with nana's position
    // and simply put it in another layer statically just isn't gonna do.
    
    float after_height;
    std::vector<cocos2d::Sprite *> _bgSprites;
    
    /*** gesture draw system ***/
    
    /**
     * Color of gesture draw.
     */
    Color4F _drawColor = Color4F(0.984375f, 0.85546875f, 0.15625f, 1);
    
    /**
     * How long the gesture draw will last once touch ended.
     */
    float _drawLast = 1.5f;
    
    /**
     * Box2D body of gesture draw.
     */
    b2Body *_drawBody;
    
    /**
     * Vertices of gesture draw in cocos2d::Vec2.
     */
    std::vector<cocos2d::Vec2> _drawVertices;
    
    /**
     * Fixtures of gesture draw.
     */
    std::vector<b2Fixture *> _drawFixtures;
    
    /**
     * DrawNode for gesture draw
     */
    cocos2d::DrawNode *_drawNode;
    
    /**
     * If the last gesture draw was being destroyed.
     */
    bool isCounting2Destroy;
    
    /**
     *  @brief	add new point to gesture draw
     *
     *  @param 	newPoint 	新点
     */
    void addPoint(cocos2d::Vec2 newPoint);
    
    /**
     *  @brief	destroy fixtures, clear DrawNode and vertices
     */
    void destroyDrawFixtures(float dt);
public:
    
    static cocos2d::Scene* createScene();
    static GameLayer *create(InfoLayer *infoLayer);
    GameLayer();
    ~GameLayer();
    
    InfoLayer *_infoLayer;
    
    /*
     *  @brief	Enable Accelerometer and attach listener, enable touch and attach listeners.
     */
    void initListeners();
    
    void initPhysics();
    void update(float dt) override;
    void gameOver(float dt);
    void reset();
    
    void initBG();
    void updateBG(float topY, float bottomY);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event) override;
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t transformFlags)
    override;
    
    void captureScreenCallback(bool, const std::string &);
    
    void pauseOverCallback(cocos2d::Ref *pSender);
    void pauseCallback(cocos2d::Ref *pSender);
    EventListenerKeyboard *keyListener;
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // __GAME_LAYER_H__
