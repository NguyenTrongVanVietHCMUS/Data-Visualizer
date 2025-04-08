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
        present[positionPresentation].SetStartAnimation(i, startAnimation);
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

void Presentation::EraseAnimation(int positionPresentation, NewAnimation animation)
{
    if (positionPresentation == -1) 
    {
        positionPresentation = int(present.size()) - 1;
        if (positionPresentation < 0)
        {
            return;
        }
    }
    for (int j = 0; j < int(present[positionPresentation].setOfAnimation.size()); ++j)
    {
        present[positionPresentation].EraseAnimation(j, animation);
    }
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
    if (!present.empty())
    {
        currentPresentation = std::min(currentPresentation, int(present.size()) - 1);
        currentPresentation += present[currentPresentation].DrawSetOfAnimation(hollowCircle, solidCircle, arrowEdge, fontValue, fontText, isLightMode, speed, list, position, width, height, numberOfPresentation) == true;

    }
}



int Presentation::CountNumberOfAnimation()
{
    int res = 0;
    for (SetOfAnimation &curSet: present)
    {
        for (std::vector<NewAnimation> &curGroup: curSet.setOfAnimation) 
        {
            bool flag = false;
            for (NewAnimation &curAnimation: curGroup) 
            {
                if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation != 1)
                {
                    flag = true;
                }
            }
            res += flag;
        }
    }
    return res;
}

void Presentation::InitBoardText(const std::vector<std::string> &_list, Vector2 _postition, float _width, float _height)
{
    list = _list;
    position = _postition;
    width = _width;
    height = _height;
}

void Presentation::SetAllhighlightLines(int startPresentation, int endPresentation, std::vector<int> listHighlights)
{
    if (startPresentation == -1) 
    {
        startPresentation = int(present.size()) - 1;
        if (startPresentation < 0)
        {
            return;
        }
    }
    if (endPresentation == -1) 
    {
        endPresentation = int(present.size()) - 1;
        if (endPresentation < 0)
        {
            return;
        }
    }
    for (int i = startPresentation; i <= endPresentation; ++i)  
    {
        for (std::vector<NewAnimation> &curGroup: present[i].setOfAnimation)
        {
            for (NewAnimation &curAnimation: curGroup)
            {
                curAnimation.listHighlights = listHighlights;
            }
        }
    }
}
