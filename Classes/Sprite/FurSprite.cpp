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

void FurSprite::setup(b2World *world, b2Body *ground, b2Vec2 root, int isRight, int length) {
    this->setPosition(b2ToV(root));
    _isRight = isRight;
    // __ __ __
    //   |  |  |
    // __|__|__|
    float width = length / PTM_RATIO / (float)parts;
    float hheight = 40. / PTM_RATIO / 2.;
    int sign = -2 * isRight + 1;
    float round_edge_radius = hheight;
    b2Vec2 round_edge_p;
    
    b2Filter filter = b2Filter();
    filter.groupIndex = -1;
    
    for(int i = 0; i < parts; i++) {
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
        if(i == parts-1) {
            vertices[2].x -= round_edge_radius;
            vertices[3].x -= round_edge_radius;
            round_edge_p = b2Vec2(vertices[2].x, 0);
        }
        hheight = vertices[2].y;
        
        stick.Set(vertices, 4);
        auto fix = body->CreateFixture(&stick, 0.2f);
        fix->SetFilterData(filter);
        _bodies.push_back(body);
    }
    
    b2CircleShape round_edge;
    round_edge.m_p = round_edge_p;
    round_edge.m_radius = round_edge_radius;
    auto fix = _bodies.back()->CreateFixture(&round_edge, 0.2f);
    fix->SetFilterData(filter);
    
    b2DistanceJointDef jointDef;
    for(int i = 1; i < parts; i++) {
        // Get the current body and the neighbor
        b2Body *currentBody = _bodies[i-1];
        b2Body *neighborBody = _bodies[i];
        
        b2Vec2 joint_1 = root + b2Vec2(sign*i*width, hheight);
        b2Vec2 joint_2 = root + b2Vec2(sign*i*width, -hheight);
        
        // Connect
        jointDef.Initialize(currentBody, neighborBody,
                            joint_1,
                            joint_1 );
        jointDef.length = 0.6;
        jointDef.collideConnected = true;
        jointDef.frequencyHz = 4.0f;
        jointDef.dampingRatio = 1.f;
        
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

void FurSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

    Vec2 midvertices[4];
    for (int i = 0; i < _bodies.size(); i++) {
        Vec2 root = b2ToV(_bodies[i]->GetPosition()) - this->getPosition();
        b2PolygonShape *shape = (b2PolygonShape *) _bodies[i]->GetFixtureList()->GetShape();
        if(i == parts-1) {
            shape = (b2PolygonShape *) _bodies[i]->GetFixtureList()->GetNext()->GetShape();
        }
        Vec2 vertices[4] = {
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(3))),
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(0))),
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(1))),
            root + b2ToV(_bodies[i]->GetWorldVector(shape->GetVertex(2)))
        };

        DrawPrimitives::drawSolidPoly(vertices, 4, Color4F(0.47265625, 0.9296875, 0.89453125, 1));
        
        if(_isRight == 0) {
            if(i == 0) {
                midvertices[0] = vertices[1];
                midvertices[1] = vertices[2];
            } else {
                midvertices[2] = vertices[3];
                midvertices[3] = vertices[0];
                DrawPrimitives::drawSolidPoly(midvertices, 4, Color4F(0.47265625, 0.9296875, 0.89453125, 1));
                midvertices[0] = vertices[1];
                midvertices[1] = vertices[2];
            }
        } else {
            if(i == 0) {
                midvertices[0] = vertices[3];
                midvertices[1] = vertices[0];
            } else {
                midvertices[2] = vertices[1];
                midvertices[3] = vertices[2];
                DrawPrimitives::drawSolidPoly(midvertices, 4, Color4F(0.47265625, 0.9296875, 0.89453125, 1));
                midvertices[0] = vertices[3];
                midvertices[1] = vertices[0];
            }
        }
        ccDrawColor4F(0.47265625, 0.9296875, 0.89453125, 1);
        if(i == parts-1) {
            b2CircleShape *cshape = (b2CircleShape *) _bodies[i]->GetFixtureList()->GetShape();
            DrawPrimitives::drawSolidCircle
            (root + b2ToV(_bodies[i]->GetWorldVector(cshape->m_p)),
             cshape->m_radius*PTM_RATIO, 360, 12, 1, 1);
        } else if (i == 0) {
            DrawPrimitives::drawSolidCircle
            (root, 20, 360, 12, 1, 1);
        }
    }
    
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}