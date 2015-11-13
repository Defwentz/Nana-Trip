//
//  NanaSprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/19/15.
//
//

#include "NanaSprite.h"

USING_NS_CC;

#define NUM_SEGMENTS 12

/*
b2Body *NanaSprite::createPhysicsObject(b2World *world) {
    
    // Center is the position of circle that is in the center
    b2Vec2 center = b2Vec2(240/PTM_RATIO, 260/PTM_RATIO);
    
    b2CircleShape circleShape;
    circleShape.m_radius = 0.25f;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 0.1;
    fixtureDef.restitution = 0.05;
    fixtureDef.friction = 1.0;
    
    // The greater the number, the more springy
    float springiness = 4.0;
    
    // Delta angle to step by
    float deltaAngle = (2.f * M_PI) / NUM_SEGMENTS;
    
    // Radius of the wheel
    float radius = 50;
    
    // Need to store the bodies so that we can refer back
    // to it when we connect the joints
    std::vector<b2Body *> bodies;
    
    // For each segment...
    for (int i = 0; i < NUM_SEGMENTS; i++) {
        // Current angle
        float theta = deltaAngle*i;
        
        // Calcualte the x and y based on theta
        float x = radius*cosf(theta);
        float y = radius*sinf(theta);
        
        // Remember to divide by PTM_RATIO to convert to Box2d coordinates
        b2Vec2 circlePosition = b2Vec2(x/PTM_RATIO, y/PTM_RATIO);
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        // Position should be relative to the center
        bodyDef.position = (center + circlePosition);
        
        // Create the body and fixture
        b2Body *body;
        body = world->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        
        // Add the body to the array to connect joints to it
        // later. b2Body is a C++ object, so must wrap it
        // in NSValue when inserting into it NSMutableArray
        
        bodies.push_back(body);//[bodies addObject:[NSValue valueWithPointer:body]];
    }
    
    // Circle at the center (inner circle)
    b2BodyDef innerCircleBodyDef;
    innerCircleBodyDef.type = b2_dynamicBody;
    // Position is at the center
    innerCircleBodyDef.position = center;
    b2Body *innerCircleBody = world->CreateBody(&innerCircleBodyDef);
    innerCircleBody->CreateFixture(&fixtureDef);
    
    // Connect the joints
    b2DistanceJointDef jointDef;
    for (int i = 0; i < NUM_SEGMENTS; i++) {
        // The neighbor.
        int neighborIndex = (i + 1) % NUM_SEGMENTS;
        
        // Get the current body and the neighbor
        b2Body *currentBody = bodies.at(i);//(b2Body*)[[bodies objectAtIndex:i] pointerValue];
        b2Body *neighborBody = bodies.at(neighborIndex);//(b2Body*)[[bodies objectAtIndex:neighborIndex] pointerValue];
        
        // Connect the outer circles to each other
        jointDef.Initialize(currentBody, neighborBody,
                            currentBody->GetWorldCenter(),
                            neighborBody->GetWorldCenter() );
        jointDef.collideConnected = true;
        jointDef.frequencyHz = springiness*10;
        jointDef.dampingRatio = 0.5f;
        
        world->CreateJoint(&jointDef);
        
        // Connect the center circle with other circles
        jointDef.Initialize(currentBody, innerCircleBody, currentBody->GetWorldCenter(), center);
        jointDef.collideConnected = true;
        jointDef.frequencyHz = springiness;
        jointDef.dampingRatio = 0.5;
        
        world->CreateJoint(&jointDef);
    }
    return innerCircleBody;
}
*/

NanaSprite* NanaSprite::create(b2World *world)
{
    NanaSprite* pRet = new NanaSprite(world);
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

NanaSprite::NanaSprite(b2World *world){
    _nanaFace = Director::getInstance()->getTextureCache()->addImage("nana_face_6.png");
    _nanaFace->retain();
    initPhysics(world);
}
NanaSprite::~NanaSprite()
{
    _nanaFace->release();
}

void NanaSprite::initPhysics(b2World *world)
{
    // Center of the circle, set to the center of the screen.
    b2Vec2 center = b2Vec2(winMidX/PTM_RATIO, winMidY/PTM_RATIO);
    if(IS_DEBUGGING_TERRAIN) {
        center = b2Vec2(-winMidX/PTM_RATIO, winMidY/PTM_RATIO);
    }
    
    /*b2CircleShape circleShape;
    circleShape.m_radius = 0.25f;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 0.1;
    fixtureDef.restitution = 0.05;
    fixtureDef.friction = 1.0;*/
    
    // Delta angle to step by
    float deltaAngle = (2.f * M_PI) / NUM_SEGMENTS;

    // Radius of the wheel
    //float radius = screenSiz.width*0.0016f;//1.2f;
    //float inner_radius = screenSiz.width*0.0012f;//0.9f;
    
    // Need to store the bodies so that we can refer back
    // to it when we connect the joints
    std::vector<b2Body *> bodies;
    
    // Calcualte the x and y based on theta
    float ox = radius*cosf(0);
    float oy = radius*sinf(0);
    float ix = inner_radius*cosf(0);
    float iy = inner_radius*sinf(0);
    float next_theta = 0;
    
    ////////////////////////////////////
    /*
    b2CircleShape circleShape;
    circleShape.m_radius = 0.25f;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 0.1;
    fixtureDef.restitution = 0.05;
    fixtureDef.friction = 1.0;
    
    // Circle at the center (inner circle)
    b2BodyDef innerCircleBodyDef;
    innerCircleBodyDef.type = b2_dynamicBody;
    // Position is at the center
    innerCircleBodyDef.position = center;
    b2Body *innerCircleBody = world->CreateBody(&innerCircleBodyDef);
    innerCircleBody->CreateFixture(&fixtureDef);
    */
    ////////////////////////////////////
    
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
        bodyDef.position = center;
                            //+ b2Vec2(0.4*cosf(next_theta - deltaAngle/2),
                                  // 0.4*sinf(next_theta - deltaAngle/2))
                            //);
        // don't know exact effect
        bodyDef.linearDamping = 0.f;
        
        b2Body *body = world->CreateBody(&bodyDef);
        body->CreateFixture(&shell, 1.0f);
        body->SetUserData(new Entity(UD_NANA));
        bodies.push_back(body);
        
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
        b2Body *currentBody = bodies.at(i);
        b2Body *neighborBody = bodies.at(neighborIndex);
        
        // Next angle
        next_theta += deltaAngle;
        
        // Calcualte the x and y based on theta
        float nox = radius*cosf(next_theta);
        float noy = radius*sinf(next_theta);
        float nix = inner_radius*cosf(next_theta);
        float niy = inner_radius*sinf(next_theta);
        b2Vec2 outer_joint = center + b2Vec2(nox, noy);
        b2Vec2 inner_joint = center + b2Vec2(nix, niy);
        
        // Connect the outside
        rjointDef.Initialize(currentBody, neighborBody, outer_joint);
        if(enableUnbreakable) {
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
        jointDef.dampingRatio = 0.f;
        
        world->CreateJoint(&jointDef);
        /*
        // Connect the center circle with other circles
        jointDef.Initialize(currentBody, innerCircleBody, currentBody->GetWorldCenter(), center);
        jointDef.collideConnected = true;
        jointDef.frequencyHz = 2.0f;//springiness;
        jointDef.dampingRatio = 0.5f;
        
        world->CreateJoint(&jointDef);*/
    }
    _bodies = bodies;
}

bool NanaSprite::isNana(b2Body *body)
{
    for(int i = 0; i < _bodies.size(); i++) {
        if(body == _bodies[i])
            return true;
    }
    return false;
}

Vec2 NanaSprite::getPosition0()
{
    return Vec2(winMidX, _bodies[0]->GetPosition().y*PTM_RATIO);
}

float NanaSprite::getpy() {
    float py;
    for(int i = 0; i < _bodies.size(); i++) {
        py += _bodies[i]->GetPosition().y;
    }
    py = py/_bodies.size()*PTM_RATIO;
    return py;
}
float NanaSprite::getpx() {
    float px;
    for(int i = 0; i < _bodies.size(); i++) {
        px += _bodies[i]->GetPosition().x;
    }
    px = px/_bodies.size()*PTM_RATIO;
    return px;
}

Vec2 NanaSprite::getCenter()
{
    float x = 0, y = 0;//, x2 =0,y2=0,x3=0,y3=0;
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

void NanaSprite::gasUp()
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

void NanaSprite::ApplyForce(b2Vec2 force)
{
    for(int i = 0; i < _bodies.size(); i++) {
        _bodies[i]->ApplyForce(force, _bodies[i]->GetPosition(), true);
    }
}


// Draw

void NanaSprite::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(NanaSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void NanaSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
{
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
    ccDrawColor4F(0.347656f, 0.68f, 0.8086f, 1);
    Vec2 center = this->getCenter();
    nanap = this->convertToWorldSpace(center);
    //log("nanapos: %f, %f\n", nanap.x, nanap.y);
    // tiny side burns (...Guesss that one way to call it)
    if(!IS_DEBUGGING)
    for(int i = 0; i < _bodies.size(); i++) {
        Vec2 target = (center - vertices[i]) * nub_pos;
        target += center;
        DrawPrimitives::drawSolidCircle(vertices[i], nub_size, CC_DEGREES_TO_RADIANS(360), 30);
    }
    
    // draw the body
    if(!IS_DEBUGGING)
    DrawPrimitives::drawSolidPoly(vertices, NUM_SEGMENTS, _nanaColor);
    
    //center.x-=12;center.y-=12;
    _nanaFace->drawAtPoint(center + Vec2(face_offset, face_offset));
    
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}