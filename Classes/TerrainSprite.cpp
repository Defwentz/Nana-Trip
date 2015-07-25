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
    terrainTypes.push_back(TYPE_DOOR);
    
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
}


//const int NOTHING = 0;//100;
//const int BUMPS = 30;
const int ODDS_TUNNEL = 30,
    ODDS_BUMPS = 0,
    ODDS_CHESSBORD = 30,
    GENERAL_MASK = ODDS_TUNNEL + ODDS_CHESSBORD;
int TerrainSprite::spawnTerrain()
{
    int r = rand() % GENERAL_MASK;
    if(r < ODDS_TUNNEL) {
        this->spawnTunnel(r);
        return r;
    }
    r -= ODDS_TUNNEL;/*
    if(r < ODDS_BUMPS) {
        this->spawnBumps(r);
        return r;
    }
    r -= ODDS_BUMPS;*/
    if(r < ODDS_CHESSBORD) {
        this->spawnChessbord(r);
        return r;
    }
    r -= ODDS_CHESSBORD;
    return 0;
}

const int ODDS_SUPER_NARROW = 10,
    ODDS_ONE_ROW = 0,
    ODDS_TWO_ROWS = 10,
    TUNNEL_WIDTH_MASK =
        ODDS_SUPER_NARROW + ODDS_ONE_ROW + ODDS_TWO_ROWS;

const int TUNNEL_KEYPOINT = 5,
    TUNNEL_KP_DIST_IN_SCREEN = 3,
    X_IN_SCREEN = 5,
    NARROW_WIDTH = 45,                      // different than others
    ROW_WIDTH = 50, ROW_WIDTH_ADDON = 50;   // different than others
void TerrainSprite::spawnTunnel(int _r)
{
    auto winSize = Director::getInstance()->getWinSize();
    float lastY;
    if(lvertices.back().y < rvertices.back().y)
        lastY = lvertices.back().y;
    else
        lastY = rvertices.back().y;
    
    int r = rand() % TUNNEL_WIDTH_MASK;
    int n = randWithBase(TUNNEL_KEYPOINT, TUNNEL_KEYPOINT);
    
    if(r < ODDS_SUPER_NARROW) {
        for(int i = 0; i < n; i++) {
            int length = randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                                  winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
            int width = randWithBase(NARROW_WIDTH, NARROW_WIDTH);
            int x = randWithBase(winSize.width/X_IN_SCREEN, winSize.width*3/X_IN_SCREEN);
            lastY -= length;
            
            lvertices.push_back(Vec2(x, lastY));
            rvertices.push_back(Vec2(x + width, lastY));
            terrainTypes.push_back(TYPE_TUNNEL_SUPER_NARROW);
        }
        lastY -= randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                                  winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
        lvertices.push_back(Vec2(0, lastY));
        rvertices.push_back(Vec2(winSize.width, lastY));
        terrainTypes.push_back(TYPE_DOOR);
        return;
    }
    r -= ODDS_SUPER_NARROW;
    /*
    if(r < ODDS_ONE_ROW) {
        for(int i = 0; i < n; i++) {
            int length = randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                                      winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
            int width = randWithBase(NARROW_WIDTH + ROW_WIDTH, NARROW_WIDTH + ROW_WIDTH_ADDON);
            int x = randWithBase(winSize.width/X_IN_SCREEN, winSize.width*3/X_IN_SCREEN);
            lastY -= length;
            
            lvertices.push_back(Vec2(x, lastY));
            rvertices.push_back(Vec2(x + width, lastY));
            terrainTypes.push_back(TYPE_TUNNEL_ONE_ROW);
        }
        lastY -= randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                              winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
        lvertices.push_back(Vec2(0, lastY));
        rvertices.push_back(Vec2(winSize.width, lastY));
        terrainTypes.push_back(TYPE_DOOR);
        return;
    }
    r -= ODDS_ONE_ROW;*/
    if(r < ODDS_TWO_ROWS) {
        for(int i = 0; i < n; i++) {
            int length = randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                                      winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
            int width = randWithBase(NARROW_WIDTH + ROW_WIDTH, NARROW_WIDTH + ROW_WIDTH_ADDON);
            int x = randWithBase(winSize.width/X_IN_SCREEN, winSize.width*3/X_IN_SCREEN);
            lastY -= length;
            
            lvertices.push_back(Vec2(x, lastY));
            rvertices.push_back(Vec2(x + width, lastY));
            terrainTypes.push_back(TYPE_TUNNEL_TWO_ROWS);
        }
        lastY -= randWithBase(winSize.height/TUNNEL_KP_DIST_IN_SCREEN,
                              winSize.height/TUNNEL_KP_DIST_IN_SCREEN);
        lvertices.push_back(Vec2(0, lastY));
        rvertices.push_back(Vec2(winSize.width, lastY));
        terrainTypes.push_back(TYPE_DOOR);
        return;
    }
    r -= ODDS_TWO_ROWS;
}

void TerrainSprite::spawnBumps(int r)
{
    
}

const int MIN_COL = 3,
    MAX_COL = 7;
//int max_radius = winSize.width / (MIN_COL-1) - margin;
//int min_radius = winSize.width / (MAX_COL-1) - margin;
void TerrainSprite::spawnChessbord(int r)
{
    auto winSize = Director::getInstance()->getWinSize();
    float lastY;
    if(lvertices.back().y < rvertices.back().y)
        lastY = lvertices.back().y;
    else
        lastY = rvertices.back().y;
    
    int length = randWithBase(winSize.height/2,
                              winSize.height*2);
    lvertices.push_back(Vec2(0, lastY-length));
    rvertices.push_back(Vec2(winSize.width, lastY-length));
    terrainTypes.push_back(TYPE_DOOR);
    int margin = randWithBase(NARROW_WIDTH, NARROW_WIDTH);
    
    int col = randWithBase(MIN_COL, MAX_COL-MIN_COL);
    float max_radius = (winSize.width / (col-1) - margin)/2;
    float min_radius = (winSize.width / (MAX_COL-1) - margin)/2;
    int row = (margin + length) / (2*max_radius + margin);
    
    float odds = rand_0_1();
    
    lastY -= max_radius;
    for(int i = 0; i < row; i++) {
        float x = random(-max_radius/2, max_radius/2);
        for(int j = 0; j < col; j++, x += (2*max_radius + margin)) {
            if(rand_0_1() > odds)continue;
            
            bool isBadGuy = false;
            if(rand_0_1() > 0.5)isBadGuy = true;
            
            BadGuySprite *bg;
            Vec2 pos = Vec2(x, lastY);
            if(isBadGuy) {
                bg = BadGuySprite::create("red.png");
                bg->setPosition(pos);
                badguys.push_back(bg);
            }
            else nonoVertices.push_back(pos);
            
            b2CircleShape ball;
            ball.m_p = vToB2(pos);
            ball.m_radius = random(min_radius, max_radius)/PTM_RATIO;
            
            if(isBadGuy) {
                bg->setScale(ball.m_radius*PTM_RATIO/36);
                this->addChild(bg);
                
                b2BodyDef bd;
                bd.position.SetZero();
                bd.type = b2_staticBody;
                b2Body *b = _world->CreateBody(&bd);
                b->SetUserData(new Entity(UD_BADGUY));
                b->CreateFixture(&ball, 0);
                bg->_body = b;
            }
            else nonos.push_back(_body->CreateFixture(&ball, 0));
        }
        lastY -= (2*max_radius + margin);
    }
    
}

void TerrainSprite::doVertices(cocos2d::Vec2 p1, cocos2d::Vec2 p2, void (*func)(const cocos2d::Vec2 &origin, const cocos2d::Vec2 &destination))
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
    //if(tdy == 0);
    /*
    bool spawnLives = false;
    switch (terrainTypes.front()) {
        case TYPE_DOOR:
        case TYPE_TUNNEL_SUPER_NARROW:
            break;
        case TYPE_TUNNEL_TWO_ROWS:
            spawnLives = true;
            break;
        default:
            break;
    }*/
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
        
        b2EdgeShape border;
        b2Vec2 _bp1 = vToB2(_p1);
        b2Vec2 _bp2 = vToB2(_p2);
        border.Set(_bp1, _bp2);
        _fixtures.push_back(_body->CreateFixture(&border,0));
        /*
        if(spawnLives) {
            if(terrainTypes.front() == TYPE_TUNNEL_TWO_ROWS) {
                nonoVertices.push_back(_p1);
                
                b2CircleShape ball;
                ball.m_radius = (_bp2 - _bp1).Length()*3/8;
                ball.m_p = _bp1;
                nonos.push_back(_body->CreateFixture(&ball, 0));
            }
        }*/
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
    
    //if(lvertices.size() < 2) {
       // spawnTerrain();
    //}
    //else
        if (lvertices[1].y > topY) {
        lvertices.erase(lvertices.cbegin());
        std::vector<b2Fixture *> _fixtures = lfixtures.front();
        for(int i = 0; i < _fixtures.size(); i++) {
            _body->DestroyFixture(_fixtures[i]);
        }
        lfixtures.erase(lfixtures.cbegin());
        terrainTypes.erase(terrainTypes.cbegin());
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
    
    //if(rvertices.size() < 2) {
      //  spawnTerrain();
    //}
    //else
        if (rvertices[1].y > topY) {
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

    for(std::vector<Vec2>::iterator i = nonoVertices.begin();
        i != nonoVertices.end();) {
        if(i->y > topY) {
            i = nonoVertices.erase(i);
            _body->DestroyFixture(nonos.front());
            nonos.erase(nonos.cbegin());
        }
        else break;
    }
    
    for(std::vector<BadGuySprite *>::iterator i = badguys.begin();
        i != badguys.end();) {
        if((*i)->getPosition().y > topY) {
            _world->DestroyBody((*i)->_body);
            (*i)->removeFromParent();
            i = badguys.erase(i);
        }
        else break;
    }
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
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    glLineWidth( 5.0f );
    ccDrawColor4F(1.0, 1.0, 1.0, 1.0);
    // left right curve floor
    for(int i = 1; i < lto; i++) {
        
        Vec2 lp1, lp2;
        lp1 = lvertices[i - 1];
        lp2 = lvertices[i];
        
        
        doVertices(lp1, lp2, ccDrawLine);
    }
    for(int i = 1; i < rto; i++) {
        
        Vec2 rp1, rp2;
        rp1 = rvertices[i - 1];
        rp2 = rvertices[i];
        
        doVertices(rp1, rp2, ccDrawLine);
    }
    
    // bad guy
    for(int i = 0; i < nonoVertices.size(); i++) {
        ccDrawSolidCircle(nonoVertices[i],
                          nonos[i]->GetShape()->m_radius * PTM_RATIO,
                          CC_DEGREES_TO_RADIANS(360), 30);
    }
    CHECK_GL_ERROR_DEBUG();
    
    kmGLPopMatrix();
}

void TerrainSprite::drawSegment(Vec2 p1, Vec2 p2)
{
    
}