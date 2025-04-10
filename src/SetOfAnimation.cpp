#include "SetOfAnimation.h"
#include <iostream>
#include <vector>
#include <algorithm>

bool SetOfAnimation::DrawSetOfAnimation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontNumber, const Font &fontText, const bool &isLightMode, const float &speed, const std::vector<std::string> &list, Vector2 position, float width, float height, int &numberOfPresentation, int curRemoteState)
{
    for (std::vector<NewAnimation> &curSet: setOfAnimation)
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
        auto checkIsNotDone = [&]()
        {
            for (NewAnimation &curAnimation : curSet)
            {
                if (curAnimation.type > 1 && curAnimation.type != 4 && curAnimation.startAnimation == 0)
                {
                    if (curAnimation.curAnimation < 1.0f) return true;
                }
            }
            return false;
        };
        if (curRemoteState == 1) 
        {
            if (checkIsAnimation() && checkIsNotDone()) 
            {
                return false;
            }
        }
        bool doneSet = true, isNewSet = false;
        for (NewAnimation &animation: curSet)
        {
            if (animation.type > 1 && animation.type != 4 && animation.startAnimation == 0 && animation.curAnimation < 1.0f)
            {
                isNewSet = true;
            }
            doneSet &= animation.DrawAnimation(hollowCircle, solidCircle, arrowEdge, fontNumber, fontText, isLightMode, speed, curRemoteState);
            DrawRectangle(position.x, position.y, width, height, Color{ 197, 75, 99, 255 });
            // Thêm padding lề trái để chữ không bị sát mép
            float textPaddingX = 15; // Đẩy lề trái
            float textPaddingY = 10; // Khoảng cách từ trên xuống
            float lineSpacing = 25;  // Khoảng cách giữa các dòng
            float lineHeight = 22;   // Chiều cao mỗi dòng
            Vector2 textPos = { position.x + textPaddingX, position.y + textPaddingY };
            std::vector<int> &highlightLines = animation.listHighlights;
            for (size_t i = 0; i < list.size(); i++)
            {
                bool isHighlighted = std::find(highlightLines.begin(), highlightLines.end(), static_cast<int>(i)) != highlightLines.end();

                if (isHighlighted)
                {
                    DrawRectangle(position.x, textPos.y - 2, width, lineHeight, BLACK);
                    DrawTextEx(fontText, list[i].c_str(), textPos, 20, 0, Color{ 173, 216, 230, 255 });
                }
                else
                {
                    DrawTextEx(fontText, list[i].c_str(), textPos, 20, 0, RAYWHITE);
                }
                textPos.y += lineHeight;
            }
        }
        if (doneSet == false)
        {
            return false;
        }
        if (isNewSet)
        {
            ++numberOfPresentation;
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