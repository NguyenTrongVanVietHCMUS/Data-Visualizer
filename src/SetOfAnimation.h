#pragma once
#include "NewAnimation.h"

class SetOfAnimtion 
{
    public:
        std::vector<std::vector<NewAnimation>> setOfAnimtion;
        int currentSet;

        bool DrawSetOfAnimation(const bool &isLightMode, const float &speed);
        SetOfAnimtion(std::vector<std::vector<NewAnimation>> setOfAnimtion = {}, int currentSet = 0): setOfAnimtion(setOfAnimtion), currentSet(currentSet) {}

};