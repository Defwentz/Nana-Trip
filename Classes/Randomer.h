//
//  Randomer.h
//  nanatrip
//
//  Created by Macbook Air on 7/26/15.
//
//

#ifndef __nanatrip__Randomer__
#define __nanatrip__Randomer__

#include <stdio.h>
#include <vector>

class Randomer
{
private:
    std::vector<int> _item;
    std::vector<int> _odds;
    
    int odds_mask;
public:
    Randomer();
    void add(int item, int odds);
    void updateOdds(int item, int newOdds);
    int getRandomItem();
};

#endif /* defined(__nanatrip__Randomer__) */
