#include "Node.h"
#include <iostream>

void Node::DrawTextNode(const Font &fontValue, const Color &color, const bool &isLightMode)
{
    char text[5];
    snprintf(text, sizeof(text), "%d", value);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
    Color curColor = (ColorIsEqual(BLACK, color) ? (isLightMode ? BLACK : WHITE) : color);
    DrawTextEx(fontValue, text, Vector2{position.x - textSize.x / 2, position.y - textSize.y / 2}, 20, 1, curColor);
}

void Node::DrawHollowNode(const Texture2D &textureNode, const Color &color, const bool &isLightMode)
{
    Color curColor = (ColorIsEqual(BLACK, color) ? (isLightMode ? BLACK : WHITE) : color);
    std::cerr << position.x << ' ' << position.y << ' ' << radius << '\n';
    DrawTextureEx(textureNode, Vector2{position.x - radius, position.y - radius}, 0, 1, curColor);
}

void Node::DrawSolidNode(const Texture2D &textureNode, const Color &color, const bool &isLightMode)
{
    Color curColor = (ColorIsEqual(BLACK, color) ? (isLightMode ? BLACK : WHITE) : color);
    DrawTextureEx(textureNode, Vector2{position.x - radius, position.y - radius}, 0, 1, curColor);
}

bool Node::DrawInsertNodeAnimation(const Texture2D &textureNode, const Font &fontValue, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed)
{
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1) {
        flag = true;
        curAnimation = 1;
    }
    DrawSolidNode(textureNode, Fade(color, curAnimation), isLightMode);
    DrawTextNode(fontValue, Fade((isLightMode ? BLACK : WHITE), curAnimation), isLightMode);
    return flag;
}

bool Node::DrawDeleteNodeAnimation(const Texture2D &textureNode, const Font &fontValue, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed)
{
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1) {
        flag = true;
        curAnimation = 1;
    }
    DrawSolidNode(textureNode, Fade(color, 1 - curAnimation), isLightMode);
    DrawTextNode(fontValue, Fade((isLightMode ? BLACK : WHITE), 1 - curAnimation), isLightMode);
    return flag;
}
