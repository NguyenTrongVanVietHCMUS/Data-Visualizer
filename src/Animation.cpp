#include "Animation.h"
#include "iostream"

bool Animation::drawAnimationNode(bool isLightMode, float speed)
{
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1)
    {
        curAnimation = 1;
        flag = true;
    }
    Node tmpNode = firstNodeStart;
    tmpNode.position = (Vector2){float(tmpNode.position.x + (firstNodeEnd.position.x - tmpNode.position.x) * curAnimation), 
                                 float(tmpNode.position.y + (firstNodeEnd.position.y - tmpNode.position.y) * curAnimation)};
    if (flag == true) 
    {
        // tmpNode.DrawChosenNode();
    }
    else 
    {
        DrawTextureEx(solidCircle, (Vector2){float(tmpNode.position.x - 24.0f), float(tmpNode.position.y - 24.0f)}, 0, 1, Fade(ORANGE, curAnimation <= 0.5 ? 1 : curAnimation >= 1 ? 0 : 1 - curAnimation));
        char text[5];
        snprintf(text, sizeof(text), "%d", tmpNode.value);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawTextEx(GetFontDefault(), text, (Vector2){tmpNode.position.x - textSize.x / 2, tmpNode.position.y - textSize.y / 2}, 20, 1, isLightMode ? BLACK : WHITE);
    }
    return flag;
}

void Animation::drawNormalEdge(bool isLightMode)
{
    DrawLineEx((Vector2){firstNodeEnd.position.x + 24.0f, firstNodeEnd.position.y}, 
                (Vector2){secondNodeEnd.position.x - 43.0f, secondNodeEnd.position.y}, 4.0f, isLightMode ? BLACK : WHITE);
    DrawTextureEx(arrowEdge, (Vector2){secondNodeEnd.position.x - 43.0f, float(secondNodeEnd.position.y - 15.0 / 2)}, 0, 1, isLightMode ? BLACK : WHITE);
}

bool Animation::DrawAnimationEdge(bool isLightMode, float speed)
{
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1)
    {
        curAnimation = 1;
        flag = true;
    }
    
    Node tmpFirstNode = firstNodeStart;
    tmpFirstNode.position = (Vector2){float(tmpFirstNode.position.x + (firstNodeEnd.position.x - tmpFirstNode.position.x) * curAnimation), 
                                    float(tmpFirstNode.position.y + (firstNodeEnd.position.y - tmpFirstNode.position.y) * curAnimation)};
    Node tmpSecondNode = secondNodeStart;
    tmpSecondNode.position = (Vector2){float(tmpSecondNode.position.x + (secondNodeEnd.position.x - tmpSecondNode.position.x) * curAnimation), 
                                        float(tmpSecondNode.position.y + (secondNodeEnd.position.y - tmpSecondNode.position.y) * curAnimation)};
    
    tmpFirstNode.position.x += 24.0f;
    tmpSecondNode.position.x -= 43.0f;

    tmpSecondNode.position.x = tmpFirstNode.position.x + curAnimation * (tmpSecondNode.position.x - tmpFirstNode.position.x);
    DrawLineEx(tmpFirstNode.position, tmpSecondNode.position, 4.0f, ORANGE);
    DrawTextureEx(arrowEdge, (Vector2){tmpSecondNode.position.x, float(tmpSecondNode.position.y - 15.0 / 2)}, 0, 1, ORANGE);
    return flag;
}

void Animation::Init(bool _type, bool _useAnimation, float _curAnimation, Node _firstNodeStart, Node _firstNodeEnd, Node _secondNodeStart, Node _secondNodeEnd)
{
    type = _type;
    useAnimation = _useAnimation;
    curAnimation = _curAnimation;
    firstNodeStart = _firstNodeStart;
    firstNodeEnd = _firstNodeEnd;
    secondNodeStart = _secondNodeStart;
    secondNodeEnd = _secondNodeEnd;
}

bool Animation::DrawAnimation(bool isLightMode, float speed)
{
    if (type == false)
    {
        if (useAnimation == false)
        {
            // firstNodeEnd.DrawNormalNode(isLightMode);
            return true;
        }
        else
        {
            return drawAnimationNode(isLightMode, speed);
        }
    }
    else 
    {
        if (useAnimation == false)
        {   
            drawNormalEdge(isLightMode);
            return true;
        }
        else 
        {
            return DrawAnimationEdge(isLightMode, speed);
        }
    }
    return false;
}