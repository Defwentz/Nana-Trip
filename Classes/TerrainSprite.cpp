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
    
    // Define the ground box shape.
    b2EdgeShape border;
    
    auto s = Director::getInstance()->getWinSize();
    
    Vec2 bl = Vec2(0, 0);
    Vec2 tl = Vec2(0, s.height);
    Vec2 tr = Vec2(s.width, s.height);
    Vec2 br = Vec2(s.width, 0);
    
    //lvertices.push_back(tl);
    lvertices.push_back(bl);
    //rvertices.push_back(tr);
    rvertices.push_back(br);
    
    // left
    border.Set(vToB2(bl), vToB2(tl));
    std::vector<b2Fixture *> _lfixtures;
    _lfixtures.push_back(_body->CreateFixture(&border,0));
    lfixtures.push_back(_lfixtures);
    
    // right
    border.Set(vToB2(tr), vToB2(br));
    std::vector<b2Fixture *> _rfixtures;
    _rfixtures.push_back(_body->CreateFixture(&border,0));
    rfixtures.push_back(_rfixtures);
    
    lto = 1;rto = 1;
    spawnTerrain();

    
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
        
    _body->SetLinearVelocity(b2Vec2(0, 1));
    */
}


//const int NOTHING = 0;//100;
//const int BUMPS = 30;
const int TUNNEL = 30,
    GENERAL_MASK = TUNNEL;
int TerrainSprite::spawnTerrain()
{
    int r = rand() % GENERAL_MASK;
    if(r < TUNNEL) {
        this->spawnTunnel(r);
        return r;
    }
    r -= TUNNEL;
    return 0;
}

const int SUPER_NARROW = 10,
    ONE_ROW = 0,
    ROWS_FALLS_IN = 0,
    ROWS_STICK_TO = 0,
    TUNNEL_WIDTH_MASK =
        SUPER_NARROW + ONE_ROW + ROWS_FALLS_IN + ROWS_STICK_TO;

const int TUNNEL_KEYPOINT = 5,
    TUNNEL_KP_DIST_IN_SCREEN = 3,
    X_IN_SCREEN = 5,
    NARROW_WIDTH = 45,                      // different than others
    ROW_WIDTH = 10, ROW_WIDTH_ADDON = 50;   // different than others
void TerrainSprite::spawnTunnel(int _r)
{
    auto winSize = Director::getInstance()->getWinSize();
    float lastY;
    if(lvertices.back().y < rvertices.back().y)
        lastY = lvertices.back().y;
    else
        lastY = rvertices.back().y;
    
    int r = rand() % TUNNEL_WIDTH_MASK;
    if(r < SUPER_NARROW) {
        int n = randWithBase(TUNNEL_KEYPOINT, TUNNEL_KEYPOINT);
        for(int i = 0; i < n; i++) {
            int length = randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                                  winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
            int width = randWithBase(NARROW_WIDTH, NARROW_WIDTH);
            int x = randWithBase(winSize.width/X_IN_SCREEN, winSize.width*3/X_IN_SCREEN);
            lastY -= length;
            
            lvertices.push_back(Vec2(x, lastY));
            rvertices.push_back(Vec2(x + width, lastY));
        }
        lastY -= randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                                  winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
        
        lvertices.push_back(Vec2(0, lastY));
        rvertices.push_back(Vec2(winSize.width, lastY));
        return;
    }
    r -= SUPER_NARROW;
    /*
    if(r < ONE_ROW) {
  
        int n = randWithBase(TUNNEL_KEYPOINT, TUNNEL_KEYPOINT);
        for(int i = 0; i < n; i++) {
            int length = randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                                      winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
            int width = randWithBase(NARROW_WIDTH + ROW_WIDTH,
                                     NARROW_WIDTH + ROW_WIDTH_ADDON);
            int x = randWithBase(X_IN_SCREEN, X_IN_SCREEN);
            y -= length;
            
            lnodes.push_back(TerrainNode(x, y));
            rnodes.push_back(TerrainNode(x + width, y));
        }
        y -= randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                          winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
        
        lnodes.push_back(TerrainNode(0, y));
        rnodes.push_back(TerrainNode(winSize.width, y));
        
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
*/
}

void TerrainSprite::doVertices(cocos2d::Vec2 p1, cocos2d::Vec2 p2, void (*func)(cocos2d::Vec2, cocos2d::Vec2))
{
    float tdy = p1.y - p2.y;
    //////////////
    if(tdy == 0);
    //////////////
    
    int n = floorf(tdy/10);
    float dy = tdy / n;
    float dj = M_PI / n;
    float A = (p1.x - p2.x)/2;
    float B = (p1.x + p2.x)/2;
    // x = A*cos(wy)+B
    
    Vec2 _p1 = p1, _p2;
    float y = 0;
    for(int j = 1; j <= n; j++) {
        _p2.y = _p1.y - dy;
        y+=dj;
        _p2.x = A*cosf(y) + B;
        
        func(_p1, _p2);
        _p1 = _p2;
    }
}

int TerrainSprite::randWithBase(int base, int addon)
{
    return base + rand()%addon;
}

b2Vec2 TerrainSprite::vToB2(cocos2d::Vec2 v)
{
    return b2Vec2(v.x/PTM_RATIO, v.y/PTM_RATIO);
}

cocos2d::Vec2 TerrainSprite::b2ToV(b2Vec2 b)
{
    return cocos2d::Vec2(b.x*PTM_RATIO, b.y*PTM_RATIO);
}

void TerrainSprite::connectEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isLeft)
{
    float tdy = p1.y - p2.y;
    //////////////
    if(tdy == 0);
    //////////////
    
    int n = floorf(tdy/10);
    float dy = tdy / n;
    float dj = M_PI / n;
    float A = (p1.x - p2.x)/2;
    float B = (p1.x + p2.x)/2;
    // x = A*cos(wy)+B
    
    Vec2 _p1 = p1, _p2;
    std::vector<b2Fixture *> _fixtures;
    
    float y = 0;
    for(int j = 1; j <= n; j++) {
        _p2.y = _p1.y - dy;
        y+=dj;
        _p2.x = A*cosf(y) + B;
        
        b2FixtureDef fd;
        b2EdgeShape border;
        border.Set(vToB2(_p1), vToB2(_p2));
        fd.shape = &border;
        fd.density = 0;
        fd.friction = 0;
        _fixtures.push_back(_body->CreateFixture(&border,0));
        
        _p1 = _p2;
    }
    
    if(isLeft)
        lfixtures.push_back(_fixtures);
    else
        rfixtures.push_back(_fixtures);
}

void TerrainSprite::update(float nanaY)
{
    auto winSize = Director::getInstance()->getWinSize();
    float topY = nanaY + winSize.height/2 + winSize.height/8;
    float bottomY = nanaY - winSize.height/2 - winSize.height/8;
    
    if(lvertices.size() < 2) {
        spawnTerrain();
    }
    else if (lvertices[1].y > topY) {
        lvertices.erase(lvertices.cbegin());
        std::vector<b2Fixture *> _fixtures = lfixtures.front();
        for(int i = 0; i < _fixtures.size(); i++) {
            _body->DestroyFixture(_fixtures[i]);
        }
        lfixtures.erase(lfixtures.cbegin());
        lto--;
    }
    for(int i = lto; i < lvertices.size(); i++) {
        if(lvertices[i - 1].y > bottomY) {
            connectEdge(lvertices[i - 1], lvertices[i], 1);
            lto++;
        }else
            break;
    }
    if(lto == lvertices.size())
        spawnTerrain();
    // 什么时候生成地形还有问题.
    
    if(rvertices.size() < 2) {
        spawnTerrain();
    }
    else if (rvertices[1].y > topY) {
        rvertices.erase(rvertices.cbegin());
        std::vector<b2Fixture *> _fixtures = rfixtures.front();
        for(int i = 0; i < _fixtures.size(); i++) {
            _body->DestroyFixture(_fixtures[i]);
        }
        rfixtures.erase(rfixtures.cbegin());
        rto--;
    }
    for(int i = rto; i < rvertices.size(); i++) {
        if(rvertices[i - 1].y > bottomY) {
            connectEdge(rvertices[i - 1], rvertices[i], 0);
            rto++;
        }else
            break;
    }
    if(rto == rvertices.size())
        spawnTerrain();
}

// Draw

void TerrainSprite::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(TerrainSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void TerrainSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{/*
    auto winSize = Director::getInstance()->getWinSize();
    auto parent = (HelloWorld *)(this->getParent());
    float sb = parent->getScreenY();
    //float st = parent->getScreenY() + winSize.height;
    
    for(int i = 1; i < lvertices.size(); i++) {
        Vec2 lp1, lp2;
        lp1 = lvertices[i - 1];
        lp2 = lvertices[i];
        // might be buggy
        if(lvertices[i + 1].y < sb)
            break;
        
        //drawSegment(lp1, lp2);
    }*/
}

void TerrainSprite::drawSegment(Vec2 p1, Vec2 p2)
{
}