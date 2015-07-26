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
//#include "HelloWorldScene.h"
//#include "TerrainNode.h"

#define TYPE_DOOR                   99
#define TYPE_TUNNEL_SUPER_NARROW    10
//#define TYPE_TUNNEL_ONE_ROW         11
#define TYPE_TUNNEL_TWO_ROWS        12

// for general terrain randomer
#define ITEM_TUNNEL     1
#define ITEM_BUMPS      2
#define ITEM_CHESSBOARD 3

#define ITEM_BUMPS_1    21
#define ITEM_BUMPS_2    22
#define ITEM_BUMPS_3    23

class TerrainSprite : public cocos2d::Sprite
{
public:
    static TerrainSprite *create();
    
    TerrainSprite();
    ~TerrainSprite();
    
    void initPhysics(b2World *_world);
    void spawnTerrain();
    void spawnTunnel();
    void spawnBumps();
    void spawnChessboard();
    
    void connectEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isLeft);
    void doVertices(cocos2d::Vec2 p1, cocos2d::Vec2 p2, void (*func)(const cocos2d::Vec2 &origin, const cocos2d::Vec2 &destination));
    
    int randWithBase(int base, int addon);
    
    // the default bool is false, when rand_0_1() < odds, bool switch to true
    bool boolWithOdds(float odds);
    
    void update(float nanaY);
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    void drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
    
    static b2Vec2 vToB2(cocos2d::Vec2 v);
    static cocos2d::Vec2 b2ToV(b2Vec2 b);
protected:
    b2World *_world;
    b2Body *_body;
    
    //std::vector<int> terrainTypes;
    
    std::vector<b2Fixture *> nonos;
    std::vector<cocos2d::Vec2> nonoVertices;

    std::vector<std::vector<b2Fixture *>> lfixtures;
    std::vector<std::vector<b2Fixture *>> rfixtures;
    std::vector<cocos2d::Vec2> lvertices;
    std::vector<cocos2d::Vec2> rvertices;
    int lto, rto;
    float getLastY();
    
    // TODO
    std::vector<SpriteWithBody *> obstacles;
    std::vector<SpriteWithBody *> badguys;
    
    // general terrain randomer
    Randomer *terrainRdmr;
    Randomer *bumpRdmr;
    
    cocos2d::CustomCommand _customCommand;
};

#endif /* defined(__nanatrip__TerrainSprite__) */
