#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->initPhysics();
    
    // add layer as a child to scene
    scene->addChild(layer);

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
    /*
     Device::setAccelerometerEnabled(true);
     auto accListener = EventListenerAcceleration::create([](Acceleration *acc, Event *event){
     log("x = %f, y = %f", acc->x, acc->y);
     //_world->SetGravity();
     });
     _eventDispatcher->addEventListenerWithSceneGraphPriority(accListener, this);
     
     */

    return true;
}

HelloWorld::~HelloWorld()
{
    delete _debugDraw;
    delete _world;
}

void HelloWorld::initPhysics()
{
    
    
    _world = new b2World(b2Vec2(0.0f, -11.0f));
    
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
    /*
    {
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
    
    nanaSprite = NanaSprite::create();
    this->addChild(nanaSprite);
    nanaSprite->initPhysics(_world);
    
    terrain = TerrainNode::create();
    {
        b2BodyDef bd;
        bd.position.SetZero();
        bd.type = b2_staticBody;
        b2Body *_body = _world->CreateBody(&bd);
        terrain->_body = _body;
        
        for(int i = 1; i < terrain->lvertices.size(); i++) {
            Vec2 lp1, lp2;
            lp1 = terrain->lvertices[i - 1];
            lp2 = terrain->lvertices[i];
            terrain->setEdge(lp1, lp2, _world);
        }
        for(int i = 1; i < terrain->rvertices.size(); i++) {
            Vec2 rp1, rp2;
            rp1 = terrain->rvertices[i - 1];
            rp2 = terrain->rvertices[i];
            terrain->setEdge(rp1, rp2, _world);
        }
        
        _body->SetLinearVelocity(b2Vec2(0, 5));
    }
    this->addChild(terrain);
    
    auto follow = Follow::create(nanaSprite);
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
    //terrain->update();
    //terrain->updateEdge(_world);
    nanaSprite->gasUp();
    nanaSprite->setPosition(nanaSprite->getPosition());
    //terrain->runAction(MoveBy::create(1.0f, Vec2(0, 100)));
    
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
/*
b2Vec2 &HelloWorld::getNormal(b2Vec2 src) {
    static int nscale = 20;
    b2Vec2 dst = b2Vec2(src.y, -src.x);
    dst.x *= nscale;
    dst.y *= nscale;
}*/

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    //bounce();
    //auto touchLocation = touch->getLocation();
    //auto nanaLoc = nana->GetCenter();
    //nana->ApplyLinearImpulse(b2Vec2(-(nanaLoc.x*32 - touchLocation.x)/2, -(nanaLoc.y*32 - touchLocation.y)/2)); //ApplyForce(b2Vec2(1, 1));
    //auto nodePosition = convertToNodeSpace( touchLocation );
    //log("Box2DView::onTouchBegan, pos: %f,%f, \n%f, %f", touchLocation.x, touchLocation.y, nanaLoc.x, nanaLoc.y);//, nodePosition.x, nodePosition.y);
    return true;
}
void HelloWorld::onAcceleration(Acceleration *acc, Event *event)
{
    nanaSprite->ApplyForce(b2Vec2(acc->x * 2, 0));
    //nana->ApplyForce(b2Vec2(acc->x * 20, 0), nana->GetPosition(), true);
    //log("x = %f, y = %f", acc->x, acc->y);
    //_world->SetGravity();
}

/*void HelloWorld::bounce()
{
    b2Vec2 impulse = b2Vec2(-nana->GetMass() * 10, nana->GetMass() * 150);
    b2Vec2 impulsePoint = nana->GetPosition();
    nana->ApplyLinearImpulse(impulse, impulsePoint, true);
}*/
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
