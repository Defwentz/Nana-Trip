//
//  Terrain.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/20/15.
//
//

#include "Terrain.h"

USING_NS_CC;

#define NARROW_BASE_VERTICES 3
#define NARROW_ADDON_MAX_VERTICES 3
#define NARROW_WIDTH 40
#define PTM_RATIO 32

const int NOTHING = 0;//100;
const int BUMPS = 30;
const int NARROW = 30;
const int MASK =
NOTHING + BUMPS + NARROW;

TerrainNode *TerrainNode::create()
{
    auto node = new TerrainNode;
    node->createNarrow();
    node->autorelease();
    
    return node;
}


bool TerrainNode::init()
{
    this->createNarrow();
}

void TerrainNode::appendTerrain()
{
    int r = rand() % MASK;
    if(r < NOTHING) {
        return;
    }
    r -= NOTHING;
    if(r < BUMPS) {
        this->createBumps();
        return;
    }
    r -= BUMPS;
    if(r < NARROW) {
        this->createNarrow();
        return;
    }
    r -= NARROW;
}

void TerrainNode::createNarrow()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    lvertices.push_back(Vec2(0, 0));
    rvertices.push_back(Vec2(winSize.width, 0));
    
    int n = randWithBase(NARROW_BASE_VERTICES, NARROW_ADDON_MAX_VERTICES);
    int lasty = 0;
    for(int i = 0; i < n; i++) {
        int dv = randWithBase(winSize.height / 5, winSize.height / 5);
        //rand() % (int)(winSize.height / 5) + winSize.height / 5;
        int newx = random(winSize.width/5, winSize.width*4/5);
        lvertices.push_back(Vec2(newx, lasty - dv));
        int tunnel_width = randWithBase(NARROW_WIDTH, NARROW_WIDTH);
        rvertices.push_back(Vec2(newx + tunnel_width, lasty - dv));
        
        lasty -= dv;
    }
    lvertices.push_back(Vec2(0, lasty - 700));
    rvertices.push_back(Vec2(winSize.width, lasty - 300));
}

void TerrainNode::createBumps()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    lvertices.push_back(Vec2(0, 0));
    rvertices.push_back(Vec2(winSize.width, 0));
    
    int n = rand() % NARROW_ADDON_MAX_VERTICES + NARROW_BASE_VERTICES;
    int lasty = 0;
    for(int i = 0; i < n; i++) {
        int dv = rand() % (int)(winSize.height / 3) + winSize.height / 3;
        int newx = random(winSize.width/5, winSize.width*4/5);
        lvertices.push_back(Vec2(newx, lasty - dv));
        float tunnel_width = NARROW_WIDTH + rand()%NARROW_WIDTH;
        rvertices.push_back(Vec2(newx + tunnel_width, lasty - dv));
        
        lasty -= dv;
    }
    lvertices.push_back(Vec2(0, lasty - 300));
    rvertices.push_back(Vec2(winSize.width, lasty - 700));
}

int TerrainNode::randWithBase(int base, int addon)
{
    return base + rand()%addon;
}

void TerrainNode::update()
{
    for (int i = 0; i < lvertices.size(); i++) {
        //lvertices[i].y++;
    }
    for (int i = 0; i < rvertices.size(); i++) {
        //rvertices[i].y++;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    if((lvertices.size() == 0 &&
        rvertices.size() == 0) ||
       (lvertices.back().y > winSize.height &&
       rvertices.back().y > winSize.height)) {
           int r = rand() % MASK;
           if(r < NOTHING) {
               return;
           }
           r -= NOTHING;
           if(r < BUMPS) {
               this->createBumps();
               return;
           }
           r -= BUMPS;
           if(r < NARROW) {
               this->createNarrow();
               return;
           }
           r -= NARROW;
    }
}

void TerrainNode::updateEdge(b2World *_world)
{
    int bodyInWorld = 0;
    for(b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
     {
         if(b == _body) {
             bodyInWorld = 1;
             break;
         }
     }
    if(_body && _world && bodyInWorld)
        _world->DestroyBody(_body);
    
    b2BodyDef bd;
    bd.position.SetZero();
    _body = _world->CreateBody(&bd);
    
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
}

void TerrainNode::setEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, b2World *_world)
{
    float tdy = p1.y - p2.y;
    
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
        b2FixtureDef fd;
        b2EdgeShape shape;
        //shape.Set(b2Vec2(_p1.x, _p1.y), b2Vec2(_p2.x, _p2.y));
        shape.Set(b2Vec2(_p1.x/PTM_RATIO, _p1.y/PTM_RATIO), b2Vec2(_p2.x/PTM_RATIO, _p2.y/PTM_RATIO));
        fd.shape = &shape;
        fd.density = 0;
        fd.friction = 0;
        _body->CreateFixture(&shape, 0);
        //ccDrawLine(_p1, _p2);
        _p1 = _p2;
    }
}

// Draw

void TerrainNode::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    //_customCommand.init(_globalZOrder);
    //_customCommand.func = CC_CALLBACK_0(TerrainNode::onDraw, this, transform, flags);
    //renderer->addCommand(&_customCommand);
}

void TerrainNode::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{
    ccDrawColor4F(1.0, 1.0, 1.0, 1.0);
    for(int i = 1; i < lvertices.size(); i++) {
        
        Vec2 lp1, lp2;
        lp1 = lvertices[i - 1];
        lp2 = lvertices[i];
        
        //ccDrawLine(lp1, lp2);
        //ccDrawLine(rp1, rp2);
        
        drawSegment(lp1, lp2);
    }
    for(int i = 1; i < rvertices.size(); i++) {
        
        Vec2 rp1, rp2;
        rp1 = rvertices[i - 1];
        rp2 = rvertices[i];
        
        drawSegment(rp1, rp2);
    }
}

void TerrainNode::drawSegment(Vec2 p1, Vec2 p2)
{
    float tdy = p1.y - p2.y;
    
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
        
        ccDrawLine(_p1, _p2);
        _p1 = _p2;
    }
}

