#include "GameLayer.h"
#include "OverLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameLayer::createScene()
{
    // initalize the global stuff in NanaTrip.h
    initStatistic();
    gameStatus = GAME_PLAY;
    
    auto scene = Scene::create();
    
    auto infoLayer = InfoLayer::create();
    scene->addChild(infoLayer, ZORDER_INFOLAYER);
    
    auto layer = GameLayer::create(infoLayer);
    scene->addChild(layer, ZORDER_GAMELAYER);
    
    return scene;
}

GameLayer *GameLayer::create(InfoLayer *infoLayer)
{
    GameLayer *ret = new (std::nothrow) GameLayer();
    if (ret && ret->init())
    {
        ret->_infoLayer = infoLayer;
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

GameLayer::GameLayer()
{
    initListeners();
    initBG();
    initPhysics();
}

GameLayer::~GameLayer()
{
    Device::setAccelerometerEnabled(false);
    delete _debugDraw;
    CC_SAFE_DELETE(_world);
}

void GameLayer::initListeners() {
    Device::setAccelerometerEnabled(true);
    // creating an accelerometer event
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameLayer::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);
    touchlistener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
}
void GameLayer::initBG()
{
    after_height = winSiz.width*848/1080;
    int n = ceil(winSiz.height / after_height);
    for(int i = 0; i < n; i++) {
        Sprite *bg = Sprite::create("bg_1.png");
        bg->setScale(winSiz.width/1080.0);
        bg->setPosition(winMidX, i*after_height + after_height/2);
        this->addChild(bg, ZORDER_BG);
        _bgSprites.push_back(bg);
    }
}

void GameLayer::initPhysics()
{
    //b2_velocityThreshold = 0.1f;
    
    _world = new b2World(b2Vec2(0.0f, -8.0f));
    // Do we want to let bodies sleep?
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    _debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(_debugDraw);
    
    uint32 flags = 0;
//        flags += b2Draw::e_shapeBit;
//        flags += b2Draw::e_jointBit;
    //    flags += b2Draw::e_aabbBit;
    //    flags += b2Draw::e_pairBit;
    //    flags += b2Draw::e_centerOfMassBit;
    //    flags += b2Draw::e_particleBit;
    _debugDraw->SetFlags(flags);
    /*{
        // Define the ground body.
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0, 0); // bottom-left corner

        // Call the body factory which allocates memory for the ground body
        // from a pool and creates the ground box shape (also from a pool).
        // The body is also added to the world.
        b2Body* groundBody = _world->CreateBody(&groundBodyDef);

        // Define the ground box shape.
        b2EdgeShape groundBox;

        auto s = Director::getInstance()->getWinSize();
        // bottom
        groundBox.Set(b2Vec2(0/PTM_RATIO,0/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0/PTM_RATIO));
        groundBody->CreateFixture(&groundBox,0);

        // top
        groundBox.Set(b2Vec2(0/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
        groundBody->CreateFixture(&groundBox,0);

        // left
        groundBox.Set(b2Vec2(0/PTM_RATIO,0/PTM_RATIO), b2Vec2(0/PTM_RATIO,s.height/PTM_RATIO));
        groundBody->CreateFixture(&groundBox,0);

        // right
        groundBox.Set(b2Vec2(s.width/PTM_RATIO,0/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
        groundBody->CreateFixture(&groundBox,0);
    }*/
    
    _terrain = TerrainSprite::create(_world);
    this->addChild(_terrain, ZORDER_TERRAIN);
    
    _nana = NanaSprite::create(_world);
    auto follow = Follow::create(_nana);
    this->runAction(follow);
    this->addChild(_nana, ZORDER_NANA);
    
    scheduleUpdate();
}

void GameLayer::update(float dt)
{
    if(gameStatus == GAME_PAUSE || gameStatus == GAME_OVER) return;
    
    // It is recommended that a fixed time step is used with Box2D for stability
    // of the simulation, however, we are using a variable time step here.
    // You need to make an informed choice, the following URL is useful
    // http://gafferongames.com/game-physics/fix-your-timestep/
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    _world->Step(dt, velocityIterations, positionIterations);
    _nana->gasUp();
    
    Vec2 nanaPos = _nana->getPosition();
    float topY = nanaPos.y + winSiz.height;
    float bottomY = nanaPos.y - winSiz.height;
    if(_bgSprites.front()->getPosition().y > topY) {
        _bgSprites.front()->removeFromParent();
        _bgSprites.erase(_bgSprites.cbegin());
    }
    Vec2 bgsbackPos = _bgSprites.back()->getPosition();
    if(bgsbackPos.y - after_height/2 > bottomY) {
        Sprite *bg = Sprite::create("bg_1.png");
        bg->setScale(winSiz.width/1080.0);
        bg->setPosition(winMidX, bgsbackPos.y - after_height);
        this->addChild(bg, ZORDER_BG);
        _bgSprites.push_back(bg);
    }
    
    // update score
    pos_score = (-nanaPos.y + winMidY)/400;
    _infoLayer->update();
    _nana->setPosition(nanaPos);
    _terrain->update(nanaPos.y);
    
    /*for(b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
    {
        if(b->GetUserData()!=NULL)
        {
            //CCLOG("ab");
            Sprite *sprite=(Sprite*)b->GetUserData();
            
            sprite->setPosition(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO);
            sprite->setRotation((-1)*CC_RADIANS_TO_DEGREES(b->GetAngle()));
            //CCLOG("%f,%f",sprite->getPositionX(),sprite->getPositionY());
            
        }
    }*/
}

void GameLayer::gameOver() {
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    Director::getInstance()->replaceScene(OverLayer::createScene());
}

// problematic, TODO
void GameLayer::reset()
{
    // 好像并没有什么用
    //glClear(GL_COLOR_BUFFER_BIT);
    _terrain->removeFromParent();
    //delete _terrain;
    _nana->removeFromParent();
    //delete _nana;
    delete _world;
    delete _debugDraw;
    
    for(int i = 0; i < _bgSprites.size(); i++) {
        _bgSprites[i]->removeFromParent();
    }
    _bgSprites.clear();
    initBG();
    
    pos_score = 0;
    eat_score = 0;
    _infoLayer->reset();
    //delete _nana;
    //delete _debugDraw;
    //CC_SAFE_DELETE(_world);
    this->initPhysics();
    //Director::getInstance()->resume();
    gameStatus = GAME_PLAY;
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    if(touchLocation.y < winMidY - 50) {
        _nana->ApplyForce(b2Vec2((touchLocation.x - _nana->getPosition().x)/20,
                                 (touchLocation.y - winMidY)/20));
    }
    else if(touchLocation.y > winMidY + 50) {
        _nana->ApplyForce(b2Vec2(0, 7));
    }
    return true;
}

void GameLayer::onAcceleration(Acceleration *acc, Event *event)
{
    _nana->ApplyForce(b2Vec2(acc->x * 2, 0));
}

// Draw
void GameLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t transformFlags)
{
    Layer::draw(renderer, transform, transformFlags);
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    _world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}