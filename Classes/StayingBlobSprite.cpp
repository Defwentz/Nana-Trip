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
StayingBlobSprite::StayingBlobSprite() {

}

Vec2 StayingBlobSprite::getPosition()
{
    return Vec2(winMidX, _bodies[0]->GetPosition().y*PTM_RATIO);
}

Vec2 StayingBlobSprite::getCenter()
{
    float x = 0, y = 0;//, x2 =0,y2=0,x3=0,y3=0;
    
    //    for(int i = 0; i < _bodies.size(); i++) {
    //        b2Vec2 pos = _bodies[i]->GetWorldCenter();
    //        b2Vec2 pos2 = _bodies[i]->GetPosition();
    //        b2Vec2 pos3 = _bodies[i]->GetLocalCenter();
    //        x += pos.x;
    //        y += pos.y;
    //        x2+=pos2.x;y2+=pos2.y;
    //        x3+=pos3.x;y3+=pos3.y;
    //    }
    //    Vec2 vertices[NUM_SEGMENTS];
    
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

void StayingBlobSprite::gasUp()
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


// Draw

void StayingBlobSprite::draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(StayingBlobSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void StayingBlobSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags)
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
    
    // tiny side burns (...Guesss that one way to call it)
    for(int i = 0; i < _bodies.size(); i++) {
        Vec2 target = (center - vertices[i]) * nub_pos;
        target += center;
        DrawPrimitives::drawSolidCircle(vertices[i], nub_size, CC_DEGREES_TO_RADIANS(360), 30);
    }
    
    // draw the body
    DrawPrimitives::drawSolidPoly(vertices, NUM_SEGMENTS, _nanaColor);
    
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}