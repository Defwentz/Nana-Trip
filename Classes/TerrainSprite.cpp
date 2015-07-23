//
//  TerrainSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/22/15.
//
//

#include "TerrainSprite.h"

USING_NS_CC;

TerrainSprite* TerrainSprite::create()
{
    TerrainSprite* pRet = new TerrainSprite();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

TerrainSprite::TerrainSprite()
{
    
}

void TerrainSprite::initPhysics(b2World *_world)
{
    this->_world = _world;
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    _body = _world->CreateBody(&bd);
    /*
    for(int i = 1; i < lvertices.size(); i++) {
        Vec2 lp1, lp2;
        lp1 = lvertices[i - 1];
        lp2 = lvertices[i];
        setEdge(lp1, lp2, _world);
    }
    for(int i = 1; i < rvertices.size(); i++) {
        Vec2 rp1, rp2;
        rp1 = rvertices[i - 1];
        rp2 = rvertices[i];
        setEdge(rp1, rp2, _world);
    }
        
    _body->SetLinearVelocity(b2Vec2(0, 1));*/
}


//const int NOTHING = 0;//100;
//const int BUMPS = 30;
const int TUNNEL = 30;
const int GENERAL_MASK = TUNNEL;
int TerrainSprite::spawnTerrain()
{
    int r = rand() % GENERAL_MASK;
    if(r < TUNNEL) {
        
        return r;
    }
    r -= TUNNEL;
    return 0;
}

const int SUPER_NARROW = 10,
    ONE_ROW = 10,
    ROWS_FALLS_IN = 10,
    ROWS_STICK_TO = 10;
const int TUNNEL_WIDTH_MASK =
    SUPER_NARROW + ONE_ROW + ROWS_FALLS_IN + ROWS_STICK_TO;
void TerrainSprite::spawnTunnel(int _r)
{
    int r = rand() % TUNNEL_WIDTH_MASK;
    if(r < SUPER_NARROW) {
        
        return;
    }
    r -= SUPER_NARROW;
    if(r < ONE_ROW) {
        
        return;
    }
    r -= ONE_ROW;
    if(r < ROWS_FALLS_IN) {
        
        return;
    }
    r -= ROWS_FALLS_IN;
    if(r < ROWS_STICK_TO) {
        
        return;
    }
    r -= ROWS_STICK_TO;

}

void TerrainSprite::update()
{
    
}

// Draw

void TerrainSprite::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(TerrainSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void TerrainSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{
}

void TerrainSprite::drawSegment(Vec2 p1, Vec2 p2)
{
}