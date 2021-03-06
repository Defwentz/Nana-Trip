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
    terrainRdmr->add(ITEM_TUNNEL, 10);
    terrainRdmr->add(ITEM_BUMPS, 20);
    terrainRdmr->add(ITEM_CHESSBOARD, 15);
    terrainRdmr->add(ITEM_BELT, 30);
    terrainRdmr->add(ITEM_METEOR, 15);
    terrainRdmr->add(ITEM_POCKET, 3);
    
    // initalize the tunnel randomer
    tnlRdmr = new Randomer();
    tnlRdmr->add(ITEM_TUNNEL_NR, 10);
    tnlRdmr->add(ITEM_TUNNEL_BRD, 10);
    
    // initalize the bump randomer
    bumpRdmr = new Randomer();
    bumpRdmr->add(ITEM_BUMPS_1, 15);
    bumpRdmr->add(ITEM_BUMPS_2, 6);
    bumpRdmr->add(ITEM_BUMPS_3, 6);
    
//    crvRdmr = new Randomer();
//    crvRdmr->add(ITEM_CURVE_BL, 10);
//    crvRdmr->add(ITEM_CURVE_BR, 10);
//    crvRdmr->add(ITEM_CURVE_TL, 10);
//    crvRdmr->add(ITEM_CURVE_TR, 10);
    
    ballRdmr = new Randomer();
    //ballRdmr->add(ITEM_BALL_DNA, 9);
    ballRdmr->add(ITEM_BALL_METEOR, 7);
    ballRdmr->add(ITEM_BALL_BLOB, 8);
    ballRdmr->add(ITEM_BALL_BADGUY, 4);
    ballRdmr->add(ITEM_BALL_MOVER, 7);
    ballRdmr->add(ITEM_BALL_OBSTACLE, 6);
    ballRdmr->add(ITEM_BALL_SLOWER, 7);
    
    initPhysics(world);
}
TerrainSprite::~TerrainSprite()
{
    delete terrainRdmr;
    delete tnlRdmr;
    delete bumpRdmr;
//    delete crvRdmr;
    delete ballRdmr;
}

float TerrainSprite::getLastY()
{
    // ignore the situation when l/vertices[1] might be empty.
    if(vertices[0].back().y < vertices[1].back().y)
        return vertices[0].back().y;
    else
        return vertices[1].back().y;
}

void TerrainSprite::initPhysics(b2World *_world)
{
    this->_world = _world;
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    _body = _world->CreateBody(&bd);
    _body->SetUserData(new Entity(UD_FLOOR));
    
    // Define the ground box shape.
    b2EdgeShape border;
    
    Vec2 bl = Vec2(0, 0);
    Vec2 tl = Vec2(0, winSiz.height);
    Vec2 tr = Vec2(winSiz.width, winSiz.height);
    Vec2 br = Vec2(winSiz.width, 0);
    
    vertices[0].push_back(bl);
    vertices[1].push_back(br);
    
    // left
    border.Set(vToB2(bl), vToB2(tl));
    std::vector<b2Fixture *> _lfs;
    _lfs.push_back(_body->CreateFixture(&border,0));
    fixtures[0].push_back(_lfs);
    
    // right
    border.Set(vToB2(tr), vToB2(br));
    std::vector<b2Fixture *> _rfs;
    _rfs.push_back(_body->CreateFixture(&border,0));
    fixtures[1].push_back(_rfs);
    
    ///// bottom
//    border.Set(vToB2(bl), vToB2(br));
//    _body->CreateFixture(&border, 0);
    
    Vec2 vpos = Vec2(winMidX, winSiz.height + winMidX);
    b2CircleShape ball;
    ball.m_p = vToB2(vpos);
    ball.m_radius = winMidX/PTM_RATIO;
    if(!IS_DEBUGGING_WITHOUTCHASER) {
        badboss = RedSprite::create();
        badboss->setPosition(vpos);
        badboss->setup(_world, &ball, RedSprite::_chasing);
        this->addChild(badboss);
    }
    boss_sound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("lame_soundeffect.m4a", true);
    log("play boss_sound: %d", boss_sound);
    vpos = Vec2(winMidX, winSiz.height*0.75);
    b2CircleShape ball2;
    ball2.m_p = vToB2(vpos);
    ball2.m_radius = winSiz.height*0.10/PTM_RATIO;
    createBlob(vpos, &ball2);
    
    to[0] = 1;to[1] = 1;
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
        case ITEM_POCKET:       spawnPockect();break;
        default:break;
    }
}

void TerrainSprite::spawnBallshapething(cocos2d::Vec2 vpos, float vradius) {
    b2CircleShape shape;
    shape.m_p = vToB2(vpos);
    shape.m_radius = vradius/PTM_RATIO;
    switch (ballRdmr->getRandomItem()) {
        //case ITEM_BALL_DNA:             createDNA(vpos);break;
        case ITEM_BALL_METEOR:          shape.m_p.SetZero(); createMovingLittleGuy(vpos, &shape);break;
        case ITEM_BALL_BLOB:            createBlob(vpos, &shape);break;
        case ITEM_BALL_BADGUY:          createBadGuy(vpos, &shape, RedSprite::_moving);break;
        case ITEM_BALL_MOVER:           createMoverObstacle(vpos, shape.m_radius);break;
        case ITEM_BALL_OBSTACLE:        createBallObstacle(vpos, &shape, boolWithOdds(0.5));break;
        case ITEM_BALL_SLOWER:          createSlower(vpos, &shape, SlowerSprite::_bouncy);break;
        default:break;
    }
}

/**
 * which_side == 1, right
 * which_side == 0, left
 */
void TerrainSprite::spawnPockect(int which_side, int pocket_size)
{
    float lasty;
    bool need_manage = false;
    if(which_side == -1) {
        need_manage = true;
        which_side = (int)boolWithOdds(0.5);
        lasty = getLastY();
    } else {
        lasty = vertices[which_side].back().y;
    }
    float new_lasty = lasty - pocket_size;
    // 避难所固定宽 winMidX
    int pocket_x = -winMidX + which_side*winSiz.width*2;
    // 有pocket的那一边
    vertices[which_side].push_back(Vec2(pocket_x, lasty));
    vertices[which_side].push_back(Vec2(pocket_x, new_lasty));
    vertices[which_side].push_back(Vec2(which_side * winSiz.width, new_lasty));
    
    if(need_manage) {
        int other_side = !which_side;
        vertices[other_side].push_back(Vec2(other_side * winSiz.width, new_lasty));
    }
}

void TerrainSprite::spawnMeteor()
{
    float lastY = getLastY() - winMidY/32;// - winMidY/8;
    // the border vertices
    vertices[0].push_back(Vec2(0, lastY));
    vertices[1].push_back(Vec2(winSiz.width, lastY));
    
    b2CircleShape ball;
    ball.m_radius = DNA_B2RADIUS;
    {
        Vec2 fp = Vec2(winMidX, lastY);
        createMovingLittleGuy(fp, &ball);
    }
    {
        Vec2 fp = Vec2(winMidX*3/2, lastY);
        createMovingLittleGuy(fp, &ball);
    }
    {
        Vec2 fp = Vec2(winMidX/2, lastY);
        createMovingLittleGuy(fp, &ball);
    }
}

void TerrainSprite::spawnBelt()
{
    float lastY = getLastY();
    int half_length = random(narrowest_width*2, winMidX-narrowest_width);
    lastY -= half_length;
    // the border vertices
    vertices[0].push_back(Vec2(0, lastY-half_length));
    vertices[1].push_back(Vec2(winSiz.width, lastY-half_length));
    
    // calculate position
    Vec2 pos = Vec2(winMidX, lastY);
    b2Vec2 bpos = vToB2(pos);
    
    b2CircleShape ball;
    ball.m_p = bpos;
    ball.m_radius = (half_length - narrowest_width)/PTM_RATIO;
    float pos_odds_for_badguy = pos_score / 100.0;
    if(pos_odds_for_badguy > 1)
        pos_odds_for_badguy = 1;
    if(boolWithOdds(0.5 * pos_odds_for_badguy)) {
        createBadGuy(pos, &ball, RedSprite::_moving);
    }
    else if (boolWithOdds(0.5)) {
        createBlob(pos, &ball);
    }
    else {
        createBallObstacle(pos, &ball, true);
    }
}

const int //TUNNEL_KEYPOINT = 5,
TUNNEL_KP_DIST_IN_SCREEN = 3,
X_IN_SCREEN = 5,                     // different than others
ROW_WIDTH = 50, ROW_WIDTH_ADDON = 50;   // different than others
// TODO: tunnels with caves
void TerrainSprite::spawnTunnel()
{
    float lastY = getLastY();
    if(vertices[0].back().y > lastY)
        vertices[0].push_back(Vec2(0, lastY));
    if(vertices[1].back().y > lastY)
        vertices[1].push_back(Vec2(winSiz.width, lastY));
    
    int n = randWithBase(5, 3);
    int cave_count = random(0, n/3);
    float cave_odds = cave_count/(float)n;
    switch (tnlRdmr->getRandomItem()) {
        case ITEM_TUNNEL_NR:
            for(int i = 0; i < n; i++) {
                
                if(boolWithOdds(cave_odds)) {
                    int length = random(winSiz.height/8, winSiz.height/4);
                    int width = random(winSiz.width/8, winSiz.height/4);
                    int x = random(winSiz.width/4, winSiz.width*3/4);
                    lastY -= length;
                    vertices[0].push_back(Vec2(x - width/2, lastY));
                    vertices[1].push_back(Vec2(x + width/2, lastY));
                    createMoverObstacle(Vec2(x, lastY), width/2.0/PTM_RATIO);
                    continue;
                }
                int length = randWithBase(winSiz.height/TUNNEL_KP_DIST_IN_SCREEN,
                                          winSiz.height/TUNNEL_KP_DIST_IN_SCREEN);
                int width = randWithBase(narrowest_width, narrowest_width);
                int x = randWithBase(winSiz.width/X_IN_SCREEN, winSiz.width*3/X_IN_SCREEN);
                lastY -= length;
                createDNA(Vec2(x, lastY));
                vertices[0].push_back(Vec2(x - width/2, lastY));
                vertices[1].push_back(Vec2(x + width/2, lastY));
            }
            break;
        case ITEM_TUNNEL_BRD:
            for(int i = 0; i < n; i++) {
                if(boolWithOdds(cave_odds)) {
                    int length = random(winSiz.height/8, winSiz.height/4);
                    int width = random(winSiz.width/8, winSiz.height/4);
                    int x = random(winSiz.width/4, winSiz.width*3/4);
                    lastY -= length;
                    vertices[0].push_back(Vec2(x - width/2, lastY));
                    vertices[1].push_back(Vec2(x + width/2, lastY));
                    createMoverObstacle(Vec2(x, lastY), width/2.0/PTM_RATIO);
                    continue;
                }
                int length = randWithBase(winSiz.height/TUNNEL_KP_DIST_IN_SCREEN,
                                          winSiz.height/TUNNEL_KP_DIST_IN_SCREEN);
                int width = randWithBase(narrowest_width + ROW_WIDTH, narrowest_width + ROW_WIDTH_ADDON);
                int x = random(winSiz.width/5, winSiz.width*4/5);
                lastY -= length;
                createDNA(Vec2(x, lastY));
                vertices[0].push_back(Vec2(x - width/2, lastY));
                vertices[1].push_back(Vec2(x + width/2, lastY));
            }
            break;
        default:
            break;
    }
    lastY -= randWithBase(winSiz.height/TUNNEL_KP_DIST_IN_SCREEN,
                          winSiz.height/TUNNEL_KP_DIST_IN_SCREEN);
    
    Vec2 badguyPos = Vec2((vertices[0].back().x + 0)/2, (vertices[0].back().y + lastY)/2) + Vec2(narrowest_width, -narrowest_width);
    b2CircleShape ball;
    ball.m_p = vToB2(badguyPos);
    ball.m_radius = DNA_B2RADIUS;
    createBadGuy(badguyPos, &ball, RedSprite::_static);
    
    vertices[0].push_back(Vec2(0, lastY));
    vertices[1].push_back(Vec2(winSiz.width, lastY));
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
        {
            int n = rand()%5;
            
            float min_single_bump_length = winSiz.height / 3;
            float max_single_bump_length = winSiz.height;
            for(int i = 0; i < n; i++) {
                /** the first side **/
                
                //  _ lastY
                // |                } bump_peak_tdy  ]
                // |- bump_peak_y_1                  } bump_length
                // |_ bump_end_y_1                   ]
                int bump_length = random(min_single_bump_length, max_single_bump_length);
                int bump_peak_tdy = random(bump_length/2, bump_length*3/4);
                int bump_peak_tdx = random((float)narrowest_width, winSiz.width - narrowest_width);
                int bump_peak_y_1 = lastY - bump_peak_tdy;
                int bump_end_y_1 = lastY - bump_length;
                
                bool rightFirst = boolWithOdds(0.5);
                int which_side = (int)(rightFirst);
                int side_x = which_side * winSiz.width;
                int empty_space = vertices[which_side].back().y - lastY;
                if(empty_space > 0) {// 空隙哟
                    if(boolWithOdds(0.1)) {
                        spawnPockect(which_side, empty_space);
                    } else {
                        vertices[which_side].push_back(Vec2(side_x, lastY));
                    }
                }
                // push the vertices of a single side into vertices[]
                vertices[which_side].push_back(Vec2(which_side*(winSiz.width - 2*bump_peak_tdx) + bump_peak_tdx, bump_peak_y_1));
                vertices[which_side].push_back(Vec2(side_x, bump_end_y_1));
                
                float vpos_1_y = (3*lastY + bump_peak_y_1)/4.;
                float vradius = lastY - vpos_1_y;
                spawnBallshapething(Vec2(which_side*(2*vradius - winSiz.width) + winSiz.width - vradius, vpos_1_y), vradius);
                
                /** the other side **/
                int margin = random(0.f, bump_length - bump_peak_tdy + narrowest_width);
                //  _ bump_peak_y_1
                // |_ bump_begin_y_2} margin
                // |                } bump_peak_tdy  ]
                // |- bump_peak_y_2                  } bump_length
                // |_ bump_end_y_2                   ]
                
                // also, I think same total x,y difference will look better
                // than if they looked different
                int bump_begin_y_2 = bump_peak_y_1 - margin;
                int bump_peak_y_2 = bump_begin_y_2 - bump_peak_tdy;
                int other_side = (int)(!rightFirst);
                int other_side_x = other_side * winSiz.width;
                
                /** stuff in the middle **/
                float space_left = winMidX - bump_peak_tdx - narrowest_width;
                if(space_left > 0 && boolWithOdds(0.6)) {
                    spawnBallshapething(Vec2(winMidX, (bump_peak_y_1 + bump_peak_y_2)/2),
                                        space_left);
                }
                
                if(boolWithOdds(0.1)) {
                    spawnPockect(other_side, bump_peak_tdy + margin);
                    if(vertices[other_side].back().y > bump_begin_y_2)
                        vertices[other_side].push_back(Vec2(other_side_x, bump_begin_y_2));
                } else {
                    vertices[other_side].push_back(Vec2(other_side_x, bump_begin_y_2));
                }
                vertices[other_side].push_back(Vec2(other_side*(winSiz.width - 2*bump_peak_tdx) + bump_peak_tdx, bump_peak_y_2));
                // update lastY for the next pair of bump
                lastY = bump_begin_y_2 - bump_length;
                vertices[other_side].push_back(Vec2(other_side_x, lastY));
                
                float vradius_2 = (bump_end_y_1 + bump_peak_y_2)/2. - narrowest_width*2;
                if(vradius_2 < 0) {
                    log("bad");
                    continue;
                }
                float vpos_2_y = (bump_end_y_1 + bump_peak_y_2)/2.;
                spawnBallshapething(Vec2(other_side*(-winSiz.width) + winSiz.width, vpos_2_y), vradius_2);
            }
        }break;
        // 下面两种比较相似, 与上面这种不同
        // 左右不同
        case ITEM_BUMPS_2:
        {
            int min_wrinkle = random(1, 3);
            int max_wrinkle = random(3, 7);
            // 2-6个(半屏) * min_w-max_w个(起伏/半屏)
            int n = random(2, 6) * random(min_wrinkle, max_wrinkle);
            for(int i = 0; i < n; i++) {
                int dy = random(winMidY/max_wrinkle, winMidY/min_wrinkle);
                int dx = random(winMidX/4, winMidX - narrowest_width/2);
                vertices[0].push_back(Vec2(dx, lastY-dy));
                
                dy = random(winMidY/max_wrinkle, winMidY/min_wrinkle);
                dx = random(winMidX/3, winMidX - narrowest_width/2);
                vertices[1].push_back(Vec2(winSiz.width - dx, lastY-dy));
                
                lastY = getLastY();
            }
            int dy = random(winMidY/max_wrinkle, winMidY/min_wrinkle);
            vertices[0].push_back(Vec2(0, lastY - dy));
            vertices[1].push_back(Vec2(winSiz.width, lastY - dy));
        }break;
        // 左右相同
        case ITEM_BUMPS_3:
        {
            int min_wrinkle = random(1, 3);
            int max_wrinkle = random(3, 9);
            // 2-6个(半屏) * min_w-max_w个(起伏/半屏)
            int n = random(2, 6) * random(min_wrinkle, max_wrinkle);
            
            bool isFirst = true;
            Vec2 lastp;
            for(int i = 0; i < n; i++) {
                int dy = random(winMidY/max_wrinkle, winMidY/min_wrinkle);
                int dx = random(winMidX/3, winMidX - narrowest_width/2);
                vertices[0].push_back(Vec2(dx, lastY-dy));
                vertices[1].push_back(Vec2(winSiz.width - dx, lastY-dy));
                
                if(isFirst) {
                    isFirst = false;
                    lastp = vertices[0].back();
                    if(boolWithOdds(0.6))
                        createMoverObstacle(Vec2(winMidX, lastY), dx/2.0/PTM_RATIO);
                } else {
                    Vec2 mp = vertices[0].back();
                    if((winMidX - mp.x) > narrowest_width*2) {
                        float diffy = lastp.y - mp.y;
                        int howmany = diffy / 80;
                        float k = diffy/(lastp.x - mp.x);
                        float b = lastp.y - k*lastp.x;
                        log("diffy: %f; howmany: %d; k: %f, b:%f", diffy, howmany, k, b);
                        for(int j = 0; j < howmany; j++) {
                            float y = lastp.y - j*70;
                            float x = (y - b) / k;
                            int length = winMidX - x;
                            log("(%f, %f): %d", x, y, length);
                            if(length > 100) {
                                createFur(Vec2(x, y), 0);
                                createFur(Vec2(winSiz.width - x, y), 1);
                            }
                        }
                    }
                    lastp = mp;
                }
                
                lastY = getLastY();
            }
            int dy = random(winMidY/max_wrinkle, winMidY/min_wrinkle);
            vertices[0].push_back(Vec2(0, lastY - dy));
            vertices[1].push_back(Vec2(winSiz.width, lastY - dy));
        }break;
        default:break;
    }
}
// TODO: bug fix: some sort of empty space
void TerrainSprite::spawnChessboard()
{
    float lastY = getLastY();
    int length = randWithBase(winMidY,
                              winSiz.height*2);
    // the border vertices
    vertices[0].push_back(Vec2(0, lastY-length));
    vertices[1].push_back(Vec2(winSiz.width, lastY-length));
    
    //terrainTypes.push_back(TYPE_DOOR);
    int margin = randWithBase(narrowest_width, narrowest_width);
    
    int col = random(MIN_COL, MAX_COL);
    float max_radius = (winSiz.width / (col-1) - margin)/2;
    float min_radius = (winSiz.width / (MAX_COL-1) - margin)/2;
    int row = (margin + length) / (2*max_radius + margin);
    lastY -= max_radius;
    
    float odds = random(0.2, 1.);
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
            
            float pos_odds_for_badguy = pos_score / 100.0;
            if(pos_odds_for_badguy > 1)
                pos_odds_for_badguy = 1;
            if(boolWithOdds(0.5 * pos_odds_for_badguy)) {
                createBadGuy(pos, &ball, RedSprite::_moving);
            }
            else if(boolWithOdds(0.3)) {
                createSlower(pos, &ball, SlowerSprite::_bouncy);
            }
            else if(boolWithOdds(0.3)) {
                createMoverObstacle(pos, radius);
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
//    float lastY = getLastY();
//    int length = randWithBase(winMidY,
//                              winSiz.height);
//    // the border vertices
//    vertices[0].push_back(Vec2(0, lastY-length));
//    vertices[1].push_back(Vec2(winSiz.width, lastY-length));
//    
//    //int margin_x = randWithBase(narrowest_width, narrowest_width);
//    //int big_radius = random(winMidX - margin_x*2, winSiz.width - margin_x*2);
//    
//    switch (crvRdmr->getRandomItem()) {
//        case ITEM_CURVE_BR:
//            //Vec2 origin = Vec2(winSiz.width - , float yy);
//            break;
//        case ITEM_CURVE_BL:
//            break;
//        case ITEM_CURVE_TR:
//            break;
//        case ITEM_CURVE_TL:
//            break;
//        default:break;
//    }
    
}

void TerrainSprite::createDNA(cocos2d::Vec2 vpos)
{
    b2Body *body = Box2DHelper::getStaticBall(_world, vToB2(vpos), DNA_B2RADIUS);
    
    SpriteWithBody *guy;
    int which = rand_0_1() * 100;
    if(which < 25) {
        guy = SpriteWithBody::create("gold.png");
        guy->setScale(36/142.0);
        body->SetUserData(new Entity(UD_DNA_10));
    } else if (which < 50) {
        guy = SpriteWithBody::create("red.png");
        guy->setScale(36/407.5);
        body->SetUserData(new Entity(UD_DNA_5));
    } else if (which < 50) {
        guy = SpriteWithBody::create("eyeguy_72.png");
        body->SetUserData(new Entity(UD_DNA_20));
    } else {
        guy = SpriteWithBody::create("greenguy_72.png");
        body->SetUserData(new Entity(UD_DNA_15));
    }
    guy->setPosition(vpos);
    this->addChild(guy);
    dnas.push_back(guy);
    guy->_body = body;
}

void TerrainSprite::createMovingLittleGuy(cocos2d::Vec2 vpos, b2CircleShape *shape)
{
    b2BodyDef bd;
    bd.position = vToB2(vpos);
    bd.type = b2_dynamicBody;
    bd.gravityScale = -2;
    b2Body *b = _world->CreateBody(&bd);
    b->CreateFixture(shape, 0);
    littleguys.push_back(b);
}

void TerrainSprite::createBadGuy(cocos2d::Vec2 vpos, b2CircleShape *shape, int type)
{
    SpriteWithBody *redSprite = RedSprite::create();
    redSprite->setPosition(vpos);
    ((RedSprite *)redSprite)->setup(_world, shape, type);
    badguys.push_back(redSprite);
    this->addChild(redSprite);
}
void TerrainSprite::createBlob(cocos2d::Vec2 vpos, b2CircleShape *shape)
{
    SpriteWithBody *blob = StayingBlobSprite::create();
    blob->setPosition(vpos);
    ((StayingBlobSprite *)blob)->setup(_world, shape);
    blobs.push_back(blob);
    this->addChild(blob);
}

void TerrainSprite::createBallObstacle(cocos2d::Vec2 vpos, b2CircleShape *shape, bool withDNA)
{
    SpriteWithBody *guy = SpriteWithBody::create("block_5.png");
    guy->setPosition(vpos);
    obstacles.push_back(guy);
    
    guy->setScale(shape->m_radius*PTM_RATIO/145);
    this->addChild(guy);
    
    guy->_body = Box2DHelper::getBallWithShape(_world, shape, b2_staticBody);
    
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

void TerrainSprite::createSlower(cocos2d::Vec2 vpos, b2CircleShape *shape, int type) {
    SpriteWithBody *slower = SlowerSprite::create(type);
    ((SlowerSprite *)slower)->setup(_world, shape);
    this->addChild(slower);
    slowers.push_back(slower);
}

void TerrainSprite::createMoverObstacle(cocos2d::Vec2 vpos, float radius)
{
    SpriteWithBody *mover = MoverSprite::create();
    ((MoverSprite *)mover)->setup(_world, _body, vpos, radius);
    this->addChild(mover);
    movers.push_back(mover);
}
void TerrainSprite::createFur(cocos2d::Vec2 root, int isRight, int length)
{
    SpriteWithBody *fur = FurSprite::create();
    ((FurSprite *)fur)->setup(_world, _body, vToB2(root), isRight);
    this->addChild(fur);
    furs.push_back(fur);
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

void TerrainSprite::connectEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isRight)
{
    float tdy = p1.y - p2.y;
    std::vector<b2Fixture *> _fixtures;
    //////////////
    if(tdy == 0 || p1.x - p2.x == 0) {
        b2EdgeShape border;
        b2Vec2 _bp1 = vToB2(p1);
        b2Vec2 _bp2 = vToB2(p2);
        border.Set(_bp1, _bp2);
        _fixtures.push_back(_body->CreateFixture(&border,0));
        fixtures[isRight].push_back(_fixtures);
        return;
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
    fixtures[isRight].push_back(_fixtures);
}
void TerrainSprite::drawEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isRight)
{
    Texture2D *terrainTxture = Director::getInstance()->getTextureCache()->addImage("terrain_attempt_r.jpg");
    p1.y++;
    float tdy = p1.y - p2.y;
    //////////////
    
    float x = winSiz.width;
    if(!isRight) {
        x = -winSiz.width;
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
        
        Point vt[] = {
            Point(_p1.x + x, _p1.y),
            Point(_p1.x + x, _p2.y),
            _p1,
            _p2
        };
        float _p2c1 = _p1.x/1000, _p2c2 = _p2.x/1000;
        Point ct[] = {
            Point(_p2c1, 1),
            Point(_p2c2, 1),
            Point(_p2c1, 0),
            Point(_p2c2, 0)
        };
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
        terrainTxture->getGLProgram()->use();
        terrainTxture->getGLProgram()->setUniformsForBuiltins();
        GL::bindTexture2D(terrainTxture->getName());
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vt);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, ct);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        _p1 = _p2;
    }
}
void TerrainSprite::update(float nanaY)
{
    float topY = nanaY + winSiz.height;
    float bottomY = nanaY - winMidY;
    
    for(int n =0; n < 2; n++) {
        if (vertices[n][1].y > topY) {
            vertices[n].erase(vertices[n].begin());
            std::vector<b2Fixture *> _fixtures = fixtures[n].front();
            for(int i = 0; i < _fixtures.size(); i++) {
                _body->DestroyFixture(_fixtures[i]);
            }
            fixtures[n].erase(fixtures[n].begin());
            to[n]--;
        }
        for(int i = to[n]; i < vertices[n].size(); i++) {
            if(vertices[n][i - 1].y > bottomY) {
                connectEdge(vertices[n][i - 1], vertices[n][i], n);
                to[n]++;
            }else
                break;
        }
        if(to[n] == vertices[n].size())
            spawnTerrain();
    }
    
    // check if there are sprites with b2body out of sight
    // if so, delete
    spriteCheck(obstacles, topY);
    DNASprite::checkDNAs(dnas, _world, topY);
    
    
    if(badboss != NULL) {
        badboss->update();
        badboss->_body->SetLinearVelocity(b2Vec2(0, -6) + b2Vec2(0,-23*pos_score/200));
        if(badboss->getPosition().y > topY) {
            log("pause boss_sound");
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(boss_sound);
        } else {
            log("resume boss_sound");
            CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(boss_sound);
        }
        if(badboss->getPosition().y < bottomY - winMidY) {
            log("stop boss_sound");
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(boss_sound);
            badboss->selfDestruct(_world);
            badboss->removeFromParent();
            badboss = NULL;
        }
    }
    // another way: have it appear once in a while, maybe like a minute or so.
    // and with flashing title: WARNING!!, and a special bgm
    spriteCheckAndUpdate(badguys, topY);
    spriteCheckAndUpdate(movers, topY);
    spriteCheckAndUpdate(blobs, topY);
    spriteCheckAndUpdate(slowers, topY);
    spriteCheckAndUpdate(furs, topY);
    
    for(std::vector<b2Body *>::iterator i = littleguys.begin();
        i != littleguys.end();) {
        Vec2 p = b2ToV((*i)->GetPosition());
        if((*i)->GetPosition().y*PTM_RATIO > topY) {
            _world->DestroyBody(*i);
            i = littleguys.erase(i);
        }
        else {
            ++i;
        }
    }
    
    if(nanaY - vertices[0][0].y - winSiz.height - narrowest_width > 0) {
        gameStatus = GAME_INTERESTING;
    }
    
    if(pos_score < 100) {
        
    }
}

void TerrainSprite::spriteCheck(std::vector<SpriteWithBody *> &sprites, float topY)
{
    for(std::vector<SpriteWithBody *>::iterator i = sprites.begin();
        i != sprites.end();) {
        if((*i)->getPosition().y > topY) {
            SpriteWithBody::removeFromVector(sprites, i, _world);
        }
        else break;
    }
}
void TerrainSprite::spriteCheckAndUpdate(std::vector<SpriteWithBody *> &sprites, float topY) {
    for(std::vector<SpriteWithBody *>::iterator i = sprites.begin();
        i != sprites.end();) {
        if((*i)->getPosition().y > topY) {
            SpriteWithBody::removeFromVector(sprites, i, _world);
        }
        else {
            SpriteWithBody *sprite = (*i);
            sprite->update();
            ++i;
        }
    }
}

void TerrainSprite::noMorePockets() {
    terrainRdmr->updateOdds(ITEM_POCKET, 0);
}
// Draw

void TerrainSprite::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(TerrainSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

// drawing the same spot over and over again, bad idea.
// I think all the implementation are like this, yeah.
void TerrainSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    glLineWidth( 5.0f );
    ccDrawColor4F(0.94921875, 0.859375, 0.55078125, 1.0);
    for(int i = 0; i < littleguys.size(); i++) {
        Vec2 pos = b2ToV(littleguys[i]->GetPosition());
        ccDrawSolidCircle(b2ToV(littleguys[i]->GetPosition()),
                          littleguys[i]->GetFixtureList()->GetShape()->m_radius * PTM_RATIO,
                          CC_DEGREES_TO_RADIANS(360), 30);
    }
    if(!IS_DEBUGGING)
    for(int n = 0; n < 2; n++) {
        for(int i = 1; i < to[n]; i++) {
            Vec2 lp1, lp2;
            lp1 = vertices[n][i - 1];
            lp2 = vertices[n][i];
            drawEdge(lp1, lp2, n);
            //doVertices(lp1, lp2, ccDrawLine);
        }
    }
    CHECK_GL_ERROR_DEBUG();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}