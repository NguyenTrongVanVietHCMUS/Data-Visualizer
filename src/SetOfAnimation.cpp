#include "SetOfAnimation.h"
#include "iostream"

bool SetOfAnimation::DrawSetOfAnimation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontNumber, const Font &fontText, const bool &isLightMode, const float &speed)
{
    for (std::vector<NewAnimation> &curSet: setOfAnimation)
    {
        bool doneSet = true;
        for (NewAnimation &animation: curSet)
        {
            doneSet &= animation.DrawAnimation(hollowCircle, solidCircle, arrowEdge, fontNumber, fontText, isLightMode, speed);
        }
        if (doneSet == false)
        {
            return false;
        }
    }
    return true;
}

void SetOfAnimation::EraseAnimation(int positionSet, NewAnimation animation)
{
    if (positionSet == -1)
    {
        positionSet = int(setOfAnimation.size()) - 1;
        if (positionSet < 0) 
        {
            return;
        }
    }
    for (int j = int(setOfAnimation[positionSet].size()) - 1; j >= 0; --j)
    {
        if (setOfAnimation[positionSet][j].nodes == animation.nodes && ColorIsEqual(setOfAnimation[positionSet][j].color, animation.color) && setOfAnimation[positionSet][j].type == animation.type)
        {
            setOfAnimation[positionSet].erase(setOfAnimation[positionSet].begin() + j);
        }
    }
}

void SetOfAnimation::CreateNewSet()
{
    setOfAnimation.push_back({});
}

void SetOfAnimation::SetType(int positionSet, int type)
{
    if (positionSet == -1)
    {
        positionSet = int(setOfAnimation.size()) - 1;
        if (positionSet < 0) 
        {
            return;
        }
    }
    for (NewAnimation &animation: setOfAnimation[positionSet])
    {
        animation.type = type;
    }
}

void SetOfAnimation::SetStartAnimation(int positionSet, float c)
{
    if (positionSet == -1)
    {
        positionSet = int(setOfAnimation.size()) - 1;
        if (positionSet < 0) 
        {
            return;
        }
    }
    for (NewAnimation &animation: setOfAnimation[positionSet])
    {
        animation.startAnimation = c;
    }
}

NewAnimation SetOfAnimation::GetAnimation(int positionSet, int i)
{
    if (positionSet == -1)
    {
        positionSet = int(setOfAnimation.size()) - 1;
        if (positionSet < 0) 
        {
            return NewAnimation();
        }
    }
    if (i == -1) 
    {
        i = int(setOfAnimation[positionSet].size()) - 1;
        if (i < 0)
        {
            return NewAnimation();
        }
    }
    return setOfAnimation[positionSet][i];
}

void SetOfAnimation::SetCurAnimation(int positionSet, float curAnimation)
{
    if (positionSet == -1)
    {
        positionSet = int(setOfAnimation.size()) - 1;
        if (positionSet < 0) 
        {
            return;
        }
    }
    for (NewAnimation &animation: setOfAnimation[positionSet])
    {
        animation.curAnimation = curAnimation;
    }
}

void SetOfAnimation::InsertAnimationToSet(int positionSet, NewAnimation animation)
{
    if (positionSet == -1)
    {
        positionSet = int(setOfAnimation.size()) - 1;
        if (positionSet < 0) 
        {
            return;
        }
    }
    if (positionSet < int(setOfAnimation.size()))
    {
        setOfAnimation[positionSet].push_back(animation);
    }
}