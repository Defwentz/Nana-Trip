//
//  StayingBlobSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 10/24/15.
//
//

#include "StayingBlobSprite.h"

#define NUM_SEGMENTS 12

StayingBlobSprite* StayingBlobSprite::create()
{
    StayingBlobSprite *sprite = new (std::nothrow) StayingBlobSprite();
    
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
StayingBlobSprite::~StayingBlobSprite() {
    _blobFace->release();
}
void StayingBlobSprite::setup(b2World *world, b2CircleShape *shape) {
    _blobFace = Director::getInstance()->getTextureCache()->addImage("blob_face_without.png");
    _blobFace->retain();
    
    // Delta angle to step by
    float deltaAngle = (2.f * M_PI) / NUM_SEGMENTS;
    
    // Radius of the wheel
    radius = shape->m_radius;
    inner_radius = radius*0.75;
    
    // Calcualte the x and y based on theta
    float ox = radius*cosf(0);
    float oy = radius*sinf(0);
    float ix = inner_radius*cosf(0);
    float iy = inner_radius*sinf(0);
    float next_theta = 0;
    
    for(int i = 0; i < NUM_SEGMENTS; i++) {
        // Next angle
        next_theta += deltaAngle;
        
        // Calcualte the x and y based on theta
        float nox = radius*cosf(next_theta);
        float noy = radius*sinf(next_theta);
        float nix = inner_radius*cosf(next_theta);
        float niy = inner_radius*sinf(next_theta);
        
        b2Vec2 vertices[4];
        vertices[0].Set(ix, iy);
        vertices[1].Set(ox, oy);
        vertices[2].Set(nox, noy);
        vertices[3].Set(nix, niy);
        b2PolygonShape shell;
        shell.Set(vertices, 4);
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = shape->m_p;
        //bodyDef.gravityScale = 0;
        
        // don't know exact effect
        bodyDef.linearDamping = 0.1f;
        
        b2Body *body = world->CreateBody(&bodyDef);
        body->CreateFixture(&shell, 1.0f);
        _bodies.push_back(body);
        
        ox = nox;
        oy = noy;
        ix = nix;
        iy = niy;
    }
    
    next_theta = 0;
    // Connect the joints
    b2DistanceJointDef jointDef;
    b2RevoluteJointDef rjointDef;
    for (int i = 0; i < NUM_SEGMENTS; i++) {
        // The neighbor.
        int neighborIndex = (i + 1) % NUM_SEGMENTS;
        
        // Get the current body and the neighbor
        b2Body *currentBody = _bodies.at(i);
        b2Body *neighborBody = _bodies.at(neighborIndex);
        
        // Next angle
        next_theta += deltaAngle;
        
        // Calcualte the x and y based on theta
        float nox = radius*cosf(next_theta);
        float noy = radius*sinf(next_theta);
        float nix = inner_radius*cosf(next_theta);
        float niy = inner_radius*sinf(next_theta);
        b2Vec2 outer_joint = shape->m_p + b2Vec2(nox, noy);
        b2Vec2 inner_joint = shape->m_p + b2Vec2(nix, niy);
        
        // Connect the outside
        rjointDef.Initialize(currentBody, neighborBody, outer_joint);
        if(true) {
            rjointDef.collideConnected = true;
            rjointDef.upperAngle = 0 * b2_pi;
            rjointDef.lowerAngle = -7/6.0 * b2_pi;
            rjointDef.enableLimit = true;
        }
        
        auto rj = world->CreateJoint(&rjointDef);
        _joints.push_back((b2RevoluteJoint *)rj);
        // Connect the inside
        jointDef.Initialize(currentBody, neighborBody,
                            inner_joint,
                            inner_joint );
        jointDef.collideConnected = true;
        jointDef.frequencyHz = 4.0f;
        jointDef.dampingRatio = 0.5f;
        
        world->CreateJoint(&jointDef);
    }

    
    // 中心静态小球 半径为总半径50%
    shape->m_radius *= 0.3;
    b2Body *body = Box2DHelper::getBallWithShape(world, shape, b2_staticBody);
    body->SetGravityScale(0);
    this->_body = body;
    
    nub_size = shape->m_radius*PTM_RATIO;
}

Vec2 StayingBlobSprite::getCenter()
{
    float x = 0, y = 0;
    
    Vec2 opos = this->getPosition();
    
    for(int i = 0; i < _bodies.size(); i++) {
        b2PolygonShape *shape = (b2PolygonShape *) _bodies[i]->GetFixtureList()->GetShape();
        b2Vec2 point;
        if(i < NUM_SEGMENTS / 4)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(0));
        else if(i < NUM_SEGMENTS * 2/4)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(1));
        else if(i < NUM_SEGMENTS * 3/4)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(2));
        else if(i < NUM_SEGMENTS)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(3));
        b2Vec2 pos = _bodies[i]->GetPosition();
        
        x += (point.x + pos.x)*PTM_RATIO - opos.x;
        y += (point.y + pos.y)*PTM_RATIO - opos.y;
    }
    x /= NUM_SEGMENTS;//(PTM_RATIO/NUM_SEGMENTS);
    y /= NUM_SEGMENTS;//(PTM_RATIO/NUM_SEGMENTS);
    return Vec2(x, y);
}

void StayingBlobSprite::update()
{
    for(int i = 0; i < _bodies.size(); i++) {
        b2PolygonShape *shape = (b2PolygonShape *) _bodies[i]->GetFixtureList()->GetShape();
        //CCLOG("%f, %f\n", bodies[i]->GetWorldVector(shape->GetVertex(1)).x , bodies[i]->GetWorldVector(shape->GetVertex(1)).y);
        b2Vec2 edge;
        if(i < NUM_SEGMENTS / 4)
            edge = _bodies[i]->GetWorldVector(shape->GetVertex(1) - shape->GetVertex(0));
        else if(i < NUM_SEGMENTS * 2/4)
            edge = _bodies[i]->GetWorldVector(shape->GetVertex(2) - shape->GetVertex(1));
        else if(i < NUM_SEGMENTS * 3/4)
            edge = _bodies[i]->GetWorldVector(shape->GetVertex(3) - shape->GetVertex(2));
        else if(i < NUM_SEGMENTS)
            edge = _bodies[i]->GetWorldVector(shape->GetVertex(0) - shape->GetVertex(3));
        b2Vec2 normal = b2Vec2(edge.y*hardness, -edge.x*hardness);
        _bodies[i]->ApplyForce(normal, _bodies[i]->GetPosition(), true);
    }
}

void StayingBlobSprite::selfDestruct(b2World *world) {
    SpriteWithBody::selfDestruct(world);
    for(int i = 0; i < _joints.size(); i++) {
        world->DestroyJoint(_joints[i]);
    }
    _joints.clear();
    for(int i = 0; i < _bodies.size(); i++) {
        world->DestroyBody(_bodies[i]);
    }
    _bodies.clear();
}

// Draw

void StayingBlobSprite::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(StayingBlobSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void StayingBlobSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{
    if(_bodies.size() == 0)return;
    Vec2 vertices[NUM_SEGMENTS];
    
    Vec2 opos = this->getPosition();
    
    for(int i = 0; i < _bodies.size(); i++) {
        b2PolygonShape *shape = (b2PolygonShape *) _bodies[i]->GetFixtureList()->GetShape();
        b2Vec2 point;
        if(i < NUM_SEGMENTS / 4)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(0));
        else if(i < NUM_SEGMENTS * 2/4)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(1));
        else if(i < NUM_SEGMENTS * 3/4)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(2));
        else if(i < NUM_SEGMENTS)
            point = _bodies[i]->GetWorldVector(shape->GetVertex(3));
        b2Vec2 pos = _bodies[i]->GetPosition();
        
        vertices[i].x = (point.x + pos.x)*PTM_RATIO - opos.x;
        vertices[i].y = (point.y + pos.y)*PTM_RATIO - opos.y;
    }
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    glLineWidth( 5.0f );
    ccDrawColor4F(0.1171875f, 0.125f, 0.15625f, 1);
    Vec2 center = this->getCenter();
    
    // tiny side burns (...Guesss that one way to call it)
    for(int i = 0; i < _bodies.size(); i++) {
        Vec2 target = (center - vertices[i]) * nub_pos;
        target += center;
        DrawPrimitives::drawSolidCircle(vertices[i], nub_size, CC_DEGREES_TO_RADIANS(360), 30);
    }
    
    // draw the body
    DrawPrimitives::drawSolidPoly(vertices, NUM_SEGMENTS, _nanaColor);
    
    Vec2 facepos = center + Vec2(face_offset, face_offset);
    _blobFace->drawAtPoint(facepos);
    //log("nanapos: %f, %f\n", nanap.x, nanap.y);
    Vec2 eh = this->convertToNodeSpace(nanap);
    Vec2 leftEye = facepos + Vec2(194, 287);
    Vec2 leftEyeRel =  (eh - Vec2(194 - 540 + facepos.x, -673));
    leftEyeRel = leftEyeRel/leftEyeRel.length()*37;
    //leftEyeRel.y*=-1;
    Vec2 rightEye = facepos + Vec2(416, 292);
    Vec2 rightEyeRel =  (eh - Vec2(418 - 540 + facepos.x, -670));
    rightEyeRel = rightEyeRel/rightEyeRel.length()*37;
    //rightEyeRel.y*=-1;
    log("left: %f, %f :: right: %f, %f\n", leftEyeRel.x, leftEyeRel.y, rightEyeRel.x, rightEyeRel.y);
    DrawPrimitives::drawSolidCircle(leftEye + leftEyeRel, 10, CC_DEGREES_TO_RADIANS(360), 30);
    DrawPrimitives::drawSolidCircle(rightEye + rightEyeRel, 10, CC_DEGREES_TO_RADIANS(360), 30);
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}