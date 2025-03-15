#pragma once
#include "NewAnimation.h"

class SetOfAnimation 
{
    public:
        std::vector<std::vector<NewAnimation>> setOfAnimation;
        int currentSet = 0;

        SetOfAnimation(std::vector<std::vector<NewAnimation>> setOfAnimation = {}, int currentSet = 0): setOfAnimation(setOfAnimation), currentSet(currentSet) {}

        void CreateNewSet();
        void SetType(int positionSet, int type);
        NewAnimation GetAnimation(int positionSet, int i);
        void SetCurAnimation(int positionSet, float curAnimation); 
        void EraseAnimation(int positionSet, NewAnimation animation); 
        void InsertAnimationToSet(int positionSet, NewAnimation animation);
        bool DrawSetOfAnimation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontValue, const Font &fontText, const bool &isLightMode, const float &speed);

};