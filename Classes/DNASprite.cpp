//
//  DNASprite.cpp
//  nanatrip
//
//  Created by Macbook Air on 11/3/15.
//
//

#include "DNASprite.h"

void DNASprite::checkDNAs(std::vector<SpriteWithBody *> &sprites, b2World *world, float topY) {
    for(std::vector<SpriteWithBody *>::iterator i = sprites.begin();
        i != sprites.end();) {
        auto ud = (Entity *) (*i)->_body->GetUserData();
        if(ud->type == UD_DESTROYED || (*i)->getPosition().y > topY) {
            SpriteWithBody::removeFromVector(sprites, i, world);
        }
        else {
            bool isNext = false;
            for(b2ContactEdge *ce = (*i)->_body->GetContactList(); ce; ce = ce->next) {
                b2Body *other = ce->other;
                auto other_userdata = (Entity *) other->GetUserData();
                if(other_userdata && other_userdata->type == UD_NANA) {
                    eat_score += 10;
                    dna++;
                    SpriteWithBody::removeFromVector(sprites, i, world);
                    isNext = true;
                    break;
                }
            }
            if(!isNext)
                ++i;
        }
    }
}