#include "SetOfAnimation.h"

bool SetOfAnimation::DrawSetOfAnimation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontNumber, const Font &fontText, const bool &isLightMode, const float &speed)
{
    for (int i = 0; i <= std::min(currentSet, int(setOfAnimation.size()) - 1); ++i)
    {
        bool doneSet = true;
        for (NewAnimation &animation: setOfAnimation[i])
        {
            if (animation.DrawAnimation(hollowCircle, solidCircle, arrowEdge, fontNumber, fontText, isLightMode, speed) == false)
            {
                doneSet = false;
            }
        }       
        if (doneSet == false) {
            return false;
        }
    }
    currentSet = std::min(currentSet + 1, int(setOfAnimation.size()) - 1);
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
    if (setOfAnimation[positionSet].empty() == true)
    {
        setOfAnimation.erase(setOfAnimation.begin() + positionSet);
    }
}

void SetOfAnimation::CreateNewSet()
{
    setOfAnimation.push_back({});
}

void SetOfAnimation::SetCurAnimation(float curAnimation)
{
    for (std::vector<NewAnimation> &curSet: setOfAnimation)
    {
        for (NewAnimation &animation: curSet)
        {
            animation.curAnimation = curAnimation;
        }
    }
}

void SetOfAnimation::InsertAnimationToSet(int positionSet, NewAnimation animation)
{
    if (positionSet == -1)
    {
        positionSet = int(setOfAnimation.size()) - 1;
    }
    if (positionSet < int(setOfAnimation.size()))
    {
        setOfAnimation[positionSet].push_back(animation);
    }
}