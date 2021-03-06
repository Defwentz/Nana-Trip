//
//  Box2DHelper.h
//  nanatrip
//
//  Created by Macbook Air on 10/24/15.
//
//

#ifndef __nanatrip__Box2DHelper__
#define __nanatrip__Box2DHelper__

#include <Box2D/Box2D.h>
#include <vector>

class Box2DHelper
{
public:
    /**
     *  @brief	get b2Body.
     *
     *  @param 	_world 	b2World
     *  @param 	pos 	position of the circle in b2
     *  @param 	radius 	radius of the circle in b2
     */
    static b2Body* getStaticBall(b2World* _world, b2Vec2 pos, float radius);
    /**
     *  @brief	get b2Body with shape and type.
     *
     *  @param 	_world 	b2World
     *  @param 	shape 	b2CircleShap
     *  @param 	type 	b2_static or b2_dynamic or else
     */
    static b2Body* getBallWithShape(b2World* _world, b2CircleShape *shape, b2BodyType type);
};

#endif /* defined(__nanatrip__Box2DHelper__) */
