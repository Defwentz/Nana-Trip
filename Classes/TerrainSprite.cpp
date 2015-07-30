//
//  TerrainSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/22/15.
//
//

#include "TerrainSprite.h"

USING_NS_CC;

TerrainSprite* TerrainSprite::create(b2World *world)
{
    TerrainSprite* pRet = new TerrainSprite(world);
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

TerrainSprite::TerrainSprite(b2World *world)
{
    // initalize the general terrain randomer
    terrainRdmr = new Randomer();
    terrainRdmr->add(ITEM_TUNNEL, 20);
    terrainRdmr->add(ITEM_BUMPS, 20);
    terrainRdmr->add(ITEM_CHESSBOARD, 5);
    terrainRdmr->add(ITEM_BELT, 10);
    terrainRdmr->add(ITEM_METEOR, 10);
    
    // initalize the tunnel randomer
    tnlRdmr = new Randomer();
    tnlRdmr->add(ITEM_TUNNEL_NR, 10);
    tnlRdmr->add(ITEM_TUNNEL_BRD, 10);
    
    // initalize the bump randomer
    bumpRdmr = new Randomer();
    bumpRdmr->add(ITEM_BUMPS_1, 30);
    bumpRdmr->add(ITEM_BUMPS_2, 0);
    bumpRdmr->add(ITEM_BUMPS_3, 0);
    
    crvRdmr = new Randomer();
    crvRdmr->add(ITEM_CURVE_BL, 10);
    crvRdmr->add(ITEM_CURVE_BR, 10);
    crvRdmr->add(ITEM_CURVE_TL, 10);
    crvRdmr->add(ITEM_CURVE_TR, 10);
    
    initPhysics(world);
}
TerrainSprite::~TerrainSprite()
{
    delete terrainRdmr;
    delete tnlRdmr;
    delete bumpRdmr;
    delete crvRdmr;
}

float TerrainSprite::getLastY()
{
    // ignore the situation when l/rvertices might be empty.
    if(lvertices.back().y < rvertices.back().y)
        return lvertices.back().y;
    else
        return rvertices.back().y;
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
    
    Vec2 bl = Vec2(0, 0);
    Vec2 tl = Vec2(0, winSiz.height);
    Vec2 tr = Vec2(winSiz.width, winSiz.height);
    Vec2 br = Vec2(winSiz.width, 0);
    
    lvertices.push_back(bl);
    rvertices.push_back(br);
    //terrainTypes.push_back(TYPE_DOOR);
    
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

void TerrainSprite::spawnTerrain()
{
    switch (terrainRdmr->getRandomItem()) {
        case ITEM_TUNNEL:       spawnTunnel();break;
        case ITEM_BUMPS:        spawnBumps();spawnMeteor();break;
        case ITEM_CHESSBOARD:   spawnChessboard();break;
        case ITEM_BELT:         spawnBelt();break;
        case ITEM_METEOR:       spawnMeteor();break;
        default:break;
    }
}

void TerrainSprite::spawnMeteor()
{
    float lastY = getLastY() - winMidY/8;
    // the border vertices
    lvertices.push_back(Vec2(0, lastY));
    rvertices.push_back(Vec2(winSiz.width, lastY));
    
    {
        b2CircleShape ball;
        ball.m_radius = DNA_B2RADIUS + 1;
        Vec2 fp = Vec2(winMidX, lastY);
        createMovingLittleGuy(fp, &ball);
    }
    {
        b2CircleShape ball;
        ball.m_radius = DNA_B2RADIUS + 1;
        Vec2 fp = Vec2(winMidX*3/2, lastY);
        createMovingLittleGuy(fp, &ball);
    }
    {
        b2CircleShape ball;
        ball.m_radius = DNA_B2RADIUS + 1;
        Vec2 fp = Vec2(winMidX/2, lastY);
        createMovingLittleGuy(fp, &ball);
    }
}

void TerrainSprite::spawnBelt()
{
    float lastY = getLastY();
    int half_length = random(winMidX/20, winMidX);
    lastY -= half_length;
    // the border vertices
    lvertices.push_back(Vec2(0, lastY-half_length));
    rvertices.push_back(Vec2(winSiz.width, lastY-half_length));
    
    // calculate position
    Vec2 pos = Vec2(winMidX, lastY);
    b2Vec2 bpos = vToB2(pos);
    
    b2CircleShape ball;
    ball.m_p = bpos;
    ball.m_radius = half_length/PTM_RATIO - DNA_B2RADIUS*2;
    
    createBallObstacle(pos, &ball, true);
}

const int TUNNEL_KEYPOINT = 5,
    TUNNEL_KP_DIST_IN_SCREEN = 3,
    X_IN_SCREEN = 5,
    NARROW_WIDTH = 45,                      // different than others
    ROW_WIDTH = 50, ROW_WIDTH_ADDON = 50;   // different than others
void TerrainSprite::spawnTunnel()
{
    float lastY = getLastY();
    if(lvertices.back().y > lastY)
        lvertices.push_back(Vec2(0, lastY));
    if(rvertices.back().y > lastY)
        rvertices.push_back(Vec2(winSiz.width, lastY));
    
    int n = randWithBase(TUNNEL_KEYPOINT, TUNNEL_KEYPOINT);
    switch (tnlRdmr->getRandomItem()) {
        case ITEM_TUNNEL_NR:
            for(int i = 0; i < n; i++) {
                int length = randWithBase(winSiz.height/TUNNEL_KP_DIST_IN_SCREEN,
                                          winSiz.height/TUNNEL_KP_DIST_IN_SCREEN);
                int width = randWithBase(NARROW_WIDTH, NARROW_WIDTH);
                int x = randWithBase(winSiz.width/X_IN_SCREEN, winSiz.width*3/X_IN_SCREEN);
                lastY -= length;
                
                lvertices.push_back(Vec2(x - width/2, lastY));
                rvertices.push_back(Vec2(x + width/2, lastY));
            }
            break;
        case ITEM_TUNNEL_BRD:
            for(int i = 0; i < n; i++) {
                int length = randWithBase(winSiz.height/TUNNEL_KP_DIST_IN_SCREEN,
                                          winSiz.height/TUNNEL_KP_DIST_IN_SCREEN);
                int width = randWithBase(NARROW_WIDTH + ROW_WIDTH, NARROW_WIDTH + ROW_WIDTH_ADDON);
                int x = random(winSiz.width/5, winSiz.width*4/5);
                lastY -= length;
                
                lvertices.push_back(Vec2(x - width/2, lastY));
                rvertices.push_back(Vec2(x + width/2, lastY));
            }
            break;
        default:
            break;
    }
    lastY -= randWithBase(winSiz.height/TUNNEL_KP_DIST_IN_SCREEN,
                          winSiz.height/TUNNEL_KP_DIST_IN_SCREEN);
    
    Vec2 badguyPos = Vec2((lvertices.back().x + 0)/2, (lvertices.back().y + lastY)/2) + Vec2(NARROW_WIDTH, -NARROW_WIDTH);
    b2CircleShape ball;
    ball.m_p = vToB2(badguyPos);
    ball.m_radius = DNA_B2RADIUS;
    createBadGuy(badguyPos, &ball);
    
    lvertices.push_back(Vec2(0, lastY));
    rvertices.push_back(Vec2(winSiz.width, lastY));
  }


// type 1   >
//            <
// type 2   > <
// type 3   >    <
//          >    <  like little hills on a mountain
void TerrainSprite::spawnBumps()
{
    float lastY = getLastY();
    
    switch (bumpRdmr->getRandomItem()) {
        case ITEM_BUMPS_1:
            int n = rand()%5;
            
            float min_bump_length = winSiz.height / 3;
            float max_bump_length = winSiz.height;
            for(int i = 0; i < n; i++) {
                // I think same total x,y difference will look better than different
                int bump_length = random(min_bump_length, max_bump_length);
                int bump_peak_tdy = random(bump_length/2, bump_length*2/3);
                int bump_peak_tdx = random((float)NARROW_WIDTH, winSiz.width - NARROW_WIDTH);
                int bump_peak_y_1 = lastY - bump_peak_tdy;
                int bump_end_y_1 = lastY - bump_length;
                
                bool leftFirst = boolWithOdds(0.5);
                if(leftFirst) {
                    if(lvertices.back().y > lastY)
                        lvertices.push_back(Vec2(0, lastY));
                    lvertices.push_back(Vec2(bump_peak_tdx, bump_peak_y_1));
                    lvertices.push_back(Vec2(0, bump_end_y_1));
                }
                else {
                    if(rvertices.back().y > lastY)
                        rvertices.push_back(Vec2(winSiz.width, lastY));
                    rvertices.push_back(Vec2(winSiz.width - bump_peak_tdx, bump_peak_y_1));
                    rvertices.push_back(Vec2(winSiz.width, bump_end_y_1));
                }
                
                lastY = bump_end_y_1;
                int margin = random(0, bump_peak_tdy - bump_peak_tdy + NARROW_WIDTH);
                int bump_begin_y_2 = bump_peak_y_1 - margin;
                int bump_peak_y_2 = bump_begin_y_2 - bump_peak_tdy;
                lastY = bump_begin_y_2 - bump_length;
                // lastY now is still the frist bump's door y
                if(leftFirst) {
                    rvertices.push_back(Vec2(winSiz.width, bump_begin_y_2));
                    rvertices.push_back(Vec2(winSiz.width - bump_peak_tdx, bump_peak_y_2));
                    rvertices.push_back(Vec2(winSiz.width, lastY));
                }
                else {
                    lvertices.push_back(Vec2(0, bump_begin_y_2));
                    lvertices.push_back(Vec2(bump_peak_tdx, bump_peak_y_2));
                    lvertices.push_back(Vec2(0, lastY));
                }
            }
            break;
//        case ITEM_BUMPS_2:
//            break;
//        case ITEM_BUMPS_3:
//            break;
//        default:break;
    }
}

//int max_radius = winSize.width / (MIN_COL-1) - margin;
//int min_radius = winSize.width / (MAX_COL-1) - margin;
// TODO: bug fix: some sort of empty space
void TerrainSprite::spawnChessboard()
{
    float lastY = getLastY();
    int length = randWithBase(winMidY,
                              winSiz.height*2);
    // the border vertices
    lvertices.push_back(Vec2(0, lastY-length));
    rvertices.push_back(Vec2(winSiz.width, lastY-length));
    
    //terrainTypes.push_back(TYPE_DOOR);
    int margin = randWithBase(NARROW_WIDTH, NARROW_WIDTH);
    
    int col = random(MIN_COL, MAX_COL);
    float max_radius = (winSiz.width / (col-1) - margin)/2;
    float min_radius = (winSiz.width / (MAX_COL-1) - margin)/2;
    int row = (margin + length) / (2*max_radius + margin);
    lastY -= max_radius;
    
    float odds = rand_0_1();
    for(int i = 0; i < row; i++) {
        float x = random(-max_radius/2, max_radius/2);
        for(int j = 0; j < col; j++, x += (2*max_radius + margin)) {
            // decide whether to use the spot or not, with odds
            if(rand_0_1() < odds)continue;
            
            float radius = random(min_radius, max_radius)/PTM_RATIO;
            
            // calculate position
            Vec2 pos = Vec2(x, lastY);
            b2Vec2 bpos = vToB2(pos);
            
            b2CircleShape ball;
            ball.m_p = bpos;
            ball.m_radius = radius;
            
            
            if(boolWithOdds(0.1)) {
                createBadGuy(pos, &ball);
            }
            else {
                createBallObstacle(pos, &ball, false);
            }
        }
        lastY -= (2*max_radius + margin);
    }
}

// using circle
// type bottom left    \
//             right   /
//      top left       /
//          right      \

void TerrainSprite::spawnCurve()
{
    float lastY = getLastY();
    int length = randWithBase(winMidY,
                              winSiz.height);
    // the border vertices
    lvertices.push_back(Vec2(0, lastY-length));
    rvertices.push_back(Vec2(winSiz.width, lastY-length));
    
    int margin_x = randWithBase(NARROW_WIDTH, NARROW_WIDTH);
    int big_radius = random(winMidX - margin_x*2, winSiz.width - margin_x*2);
    
    switch (crvRdmr->getRandomItem()) {
        case ITEM_CURVE_BR:
            //Vec2 origin = Vec2(winSiz.width - , float yy);
            break;
        case ITEM_CURVE_BL:
            break;
        case ITEM_CURVE_TR:
            break;
        case ITEM_CURVE_TL:
            break;
        default:break;
    }
    
}

void TerrainSprite::createDNA(cocos2d::Vec2 vpos)
{
    SpriteWithBody *guy = SpriteWithBody::create("gold.png");
    guy->setPosition(vpos);
    dnas.push_back(guy);
    guy->setScale(36/142.0);
    this->addChild(guy);
    
    b2Vec2 bpos = vToB2(vpos);
    b2CircleShape ball;
    ball.m_p = bpos;
    ball.m_radius = DNA_B2RADIUS;
    
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    b2Body *b = _world->CreateBody(&bd);
    b->SetUserData(new Entity(UD_DNA));
    b->CreateFixture(&ball, 0);
    guy->_body = b;
}

void TerrainSprite::createMovingLittleGuy(cocos2d::Vec2 vpos, b2CircleShape *shape)
{
    b2BodyDef bd;
    bd.position.Set(vpos.x/PTM_RATIO, vpos.y/PTM_RATIO);
    bd.type = b2_dynamicBody;
    b2Body *b = _world->CreateBody(&bd);
    b->CreateFixture(shape, 0);
    b->ApplyForce(b2Vec2(0, 16.0f), b->GetWorldCenter(), true);
    littleguys.push_back(b);
}

void TerrainSprite::createBadGuy(cocos2d::Vec2 vpos, b2CircleShape *shape)
{
    SpriteWithBody *guy = SpriteWithBody::create("red.png");
    guy->setPosition(vpos);
    badguys.push_back(guy);
    
    guy->setScale(shape->m_radius*PTM_RATIO/36);
    this->addChild(guy);
    
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    b2Body *b = _world->CreateBody(&bd);
    b->SetUserData(new Entity(UD_BADGUY));
    b->CreateFixture(shape, 0);
    guy->_body = b;
    
    guy->runAction(RepeatForever::create(RotateBy::create(7, 360)));
}

void TerrainSprite::createBallObstacle(cocos2d::Vec2 vpos, b2CircleShape *shape, bool withDNA)
{
    SpriteWithBody *guy = SpriteWithBody::create("block.png");
    guy->setPosition(vpos);
    obstacles.push_back(guy);
    
    guy->setScale(shape->m_radius*PTM_RATIO/145);
    this->addChild(guy);
    
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    b2Body *b = _world->CreateBody(&bd);
    b->CreateFixture(shape, 0);
    guy->_body = b;
    
    if(withDNA) {
        float vr = (shape->m_radius + DNA_B2RADIUS) * PTM_RATIO;
        float dtheta = 2*asin((DNA_B2RADIUS + 0.1) / shape->m_radius);
        int segment = (2.f * M_PI) / dtheta;
        float theta = 0;
        for(int i = 0; i < segment; i++, theta += dtheta) {
            createDNA(vpos + Vec2(vr*cosf(theta), vr*sinf(theta)));
        }
    }
}

void TerrainSprite::createMoverObstacle(cocos2d::Vec2 vpos, float radius)
{
    b2BodyDef bd;
    bd.position = vToB2(vpos);
    bd.type = b2_dynamicBody;
    b2Body *mover = _world->CreateBody(&bd);
    
    b2PolygonShape stick_1;
    float round_edge_radius = 10.0/PTM_RATIO;
    b2Vec2 vertices[4];
    vertices[0] = b2Vec2(-round_edge_radius, round_edge_radius - radius);
    vertices[1] = b2Vec2(-round_edge_radius, radius - round_edge_radius);
    vertices[2] = b2Vec2(round_edge_radius, radius - round_edge_radius);
    vertices[3] = b2Vec2(round_edge_radius, round_edge_radius - radius);
    stick_1.Set(vertices, 4);
    mover->CreateFixture(&stick_1, 0.2f);   // trying something
    {
        b2CircleShape round_edge;
        round_edge.m_p.Set(0, radius - round_edge_radius);
        round_edge.m_radius = round_edge_radius;
        mover->CreateFixture(&round_edge, 0.2f);
    }
    {
        b2CircleShape round_edge;
        round_edge.m_p.Set(0, - radius + round_edge_radius);
        round_edge.m_radius = round_edge_radius;
        mover->CreateFixture(&round_edge, 0.2f);
    }
    b2RevoluteJointDef jd;
    jd.Initialize(_body, mover, vToB2(vpos));
    _world->CreateJoint(&jd);
}

void TerrainSprite::doVertices(cocos2d::Vec2 p1, cocos2d::Vec2 p2,
                               void (*func)(const cocos2d::Vec2 &origin, const cocos2d::Vec2 &destination))
{
    float tdy = p1.y - p2.y;
    //////////////
    if(tdy == 0) {
        func(p1, p2);
    }
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
bool TerrainSprite::boolWithOdds(float odds)
{
    if(rand_0_1() < odds)
        return true;
    else
        return false;
}

void TerrainSprite::connectEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isLeft)
{
    float tdy = p1.y - p2.y;
    std::vector<b2Fixture *> _fixtures;
    //////////////
    if(tdy == 0) {
        b2EdgeShape border;
        b2Vec2 _bp1 = vToB2(p1);
        b2Vec2 _bp2 = vToB2(p2);
        border.Set(_bp1, _bp2);
        _fixtures.push_back(_body->CreateFixture(&border,0));
        if(isLeft)
            lfixtures.push_back(_fixtures);
        else
            rfixtures.push_back(_fixtures);
    };
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
        
        b2EdgeShape border;
        b2Vec2 _bp1 = vToB2(_p1);
        b2Vec2 _bp2 = vToB2(_p2);
        border.Set(_bp1, _bp2);
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
    float topY = nanaY + winMidY + winSiz.height/4;
    float bottomY = nanaY - winMidY - winSiz.height/4;
    
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
        //terrainTypes.erase(terrainTypes.cbegin());
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

    // check if there are sprites with b2body out of sight
    // if so, delete
    spriteCheck(obstacles, topY);
    spriteCheck(badguys, topY);
    
    for(std::vector<SpriteWithBody *>::iterator i = dnas.begin();
        i != dnas.end();) {
        auto ud = (Entity *) (*i)->_body->GetUserData();
        if(ud->type == UD_DESTROYED || (*i)->getPosition().y > topY) {
            _world->DestroyBody((*i)->_body);
            (*i)->removeFromParent();
            i = dnas.erase(i);
        }
        else
            ++i;
    }
    //spriteCheck(dnas, topY);
    
    for(std::vector<b2Body *>::iterator i = littleguys.begin();
        i != littleguys.end();) {
        Vec2 p = b2ToV((*i)->GetPosition());
        if((*i)->GetPosition().y*PTM_RATIO > topY) {
            _world->DestroyBody((*i));
            i = littleguys.erase(i);
        }
        else {
            (*i)->ApplyForce(b2Vec2(0, 16.0f), (*i)->GetWorldCenter(), true);
            ++i;
        }
    }
}

void TerrainSprite::spriteCheck(std::vector<SpriteWithBody *> &sprites, float topY)
{
    for(std::vector<SpriteWithBody *>::iterator i = sprites.begin();
        i != sprites.end();) {
        if((*i)->getPosition().y > topY) {
            _world->DestroyBody((*i)->_body);
            (*i)->removeFromParent();
            i = sprites.erase(i);
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

// drawing the same spot over and over again, bad idea.
// TODO: fix this
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
    
    // draw obstacles
//    for(int i = 0; i < nonoVertices.size(); i++) {
//        ccDrawSolidCircle(nonoVertices[i],
//                          nonos[i]->GetShape()->m_radius * PTM_RATIO,
//                          CC_DEGREES_TO_RADIANS(360), 30);
//    }
    ccDrawColor4F(0.9, 0.83, 0.62, 1.0);
    for(int i = 0; i < littleguys.size(); i++) {
        Vec2 pos = b2ToV(littleguys[i]->GetPosition());
        ccDrawSolidCircle(b2ToV(littleguys[i]->GetPosition()),
                          littleguys[i]->GetFixtureList()->GetShape()->m_radius * PTM_RATIO,
                          CC_DEGREES_TO_RADIANS(360), 30);
    }
    CHECK_GL_ERROR_DEBUG();
    kmGLPopMatrix();
}

void TerrainSprite::drawSegment(Vec2 p1, Vec2 p2)
{}