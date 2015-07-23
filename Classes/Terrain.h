//
//  Terrain.h
//  nanatrip
//
//  Created by Macbook Air on 7/20/15.
//
//

#ifndef __nanatrip__Terrain__
#define __nanatrip__Terrain__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include <vector>

class TerrainNode : public cocos2d::Node
{
public:
    static TerrainNode *create();
    bool init();
    void appendTerrain();
    void createNarrow();
    void createBumps();
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
        override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    void drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
    void update();
    void updateEdge(b2World *_world);
    void setEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, b2World *_world);
    
    int randWithBase(int base, int addon);
    
    std::vector<cocos2d::Vec2> lvertices;
    std::vector<cocos2d::Vec2> rvertices;
    
     b2Body *_body;
protected:
    
    cocos2d::CustomCommand _customCommand;
    
};

#endif /* defined(__nanatrip__Terrain__) */
