#include "GameLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameLayer::createScene()
{
    // initalize the global stuff in NanaTrip.h
    initWinSiz();
    initStatistic();
    
    auto scene = Scene::create();
    
    auto layer = GameLayer::create();
    scene->addChild(layer, 5);
    
    auto infoLayer = InfoLayer::create();
    scene->addChild(infoLayer, 4);

    return scene;
}

GameLayer *GameLayer::create()
{
    GameLayer *ret = new (std::nothrow) GameLayer();
    if (ret && ret->init())
    {
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
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);
    touchlistener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    eventDispatcher->addEventListenerWithFixedPriority(touchlistener, 1);
    setAccelerometerEnabled(true);
    
    initPhysics();
}

GameLayer::~GameLayer()
{
    delete _terrain;
    delete _nana;
    delete _debugDraw;
    CC_SAFE_DELETE(_world);
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
    //flags += b2Draw::e_shapeBit;
    //flags += b2Draw::e_jointBit;
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
    this->addChild(_terrain);
    
    _nana = NanaSprite::create(_world);
    this->addChild(_nana);
    
    auto follow = Follow::create(_nana);
    this->runAction(follow);
    
    scheduleUpdate();
}

void GameLayer::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    _world->Step(dt, velocityIterations, positionIterations);
    _nana->gasUp();
    
    Vec2 nanaPos = _nana->getPosition();
    // update score
    score = (-nanaPos.y + winMidY)/400;
    _nana->setPosition(nanaPos);
    _terrain->update(nanaPos.y);
    
    for(auto *contact = _world->GetContactList();contact; contact = contact->GetNext())
    {
        b2Body *bodyA = contact->GetFixtureA()->GetBody();
        b2Body *bodyB = contact->GetFixtureB()->GetBody();
        
        auto udA = (Entity *) bodyA->GetUserData();
        auto udB = (Entity *) bodyB->GetUserData();
        
        if(udA == NULL || udB == NULL)
            return;
        if(udA->type == UD_NANA && udB->type == UD_NANA)
            return;
        
        
        if(udA->type == UD_NANA || udB->type == UD_NANA) {
            if(udA->type == UD_BADGUY || udB->type == UD_BADGUY) {
                //reset();
                CCLOG("%f-----hit", _nana->getPosition().y);
            }
            if(udA->type == UD_DNA) {
                udA->type = UD_DESTROYED;
                dna++;
            }
            else {
                udB->type = UD_DESTROYED;
                dna++;
            }
        }

    }
    
    //m_world->Step(dt, 8, 1);
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

// problematic, TODO
void GameLayer::reset()
{
    Director::getInstance()->pause();
    // glClear并没有什么用
    glClear(GL_COLOR_BUFFER_BIT);
    delete _terrain;
    delete _nana;
    delete _debugDraw;
    CC_SAFE_DELETE(_world);
    this->initPhysics();
    Director::getInstance()->resume();
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
    //auto nanaLoc = nana->GetCenter();
    //nana->ApplyLinearImpulse(b2Vec2(-(nanaLoc.x*32 - touchLocation.x)/2, -(nanaLoc.y*32 - touchLocation.y)/2)); //ApplyForce(b2Vec2(1, 1));
    //auto nodePosition = convertToNodeSpace( touchLocation );
    //log("Box2DView::onTouchBegan, pos: %f,%f", touchLocation.x, touchLocation.y);//, nodePosition.x, nodePosition.y);
    return true;
}

void GameLayer::onAcceleration(Acceleration *acc, Event *event)
{
    _nana->ApplyForce(b2Vec2(acc->x * 5, 0));
}
// Draw

void GameLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t transformFlags)
{
    Layer::draw(renderer, transform, transformFlags);
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    _world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    kmGLPopMatrix();
}