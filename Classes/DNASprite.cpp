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
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("eat_sound.mp3");
                    eat_score += ud->type;
                    dna++;
                    SpriteWithBody::removeFromVector(sprites, i, world);
                    isNext = true;
                    makeToast((*i), StringUtils::format("+%d", ud->type));
                    break;
                }
            }
            if(!isNext)
                ++i;
        }
    }
}

#include "GameLayer.h"
void DNASprite::makeToast(SpriteWithBody *tracker, std::string txt) {
    cocos2d::Label *toastLabel = Label::createWithTTF(txt, "fonts/Marker Felt.ttf", 30);
    toastLabel->setPosition(winMidX, winMidY*1.5);
    toastLabel->setColor(Color3B::WHITE);
    
    ((GameLayer *)(tracker->getParent()->getParent()))->_infoLayer->addChild(toastLabel);
    
    toastLabel->runAction(Sequence::
                          create(ScaleBy::create(0.1, 1.5, 1.5),
                                 ScaleBy::create(0.1, 0.66, 0.66),
                                 CallFunc::create
                                 (CC_CALLBACK_0(Sprite::removeFromParent,
                                                toastLabel)),
                                 NULL));
}
