//
//  DNASprite.h
//  nanatrip
//
//  Created by Macbook Air on 11/3/15.
//
//

#ifndef __nanatrip__DNASprite_
#define __nanatrip__DNASprite_

#include "NanaTrip.h"
#include "SpriteWithBody.h"

class DNASprite : public SpriteWithBody {
public:
    static void checkDNAs(std::vector<SpriteWithBody *> &sprites, b2World *world, float topY);
    static void makeToast(SpriteWithBody *tracker, std::string);
};

#endif /* DNASprite_h */
