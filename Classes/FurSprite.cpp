//
//  FurSprite.cpp
//  nana
//
//  Created by Macbook Air on 2/26/16.
//
//

#include "FurSprite.h"
FurSprite* FurSprite::create()
{
    FurSprite *sprite = new (std::nothrow) FurSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void FurSprite::setup(b2World *world, b2Body *ground, b2Vec2 root, int length, int isRight) {
    this->setPosition(b2ToV(root));
    // __ __ __
    //   |  |  |
    // __|__|__|
    float width = length / PTM_RATIO / 3.;
    float hheight = 30. / PTM_RATIO / 2.;
    int sign = -2 * isRight + 1;
    
    for(int i = 0; i < 3; i++) {
        b2BodyDef bd;
        bd.gravityScale = 0;
        bd.position = root;
        bd.type = b2_dynamicBody;
        b2Body *body = world->CreateBody(&bd);
        
        b2PolygonShape stick;
        b2Vec2 vertices[4] = {
            b2Vec2(sign*i*width, -hheight),
            b2Vec2(sign*i*width, hheight),
            b2Vec2(sign*(i*width+width), hheight),
            b2Vec2(sign*(i*width+width), -hheight)
        };
        stick.Set(vertices, 4);
        body->CreateFixture(&stick, 0.2f);   // trying something
        _bodies.push_back(body);
    }
    b2DistanceJointDef jointDef;
    for(int i = 1; i < 3; i++) {
        // Get the current body and the neighbor
        b2Body *currentBody = _bodies[i-1];
        b2Body *neighborBody = _bodies[i];
        
        b2Vec2 joint_1 = root + b2Vec2(sign*i*width, hheight);
        b2Vec2 joint_2 = root + b2Vec2(sign*i*width, -hheight);
        
        // Connect
        jointDef.Initialize(currentBody, neighborBody,
                            joint_1,
                            joint_1 );
        jointDef.collideConnected = true;
        jointDef.frequencyHz = 4.0f;
        jointDef.dampingRatio = 0.5f;
        
        _joints.push_back((b2DistanceJoint*)(world->CreateJoint(&jointDef)));

        jointDef.Initialize(currentBody, neighborBody,
                            joint_2,
                            joint_2 );
        
        _joints.push_back((b2DistanceJoint*)(world->CreateJoint(&jointDef)));
    }
    b2RevoluteJointDef rjointDef;
    rjointDef.Initialize(ground, _bodies[0], root);
    if(true) {
        rjointDef.collideConnected = true;
        rjointDef.upperAngle = 0 * b2_pi;
        rjointDef.lowerAngle = -7/6.0 * b2_pi;
        rjointDef.enableLimit = true;
    }
    
    _rjoint = (b2RevoluteJoint *) world->CreateJoint(&rjointDef);
}
FurSprite::~FurSprite(){}
void FurSprite::selfDestruct(b2World *world){
    for(int i = 0; i < _joints.size(); i++) {
        world->DestroyJoint(_joints[i]);
    }
    _joints.clear();
    for(int i = 0; i < _bodies.size(); i++) {
        world->DestroyBody(_bodies[i]);
    }
    _bodies.clear();
}
void FurSprite::update(){}

// Draw

void FurSprite::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(FurSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

//Vec2 FurSprite::getPosition() {
//    return b2ToV(_bodies[0]->GetPosition());
//}

void FurSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{
//    Vec2 vertices[12];
//    Vec2 root = b2ToV(_bodies[0]->GetPosition());
//    
//    
//    int i = 0;
//    for(int j = 0; j < 3; j++) {
//        b2PolygonShape *shape = (b2PolygonShape *) _bodies[j]->GetFixtureList()->GetShape();
//        b2Vec2 dp = _bodies[j]->GetWorldVector(shape->GetVertex(3));
//        log("v0: %f, %f", _bodies[j]->GetWorldVector(shape->GetVertex(0)).x, _bodies[j]->GetWorldVector(shape->GetVertex(0)).y);
//        log("v1: %f, %f", _bodies[j]->GetWorldVector(shape->GetVertex(1)).x, _bodies[j]->GetWorldVector(shape->GetVertex(1)).y);
//        log("v2: %f, %f", _bodies[j]->GetWorldVector(shape->GetVertex(2)).x, _bodies[j]->GetWorldVector(shape->GetVertex(2)).y);
//        log("v3: %f, %f", dp.x, dp.y);
//        vertices[i++] = root + b2ToV(dp);
//        dp = _bodies[j]->GetWorldVector(shape->GetVertex(0));
//        vertices[i++] = root + b2ToV(dp);
//    }
//    for(int j = 2; j > -1; j--) {
//        b2PolygonShape *shape = (b2PolygonShape *) _bodies[j]->GetFixtureList()->GetShape();
//        b2Vec2 dp = _bodies[j]->GetWorldVector(shape->GetVertex(1));
//        vertices[i++] = root + b2ToV(dp);
//        dp = _bodies[j]->GetWorldVector(shape->GetVertex(2));
//        vertices[i++] = root + b2ToV(dp);
//    }
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
//
//    //glLineWidth( 5.0f );
//    //ccDrawColor4F(1.f, 0.125f, 0.15625f, 1);
//    // tiny side burns (...Guesss that one way to call it)
//    //    for(int i = 0; i < _bodies.size(); i++) {
//    //        Vec2 target = (center - vertices[i]) * nub_pos;
//    //        target += center;
//    //        DrawPrimitives::drawSolidCircle(vertices[i], nub_size, CC_DEGREES_TO_RADIANS(360), 30);
//    //    }
//    
//    // draw the body
//    DrawPrimitives::drawSolidPoly(vertices, 12, Color4F(0.9453125, 0.546875, 0.33984375, 1));
    for (int i = 0; i < _bodies.size(); i++) {
        Vec2 root = b2ToV(_bodies[i]->GetPosition()) - this->getPosition();
        b2PolygonShape *shape = (b2PolygonShape *) _bodies[i]->GetFixtureList()->GetShape();
        Vec2 vertices[4] = {
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(3))),
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(0))),
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(1))),
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(2)))
        };
        DrawPrimitives::drawSolidPoly(vertices, 4, Color4F(0.9453125, 0.546875, 0.33984375, 1));
    }
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}