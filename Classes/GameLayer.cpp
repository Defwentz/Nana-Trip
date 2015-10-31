#include "GameLayer.h"
#include "OverLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameLayer::createScene()
{
    // initalize the global stuff in NanaTrip.h
    initStatistic();
    
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
    //gameStatus = GAME_PAUSE;
    //CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("BGMusic01.mp3", true);
    initListeners();
    initBG();
    initPhysics();
    isCounting2Destroy = false;
    
    _drawNode = DrawNode::create();
    this->addChild(_drawNode, ZORDER_HANDDRAW);
}

GameLayer::~GameLayer()
{
    NotificationCenter::getInstance()->removeObserver(this, "pause_sign");
    NotificationCenter::getInstance()->removeObserver(this, "defaultCallback");
    delete _debugDraw;
    CC_SAFE_DELETE(_world);
}

// Enable Accelerometer and attach listener, enable touch and attach listeners.
void GameLayer::initListeners() {

    Device::setAccelerometerEnabled(true);
    // creating an accelerometer event
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameLayer::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);
    touchlistener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    touchlistener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
    touchlistener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    NotificationCenter::getInstance()->
    addObserver(this, callfuncO_selector(GameLayer::pauseCallback), "pause_sign", NULL);
    NotificationCenter::getInstance()->
    addObserver(this, callfuncO_selector(GameLayer::pauseOverCallback), "defaultCallback", NULL);
}
void GameLayer::initBG() {
    if(IS_DEBUGGING)return;
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
void GameLayer::updateBG(float topY, float bottomY) {
    if(IS_DEBUGGING)return;
    if(_bgSprites.front()->getPosition().y > topY) {
        _bgSprites.front()->removeFromParent();
        _bgSprites.erase(_bgSprites.begin());
    }
    Vec2 bgsbackPos = _bgSprites.back()->getPosition();
    if(bgsbackPos.y - after_height/2 > bottomY) {
        Sprite *bg = Sprite::create("bg_1.png");
        bg->setScale(winSiz.width/1080.0);
        bg->setPosition(winMidX, bgsbackPos.y - after_height);
        this->addChild(bg, ZORDER_BG);
        _bgSprites.push_back(bg);
    }
}

void GameLayer::initPhysics()
{
    _world = new b2World(stdGrav);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    _debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(_debugDraw);
    
    uint32 flags = 0;
    if(IS_DEBUGGING)
        flags += b2Draw::e_shapeBit;
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
    
//    {
//        b2Vec2 center = b2Vec2(winMidX/PTM_RATIO, (winMidY+100)/PTM_RATIO);
//        
//        b2Vec2 vertices[4];
//        vertices[0].Set(-1, -1);
//        vertices[1].Set(-1, 1);
//        vertices[2].Set(1, 1);
//        vertices[3].Set(1, -1);
//        b2PolygonShape shell;
//        shell.Set(vertices, 4);
//        
//        b2BodyDef bodyDef;
//        bodyDef.type = b2_dynamicBody;
//        bodyDef.position = center + b2Vec2(-1, 0);
//        //+ b2Vec2(0.4*cosf(next_theta - deltaAngle/2),
//        // 0.4*sinf(next_theta - deltaAngle/2))
//        //);
//        // don't know exact effect
//        bodyDef.linearDamping = 0.1f;
//        bodyDef.gravityScale = 1;
//        
//        b2Body *body1 = _world->CreateBody(&bodyDef);
//        body1->CreateFixture(&shell, 1.0f);
//        
//        bodyDef.position = center + b2Vec2(1, 0);
//        b2Body *body2 = _world->CreateBody(&bodyDef);
//        body2->CreateFixture(&shell, 1.0f);
//        
//        // Connect the outside
//        b2RevoluteJointDef rjointDef;
//        rjointDef.Initialize(body1, body2, center + b2Vec2(0, 1));
//        rjointDef.collideConnected = true;
//        // do not work
//        if(false) {
//            rjointDef.upperAngle = 0 * b2_pi;
//            rjointDef.lowerAngle = -7/6.0 * b2_pi;//-210*M_PI/180;
//            rjointDef.enableLimit = true;
//            rjointDef.maxMotorTorque = 10.0f;
//            rjointDef.motorSpeed = 0.0f;
//            rjointDef.enableMotor = true;
//        }
//        
//        _world->CreateJoint(&rjointDef);
//    }
    
    _nana = NanaSprite::create(_world);
    _nana->setPosition(winMidX, winMidY);
    auto follow = Follow::create(_nana);
    this->runAction(follow);
    this->addChild(_nana, ZORDER_NANA);
    
    _terrain = TerrainSprite::create(_world);
    this->addChild(_terrain, ZORDER_TERRAIN);
    
    b2BodyDef bd;
    bd.position.SetZero();
    bd.type = b2_staticBody;
    _drawBody = _world->CreateBody(&bd);
    
    scheduleUpdate();
}

void GameLayer::update(float dt)
{
    if(gameStatus == GAME_PAUSE) {
        return;
    }
    else if(gameStatus == GAME_OVER) {
        gameStatus = GAME_PAUSE;
        this->_infoLayer->removeFromParent();
        this->unscheduleUpdate();
        Device::setAccelerometerEnabled(false);
        //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        utils::captureScreen(CC_CALLBACK_2(GameLayer::captureScreenCallback, this), "dead.png");
        scheduleOnce(schedule_selector(GameLayer::gameOver), 0.2f);
        return;
    }
    
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
    
    updateBG(topY, bottomY);
    
    // update score
    pos_score = (-nanaPos.y + winMidY)/400;
    _infoLayer->update();
    _nana->setPosition(nanaPos);
    //log("%f\n", nanaPos.y);
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


void GameLayer::gameOver(float dt) {
//    RenderTexture *screen = RenderTexture::create(winSiz.width, winSiz.height);
//    screen->setPosition(winMidX, winMidY);
//    screen->begin();
//    Director::getInstance()->getRunningScene()->visit();
//    screen->end();
//    screen->saveToFile(deadScreen);
    //this->_infoLayer->unscheduleUpdate();
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    this->getParent()->addChild(OverLayer::create(), ZORDER_OVERLAYER);
    //Director::getInstance()->replaceScene(OverLayer::createScene());
}

void GameLayer::captureScreenCallback(bool succeed, const std::string &filename) {
    if(succeed) {
        deadScreen = filename;
//        auto sp = Sprite::create(filename);
//        Director::getInstance()->getRunningScene()->addChild(sp, 10);
//        sp->setPosition(winMidX, winMidY);
//        sp->setScale(0.25);
    }
}

// problematic, TODO
void GameLayer::reset()
{
    // 好像并没有什么用
    //glClear(GL_COLOR_BUFFER_BIT);
    _terrain->removeFromParent();
    _nana->removeFromParent();
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
    this->initPhysics();
    //Director::getInstance()->resume();
    gameStatus = GAME_PLAY;
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event) {
    if(isCounting2Destroy || _drawVertices.size() != 0)
        return true;
    _drawVertices.push_back(this->convertTouchToNodeSpace(touch));
    return true;
}
void GameLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
    if(isCounting2Destroy || _drawVertices.size() == 0)
        return;
    Vec2 cTouchLoc = this->convertTouchToNodeSpace(touch);
    addPoint(cTouchLoc);
}
void GameLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if(isCounting2Destroy || _drawVertices.size() == 0)
        return;
    Vec2 cTouchLoc = this->convertTouchToNodeSpace(touch);
    addPoint(cTouchLoc);
    
    isCounting2Destroy = true;
    scheduleOnce(schedule_selector(GameLayer::destroyDrawFixtures), _drawLast);
}

void GameLayer::addPoint(Vec2 newPoint) {
    // get chain shape
    b2ChainShape chain;
    b2Vec2 vt[] = {vToB2(_drawVertices.back()), vToB2(newPoint)};
    //log("(%f,%f)-(%f,%f)", vt[0].x, vt[0].y, vt[1].x, vt[1].y);
    chain.CreateChain(vt, 2);
    
    // draw
    _drawNode->drawSegment(_drawVertices.back(), newPoint, 7.0f, _drawColor);
    _drawVertices.push_back(newPoint);
    _drawFixtures.push_back(_drawBody->CreateFixture(&chain, 1.0f));
}

void GameLayer::destroyDrawFixtures(float dt) {
    _drawVertices.clear();
    for(std::vector<b2Fixture *>::iterator i = _drawFixtures.begin();
        i != _drawFixtures.end();) {
        _drawBody->DestroyFixture((*i));
        i = _drawFixtures.erase(i);
    }
    _drawNode->clear();
    //log("%f,%f", vToB2(_drawVertices.back()).x, vToB2(_drawVertices.back()).y);
    isCounting2Destroy = false;
}

void GameLayer::onAcceleration(Acceleration *acc, Event *event)
{
    _nana->ApplyForce(b2Vec2(acc->x * 5, 0));
    _world->SetGravity(stdGrav + b2Vec2(acc->x * 15, 0));
}
void GameLayer::pauseCallback(cocos2d::Ref *pSender) {
    _eventDispatcher->removeEventListener(keyListener);
}
void GameLayer::pauseOverCallback(cocos2d::Ref *pSender) {
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}
void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch(keyCode)
    {
            //监听返回键
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            pauseCallback(NULL);
            _infoLayer->pauseCallback(NULL, cocos2d::ui::Widget::TouchEventType::ENDED);
            break;
            //监听menu键
        case EventKeyboard::KeyCode::KEY_MENU:
            break;
    }
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