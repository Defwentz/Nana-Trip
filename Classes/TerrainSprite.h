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

//#define TYPE_DOOR                   99
//#define TYPE_TUNNEL_SUPER_NARROW    10
////#define TYPE_TUNNEL_ONE_ROW         11
//#define TYPE_TUNNEL_TWO_ROWS        12

// for general terrain randomer
#define ITEM_TUNNEL         0
#define ITEM_BUMPS          1
#define ITEM_CURVE          2
#define ITEM_CHESSBOARD     3
#define ITEM_BELT           4
#define ITEM_METEOR         5

#define ITEM_TUNNEL_NR      10
#define ITEM_TUNNEL_BRD     11

#define ITEM_BUMPS_1        20
#define ITEM_BUMPS_2        21
#define ITEM_BUMPS_3        22

#define ITEM_CURVE_BL       30
#define ITEM_CURVE_BR       31
#define ITEM_CURVE_TL       32
#define ITEM_CURVE_TR       33

class TerrainSprite : public cocos2d::Sprite
{
    b2World *_world;
    b2Body *_body;
    
    //std::vector<int> terrainTypes;
    
    std::vector<std::vector<b2Fixture *>> lfixtures;
    std::vector<std::vector<b2Fixture *>> rfixtures;
    std::vector<cocos2d::Vec2> lvertices;
    std::vector<cocos2d::Vec2> rvertices;
    int lto, rto;
    float getLastY();
    
    std::vector<b2Body *> littleguys;
    std::vector<SpriteWithBody *> obstacles;
    std::vector<SpriteWithBody *> badguys;
    std::vector<SpriteWithBody *> dnas;
    
    Randomer *terrainRdmr;  // general terrain randomer
    Randomer *tnlRdmr;
    Randomer *bumpRdmr;
    Randomer *crvRdmr;
    
    cocos2d::CustomCommand _customCommand;
public:
    void spawnTerrain();
    void spawnMeteor();
    void spawnBelt();
    void spawnTunnel();
    void spawnBumps();
    void spawnCurve();
    void spawnChessboard();
    
    void createDNA(cocos2d::Vec2 vpos);
    void createMovingLittleGuy(cocos2d::Vec2 vpos, b2CircleShape *shape);
    void createBadGuy(cocos2d::Vec2 vpos, b2CircleShape *shape);
    void createBallObstacle(cocos2d::Vec2 vpos, b2CircleShape *shape, bool withDNA);
    void createMoverObstacle(cocos2d::Vec2 vpos, float radius);
    
    void update(float nanaY);
    
    void spriteCheck(std::vector<SpriteWithBody *> &sprites, float topY);
    
    void connectEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isLeft);
    void doVertices(cocos2d::Vec2 p1, cocos2d::Vec2 p2, void (*func)(const cocos2d::Vec2 &origin, const cocos2d::Vec2 &destination));
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    void drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
    
    int randWithBase(int base, int addon);
    // the default bool is false, when rand_0_1() < odds, bool switch to true
    bool boolWithOdds(float odds);
    
    static TerrainSprite *create(b2World *world);
    
    TerrainSprite(b2World *world);
    ~TerrainSprite();
    void initPhysics(b2World *_world);
};

#endif /* defined(__nanatrip__TerrainSprite__) */
