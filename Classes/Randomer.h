//
//  Randomer.h
//  nanatrip
//
//  Created by Macbook Air on 7/26/15.
//
//

#ifndef __nanatrip__Randomer__
#define __nanatrip__Randomer__

#include <cstdlib>
#include <vector>

class Randomer
{
private:
    /**
     * item-code
     */
    std::vector<int> _item;
    
    /**
     * odds of the correspond item appears
     */
    std::vector<int> _odds;
    
    /**
     * sum odds of the Randomer
     */
    int odds_mask;
public:
    Randomer();
    
    /**
     *  @brief	Add new item with item-code and its odds of appearence.
     *
     *  @param 	item 	item-code
     *  @param 	odds 	odds of appearence
     */
    void add(int item, int odds);
    
    /**
     *  @brief	Update the odds of certain item.
     *
     *  @param 	item 	item-code
     *  @param 	odds 	new odds of appearence
     */
    void updateOdds(int item, int newOdds);
    
    /**
     *  @brief	get random item
     *
     *  @return item 	item-code
     */
    int getRandomItem();
};

#endif /* defined(__nanatrip__Randomer__) */
