#include "Presentation.h"
#include "iostream"

void Presentation::Clear()
{
    present.clear();
    currentPresentation = 0;
}

void Presentation::CreateNewPresentation()
{
    present.push_back({});
}

void Presentation::CreateNewSet(int positionPresentation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present[positionPresentation].CreateNewSet();
}

void Presentation::SetType(int positionPresentation, int positionSet, int type)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present[positionPresentation].SetType(positionSet, type);
}

void Presentation::SetStartAnimation(int positionPresentation, float startAnimation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    for (int i = 0; i < int(present[positionPresentation].setOfAnimation.size()); ++i)
    {
        present[positionPresentation].SetCurAnimation(i, startAnimation);
    }
}

void Presentation::SetStartAnimation(int positionPresentation, int positionSet, float startAnimation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present[positionPresentation].SetStartAnimation(positionSet, startAnimation);
}

void Presentation::SetType(int positionPresentation, int type)
{
    for (int i = 0; i < int(present[positionPresentation].setOfAnimation.size()); ++i)
    {
        SetType(positionPresentation, type);
    }
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
    for (int i = 0; i < int(present[positionPresentation].setOfAnimation.size()); ++i)
    {
        present[positionPresentation].SetCurAnimation(i, curAnimation);
    }
}

void Presentation::SetCurAnimation(int positionPresentation, int positionSet, float curAnimation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    present[positionPresentation].SetCurAnimation(positionSet, curAnimation);
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

void Presentation::Swap(int positionPresentation, int i, int j)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    std::swap(present[positionPresentation].setOfAnimation[i], present[positionPresentation].setOfAnimation[j]);
}

void Presentation::Merge(int positionPresentation, int i, int j)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    for (NewAnimation &animation: present[positionPresentation].setOfAnimation[j])
    {
        InsertAnimationToSet(positionPresentation, i, animation);
    }
    present[positionPresentation].setOfAnimation[j].clear();
}

NewAnimation Presentation::GetAnimation(int positionPresentation, int positionSet, int i)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return NewAnimation();
        }
    }
    return present[positionPresentation].GetAnimation(positionSet, i);
}

void Presentation::DrawPresentation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontValue, const Font &fontText, const bool &isLightMode, const float &speed)
{
    if (present.empty() == false)
    {
        currentPresentation = std::min(currentPresentation, int(present.size()) - 1);
        currentPresentation += present[currentPresentation].DrawSetOfAnimation(hollowCircle, solidCircle, arrowEdge, fontValue, fontText, isLightMode, speed) == true;

    }
}
