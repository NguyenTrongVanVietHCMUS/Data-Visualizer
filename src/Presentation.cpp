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

void Presentation::DrawPresentation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontValue, const Font &fontText, const bool &isLightMode, const float &speed, int curRemoteState)
{
    if (!present.empty())
    {
        currentPresentation = std::min(currentPresentation, int(present.size()) - 1);
        currentPresentation += present[currentPresentation].DrawSetOfAnimation(hollowCircle, solidCircle, arrowEdge, fontValue, fontText, isLightMode, speed, list, position, width, height, numberOfPresentation, curRemoteState) == true;
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

void Presentation::SetCurToStartAnimation(int startPresentation, int endPresentation)
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
                curAnimation.curAnimation = curAnimation.startAnimation;
            }
        }
    }
}

void Presentation::SetCurAnimation(int startPresentation, int endPresentation, int curr)
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
                curAnimation.curAnimation = curr;
            }
        }
    }
}

void Presentation::UnFillCurPresentation()
{
    if (int(present.size()) != 0)
    {
        auto checkIsInProcessing = [&](int currentPresentation, int positionSet)
        {   
            bool flagAnimation = false, isInProcessing = true;
            for (NewAnimation &curAnimation: present[currentPresentation].setOfAnimation[positionSet])
            {
                if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                {
                    flagAnimation = true;
                    isInProcessing &= curAnimation.curAnimation != 0.0f && curAnimation.curAnimation != 1.0f;
                }
            }
            return (!flagAnimation ? false : isInProcessing);
        };
        auto checkIsAnimationDone = [&](int currentPresentation, int positionSet)
        {   
            bool isDone = true, isAnimation = false;
            for (NewAnimation &curAnimation: present[currentPresentation].setOfAnimation[positionSet])
            {
                if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                {
                    isDone &= curAnimation.curAnimation == 1.0f;
                    isAnimation = true;
                }
            }
            return isDone && isAnimation;
        };
        auto unFill = [&](int currentPresentation, int positionSet)
        {
            for (NewAnimation &curAnimation: present[currentPresentation].setOfAnimation[positionSet])
            {
                curAnimation.curAnimation = 0;
            }
            return;
        };
        for (int k = int(present.size()) - 1; k >= 0; --k)
        {
            currentPresentation = k;
            for (int i = int(present[k].setOfAnimation.size()) - 1; i >= 0; --i) 
            {
                if (checkIsInProcessing(k, i) == true || checkIsAnimationDone(k, i) == true)
                {
                    numberOfPresentation -= checkIsAnimationDone(k, i) == true;
                    
                    unFill(k, i);
                    if (i == 0)
                    {
                        if (currentPresentation > 0)
                            --currentPresentation;
                    }
                    return;
                }
            }
        }
    }
}

void Presentation::FillCurPresentation()
{
    if (int(present.size()) != 0)
    {
        auto checkIsInProcessing = [&](int currentPresentation, int positionSet)
        {   
            bool flagAnimation = false, isInProcessing = true;
            for (NewAnimation &curAnimation: present[currentPresentation].setOfAnimation[positionSet])
            {
                if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                {
                    flagAnimation = true;
                    isInProcessing &= curAnimation.curAnimation != 0.0f && curAnimation.curAnimation != 1.0f;
                }
            }
            return (!flagAnimation ? false : isInProcessing);
        };
        auto checkIsAnimation = [&](int currentPresentation, int positionSet)
        {   
            for (NewAnimation &curAnimation: present[currentPresentation].setOfAnimation[positionSet])
            {
                if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                {
                    return true;
                }
            }
            return false;
        };
        auto checkIsAnimationDone = [&](int currentPresentation, int positionSet)
        {   
            bool isDone = true, isAnimation = false;
            for (NewAnimation &curAnimation: present[currentPresentation].setOfAnimation[positionSet])
            {
                if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                {
                    isDone &= curAnimation.curAnimation == 1.0f;
                    isAnimation = true;
                }
            }
            return isDone && isAnimation;
        };
        auto fill = [&](int currentPresentation, int positionSet)
        {
            for (NewAnimation &curAnimation: present[currentPresentation].setOfAnimation[positionSet])
            {
                curAnimation.curAnimation = 1;
            }
            return;
        };
        for (int k = 0; k < int(present.size()); ++k)
        {
            currentPresentation = k;
            for (int i = 0; i < int(present[k].setOfAnimation.size()); ++i) 
            {
                if (checkIsInProcessing(k, i) == true || (!checkIsAnimationDone(k, i) && checkIsAnimation(k, i)))
                {
                    ++numberOfPresentation;
                    if (numberOfPresentation > CountNumberOfAnimation())
                    {
                        numberOfPresentation = CountNumberOfAnimation();
                    }
                    fill(k, i);
                    return;
                }
            }
        }
    }
}

void Presentation::FindPosition(int frame, int &currentPosition, int &positionSet)
{
    int frameCount = 0;

    if (frame == 0) 
    {
        currentPosition = 0;
        positionSet = -1;
        return;
    }

    auto checkIsAnimation = [&](int i, int j)
    {
        for (NewAnimation &curAnimation : present[i].setOfAnimation[j])
        {
            if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
            {
                return true;
            }
        }
        return false;
    };
    int lastI, lastJ;
    for (int i = 0; i < present.size(); ++i)
    {
        for (int j = 0; j < present[i].setOfAnimation.size(); ++j)
        {
            if (checkIsAnimation(i, j))
            {
                lastI = i;
                lastJ = j;
                if (frame == ++frameCount)
                {
                    currentPosition = i;
                    positionSet = j;
                    return;
                }
            }
        }
    }

    // Nếu frame vượt quá tổng số animation thì trả về vị trí cuối cùng
    currentPosition = lastI;
    positionSet = lastJ;
}

void Presentation::SetCurToStartAnimationInOnePresentation(int i, int j)
{
    if (i < int(present.size()))
    {
        if (j < 0)
        {
            j = 0;
        }
        for (int k = j; k < int(present[i].setOfAnimation.size()); ++k)
        {
            for (NewAnimation &curAnimation: present[i].setOfAnimation[k])
            {
                curAnimation.curAnimation = curAnimation.startAnimation;
            }
        }
    }
}

void Presentation::NormPresentation(int curRemoteState)
{
    if (currentPresentation > 0 && curRemoteState == 1)
    {
        bool flag = false;
        for (std::vector<NewAnimation> &curSet: present[currentPresentation].setOfAnimation)
        {
            auto checkIsAnimation = [&]()
            {
                for (NewAnimation &curAnimation : curSet)
                {
                    if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                    {
                        return true;
                    }
                }
                return false;
            };
            auto checkIsStart = [&]()
            {
                for (NewAnimation &curAnimation : curSet)
                {
                    if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                    {
                        if (curAnimation.curAnimation == 0.0f) return true;
                    }
                }
                return false;
            };
            if (checkIsAnimation() == true)
            {
                if (checkIsStart() == true)
                {
                    if (flag == false)
                    {
                        --currentPresentation;
                    }
                    return;
                }
                flag = true;
            }
        }
    }
}