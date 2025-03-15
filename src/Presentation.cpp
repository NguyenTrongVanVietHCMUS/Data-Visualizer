#include "Presentation.h"

void Presentation::CreateNewPresentation()
{
    present.push_back({});
}

void Presentation::SetCurAnimation(int positionPresentation, float curAnimation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present[positionPresentation].SetCurAnimation(curAnimation);
}

void Presentation::EraseAnimation(int positionPresentation, int positionSet, NewAnimation animation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present[positionPresentation].EraseAnimation(positionSet, animation);
}

void Presentation::InsertAnimationToSet(int positionPresentation, int positionSet, NewAnimation animation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present[positionPresentation].InsertAnimationToSet(positionSet, animation);
}

void Presentation::CopySetToLast(int positionPresentation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present.push_back(present[positionPresentation]);
    present.back().currentSet = 0;
}

bool Presentation::DrawPresentation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontValue, const Font &fontText, const bool &isLightMode, const float &speed)
{

    return false;
}
