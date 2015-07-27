#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

// needed
void HelloWorld::initWinSiz()
{
    winSiz = Director::getInstance()->getWinSize();
    winMidX = winSiz.width/2;
    winMidY = winSiz.height/2;
}

Scene* HelloWorld::createScene()
{
    initWinSiz();
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->initPhysics();
    
    // also needed
    score = 0;
    
    // add layer as a child to scene
    scene->addChild(layer, 5);
    
    auto infoLayer = InfoLayer::create();
    scene->addChild(infoLayer, 4);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");
    //addChild(rootNode);
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->setSwallowTouches(true);
    touchlistener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    eventDispatcher->addEventListenerWithFixedPriority(touchlistener, 1);
    setAccelerometerEnabled(true);

    return true;
}

HelloWorld::~HelloWorld()
{
    delete _terrain;
    delete _nana;
    delete _debugDraw;
    CC_SAFE_DELETE(_world);
}

void HelloWorld::initPhysics()
{
    //b2_velocityThreshold = 0.1f;
    
    _world = new b2World(b2Vec2(0.0f, -8.0f));
    // Do we want to let bodies sleep?
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    _debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
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
    
    _terrain = TerrainSprite::create();
    _terrain->initPhysics(_world);
    this->addChild(_terrain);
    
    _nana = NanaSprite::create();
    _nana->initPhysics(_world);
    this->addChild(_nana);
    
    auto follow = Follow::create(_nana);
    this->runAction(follow);
    
    scheduleUpdate();
    
}

void HelloWorld::update(float dt)
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
    score = -nanaPos.y;
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
        
        if((udA->type == UD_NANA || udB->type == UD_NANA)
           && (udA->type == UD_BADGUY || udB->type == UD_BADGUY)) {
            //reset();
            CCLOG("%f-----hit", _nana->getPosition().y);
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

// problematic
void HelloWorld::reset()
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

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    //auto touchLocation = touch->getLocation();
    //auto nanaLoc = nana->GetCenter();
    //nana->ApplyLinearImpulse(b2Vec2(-(nanaLoc.x*32 - touchLocation.x)/2, -(nanaLoc.y*32 - touchLocation.y)/2)); //ApplyForce(b2Vec2(1, 1));
    //auto nodePosition = convertToNodeSpace( touchLocation );
    //log("Box2DView::onTouchBegan, pos: %f,%f, \n%f, %f", touchLocation.x, touchLocation.y, nanaLoc.x, nanaLoc.y);//, nodePosition.x, nodePosition.y);
    return true;
}
void HelloWorld::onAcceleration(Acceleration *acc, Event *event)
{
    _nana->ApplyForce(b2Vec2(acc->x * 5, 0));
}
// Draw

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t transformFlags)
{
    Layer::draw(renderer, transform, transformFlags);
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    _world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    kmGLPopMatrix();
}