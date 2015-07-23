//
//  TerrainSprite.h
//  nanatrip
//
//  Created by Macbook Air on 7/22/15.
//
//

#ifndef __nanatrip__TerrainSprite__
#define __nanatrip__TerrainSprite__

#include "NanaTrip.h"
#include "TerrainNode.h"

class TerrainSprite : public cocos2d::Sprite
{
public:
    static TerrainSprite *create();
    
    TerrainSprite();
    
    void initPhysics(b2World *_world);
    int spawnTerrain();
    void spawnTunnel(int r);
    
    void update();
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    void drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
protected:
    b2World *_world;
    b2Body *_body;
    
    std::vector<TerrainNode> nodes;
    
    cocos2d::CustomCommand _customCommand;
};

#endif /* defined(__nanatrip__TerrainSprite__) */
