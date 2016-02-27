//
//  TerrainSprite.h
//  nanatrip
//
//  Created by Macbook Air on 7/22/15.
//
//

#ifndef __nanatrip__TerrainSprite__
#define __nanatrip__TerrainSprite__

#include "NanaTrip.h"
#include "SpriteWithBody.h"
#include "MoverSprite.h"
#include "RedSprite.h"
#include "StayingBlobSprite.h"
#include "DNASprite.h"
#include "SlowerSprite.hpp"
#include "FurSprite.h"

// for general terrain randomer
#define ITEM_TUNNEL         0
#define ITEM_BUMPS          1
#define ITEM_CURVE          2
#define ITEM_CHESSBOARD     3
#define ITEM_BELT           4
#define ITEM_METEOR         5
#define ITEM_POCKET         6

#define ITEM_TUNNEL_NR      10
#define ITEM_TUNNEL_BRD     11

#define ITEM_BUMPS_1        20
#define ITEM_BUMPS_2        21
#define ITEM_BUMPS_3        22

#define ITEM_CURVE_BL       30
#define ITEM_CURVE_BR       31
#define ITEM_CURVE_TL       32
#define ITEM_CURVE_TR       33

#define ITEM_BALL_DNA       40
#define ITEM_BALL_METEOR    41
#define ITEM_BALL_BADGUY    42
#define ITEM_BALL_OBSTACLE  43
#define ITEM_BALL_SLOWER    44
#define ITEM_BALL_MOVER     45
#define ITEM_BALL_BLOB      46

class TerrainSprite : public cocos2d::Sprite
{
    b2World *_world;
    b2Body *_body;
    
    float narrowest_width = screenSiz.width * 0.07;
    
    std::vector<std::vector<b2Fixture *>> fixtures[2];
    std::vector<cocos2d::Vec2> vertices[2];
    int to[2];
    float getLastY();
    
    std::vector<b2Body *> littleguys;           // those yellow ball
    std::vector<SpriteWithBody *> obstacles;    // those brown round thing
    std::vector<SpriteWithBody *> dnas;         // yellow guy
    std::vector<SpriteWithBody *> movers;       // mover
    std::vector<SpriteWithBody *> badguys;      // red guy
    std::vector<SpriteWithBody *> blobs;        // blob
    std::vector<SpriteWithBody *> slowers;      // slower
    std::vector<SpriteWithBody *> furs;
    RedSprite *badboss = NULL;
    
    Randomer *terrainRdmr;  // general terrain randomer
    Randomer *tnlRdmr;
    Randomer *bumpRdmr;
    //Randomer *crvRdmr;
    Randomer *ballRdmr;
    
    cocos2d::CustomCommand _customCommand;
public:
    // new terrain idea: soft tissue
    void spawnTerrain();
    void spawnMeteor();
    void spawnBelt();
    void spawnTunnel();
    void spawnBumps();
    void spawnCurve();
    void spawnChessboard();
    // great tiny hidden pocket
    void spawnPockect(int which_side = -1, int pokect_size = winMidY/2);
    void spawnBallshapething(cocos2d::Vec2 vpos, float vradius);
    
    void createDNA(cocos2d::Vec2 vpos);
    void createMovingLittleGuy(cocos2d::Vec2 vpos, b2CircleShape *shape);
    void createBadGuy(cocos2d::Vec2 vpos, b2CircleShape *shape, int type);
    void createBallObstacle(cocos2d::Vec2 vpos, b2CircleShape *shape, bool withDNA);
    void createSlower(cocos2d::Vec2 vpos, b2CircleShape *shape, int type);
    void createMoverObstacle(cocos2d::Vec2 vpos, float radius);
    void createBlob(cocos2d::Vec2 vpos, b2CircleShape *shape);
    void createFur(cocos2d::Vec2 root, int length, int isRight);
    
    void update(float nanaY);
    void spriteCheck(std::vector<SpriteWithBody *> &sprites, float topY);
    void spriteCheckAndUpdate(std::vector<SpriteWithBody *> &sprites, float topY);
    
    void noMorePockets();
    
    void connectEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isRight);
    void drawEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isRight);
    void doVertices(cocos2d::Vec2 p1, cocos2d::Vec2 p2, void (*func)(const cocos2d::Vec2 &origin, const cocos2d::Vec2 &destination));
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    
    static TerrainSprite *create(b2World *world);
    
    TerrainSprite(b2World *world);
    ~TerrainSprite();
    void initPhysics(b2World *_world);
    
    float vault_switch[2] = {-narrowest_width, winSiz.width + narrowest_width};
};

#endif /* defined(__nanatrip__TerrainSprite__) */
