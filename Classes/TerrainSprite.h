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
//#include "HelloWorldScene.h"
//#include "TerrainNode.h"

class TerrainSprite : public cocos2d::Sprite
{
public:
    static TerrainSprite *create();
    
    TerrainSprite();
    
    void initPhysics(b2World *_world);
    int spawnTerrain();
    void spawnTunnel(int r);
    
    void connectEdge(cocos2d::Vec2 p1, cocos2d::Vec2 p2, int isLeft);
    void doVertices(cocos2d::Vec2 p1, cocos2d::Vec2 p2, void (*func)(cocos2d::Vec2 p1, cocos2d::Vec2 p2));
    
    int randWithBase(int base, int addon);
    
    void update(float nanaY);
    
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags)
    override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t transformFlags);
    void drawSegment(cocos2d::Vec2 p1, cocos2d::Vec2 p2);
    
    static b2Vec2 vToB2(cocos2d::Vec2 v);
    static cocos2d::Vec2 b2ToV(b2Vec2 b);
protected:
    //HelloWorld *_mparent;
    b2World *_world;
    b2Body *_body;
    
    std::vector<std::vector<b2Fixture *>> lfixtures;
    std::vector<std::vector<b2Fixture *>> rfixtures;
    
    std::vector<cocos2d::Vec2> lvertices;
    std::vector<cocos2d::Vec2> rvertices;
    int lto, rto;
    
    cocos2d::CustomCommand _customCommand;
};

#endif /* defined(__nanatrip__TerrainSprite__) */
