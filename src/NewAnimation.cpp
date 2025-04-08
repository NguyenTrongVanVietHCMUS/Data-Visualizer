#include "NewAnimation.h"
#include "math.h"
#include "iostream"

void NewAnimation::DrawNormalEdge(const Texture &arrowEdge, const bool &isLightMode)
{
    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);

    float radius = nodes[0].radius;
    Vector2 startPos = nodes[0].position, endPos = nodes[1].position;
    Vector2 direction = {endPos.x - startPos.x, endPos.y - startPos.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < radius * 2) {
        return;
    }

    Vector2 unitDir = {direction.x / length, direction.y / length};
    Vector2 newStart = {startPos.x + unitDir.x * radius, startPos.y + unitDir.y * radius};
    Vector2 newEnd = {endPos.x - unitDir.x * radius, endPos.y - unitDir.y * radius}; // Adjusted to stop at node boundary
    DrawLineEx(newStart, newEnd, 3.0f, curColor);

    // Calculate angle and arrow position
    float angle = atan2f(direction.y, direction.x) * RAD2DEG;
    // Position the arrow so its tip is at newEnd
    Vector2 arrowPos = {newEnd.x - unitDir.x * 8, newEnd.y - unitDir.y * 8}; // Offset by half texture width (8) to align tip
    DrawTexturePro(arrowEdge, {0, 0, 16, 12}, {arrowPos.x, arrowPos.y, 16, 12}, {8, 6}, angle, curColor);
}

void NewAnimation::DrawHollowNode(const Texture2D &hollowCircle, const Font &fontNumber, const bool &isLightMode)
{
    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);
    nodes[0].DrawHollowNode(hollowCircle, curColor, isLightMode);
    nodes[0].DrawTextNode(fontNumber, curColor, isLightMode);
}

void NewAnimation::DrawSolidNode(const Texture2D &solidCircle, const Font &fontNumber, const bool &isLightMode)
{
    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);
    nodes[0].DrawSolidNode(solidCircle, curColor, isLightMode);
    nodes[0].DrawTextNode(fontNumber, curColor, isLightMode);
}

bool NewAnimation::DrawInsertNodeAnimation(const Texture2D &textureNode, const Font &fontNumber, const bool &isLightMode, float &curAnimation, const float &speed)
{
    if (curAnimation < startAnimation) curAnimation = startAnimation;
    return nodes[0].DrawInsertNodeAnimation(textureNode, fontNumber, color, isLightMode, curAnimation, speed);
}

bool NewAnimation::DrawDeleteNodeAnimation(const Texture2D &textureNode, const Font &fontNumber, const bool &isLightMode, float &curAnimation, const float &speed)
{
    if (curAnimation < startAnimation) curAnimation = startAnimation;
    return nodes[0].DrawDeleteNodeAnimation(textureNode, fontNumber, color, isLightMode, curAnimation, speed);
}

bool NewAnimation::DrawMoveNodeAnimation(const Texture2D &textureNode, const Font &fontNumber, const bool &isLightMode, float &curAnimation, const float &speed)
{
    if (curAnimation < startAnimation) curAnimation = startAnimation;
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1) 
    {
        curAnimation = 1;
        flag = true;
    }

    Vector2 newPosition = Vector2  {   
        nodes[0].position.x + (nodes[1].position.x - nodes[0].position.x) * curAnimation,
        nodes[0].position.y + (nodes[1].position.y - nodes[0].position.y) * curAnimation
    };

    Node newNode = nodes[0]; newNode.position = newPosition;
    newNode.DrawHollowNode(textureNode, color, isLightMode);
    newNode.DrawTextNode(fontNumber, color, isLightMode);

    return flag;
}

bool NewAnimation::DrawInsertEdgeAnimation(const Texture &arrowEdge, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed)
{  
    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);
    if (curAnimation < startAnimation) curAnimation = startAnimation;
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1) 
    {
        curAnimation = 1;
        flag = true;
    }

    float radius = nodes[0].radius;
    Vector2 startPos = nodes[0].position, endPos = nodes[1].position;
    Vector2 direction = {endPos.x - startPos.x, endPos.y - startPos.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < radius * 2) {
        return flag;
    }

    Vector2 unitDir = {direction.x / length, direction.y / length};
    Vector2 newStart = {startPos.x + unitDir.x * radius, startPos.y + unitDir.y * radius};
    Vector2 newEnd = {endPos.x - unitDir.x * (radius + 5), endPos.y - unitDir.y * (radius + 5)};
    Vector2 newCur = {newStart.x + (newEnd.x - newStart.x) * curAnimation, newStart.y + (newEnd.y - newStart.y) * curAnimation};
    DrawLineEx(newStart, newCur, 3.0f, curColor);

    float angle = atan2f(direction.y, direction.x) * RAD2DEG;
    Vector2 arrowPos = {newCur.x - unitDir.x * 4, newCur.y - unitDir.y * 4};
    DrawTexturePro(arrowEdge, {0, 0, 16, 12}, {arrowPos.x, arrowPos.y, 16, 12}, {8, 6}, angle, curColor);

    return flag;
}

bool NewAnimation::DrawDeleteEdgeAnimation(const Texture2D &arrowEdge, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed)
{
    if (curAnimation < startAnimation) curAnimation = startAnimation;
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1) 
    {
        curAnimation = 1;
        flag = true;
    }

    float radius = nodes[0].radius;
    Vector2 startPos = nodes[0].position, endPos = nodes[1].position;
    Vector2 direction = {endPos.x - startPos.x, endPos.y - startPos.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < radius * 2) {
        return flag;
    }

    Vector2 unitDir = {direction.x / length, direction.y / length};
    Vector2 newStart = {startPos.x + unitDir.x * radius, startPos.y + unitDir.y * radius};
    Vector2 newEnd = {float(endPos.x - unitDir.x * (radius + 17.6)), float(endPos.y - unitDir.y * (radius + 17.6))};
    DrawLineEx(newStart, newEnd, 3.0f, Fade(color, 1 - curAnimation));

    newEnd = {endPos.x - unitDir.x * (radius + 5), endPos.y - unitDir.y * (radius + 5)};
    float angle = atan2f(direction.y, direction.x) * RAD2DEG;
    Vector2 arrowPos = {newEnd.x - unitDir.x * 4, newEnd.y - unitDir.y * 4};
    DrawTexturePro(arrowEdge, {0, 0, 16, 12}, {arrowPos.x, arrowPos.y, 16, 12}, {8, 6}, angle, Fade(color, 1 - curAnimation));

    return flag;
}

bool NewAnimation::DrawMoveEdgeAnimation(const Texture2D &arrowEdge, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed)
{
    if (curAnimation < startAnimation) curAnimation = startAnimation;
    bool flag = false;
    curAnimation += speed;
    if (curAnimation >= 1) 
    {
        curAnimation = 1;
        flag = true;
    }
    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);

    float radius = nodes[0].radius;
    Vector2 startPos = Vector2  {   
                                    nodes[0].position.x + (nodes[1].position.x - nodes[0].position.x) * curAnimation,
                                    nodes[0].position.y + (nodes[1].position.y - nodes[0].position.y) * curAnimation
                                };
    Vector2 endPos = Vector2  {   
                                    nodes[2].position.x + (nodes[3].position.x - nodes[2].position.x) * curAnimation,
                                    nodes[2].position.y + (nodes[3].position.y - nodes[2].position.y) * curAnimation
                                };
    Vector2 direction = {endPos.x - startPos.x, endPos.y - startPos.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < radius * 2) {
        return flag;
    }

    Vector2 unitDir = {direction.x / length, direction.y / length};
    Vector2 newStart = {startPos.x + unitDir.x * radius, startPos.y + unitDir.y * radius};
    Vector2 newEnd = {float(endPos.x - unitDir.x * (radius + 5)), float(endPos.y - unitDir.y * (radius + 5))};
    DrawLineEx(newStart, newEnd, 3.0f, curColor);

    float angle = atan2f(direction.y, direction.x) * RAD2DEG;
    Vector2 arrowPos = {newEnd.x - unitDir.x * 4, newEnd.y - unitDir.y * 4};
    DrawTexturePro(arrowEdge, {0, 0, 16, 12}, {arrowPos.x, arrowPos.y, 16, 12}, {8, 6}, angle, curColor);

    return flag;
}

bool NewAnimation::DrawNormalGraphEdge(const Font &fontNumber, const bool &isLightMode, float &curAnimation, const float &speed) {
    if (nodes.size() < 2) return false;

    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);
    
    if (curAnimation < startAnimation) curAnimation = startAnimation;
    bool flag = false;
    curAnimation += speed;

    if (curAnimation >= 1) {
        curAnimation = 1;
        flag = true;
    }

    float radius = nodes[0].radius;
    Vector2 startPos = nodes[0].position, endPos = nodes[1].position;
    Vector2 direction = {endPos.x - startPos.x, endPos.y - startPos.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length <= 2 * radius) return false;

    Vector2 unitDir = {direction.x / length, direction.y / length};
    Vector2 newStart = {startPos.x + unitDir.x * radius, startPos.y + unitDir.y * radius};
    Vector2 newEnd = {endPos.x - unitDir.x * radius, endPos.y - unitDir.y * radius};

    DrawLineEx(newStart, newEnd, 3.0f, curColor);

    // Tính toán vị trí trọng số
    Vector2 weightPos = CalculateWeightPosition(newStart, newEnd);
    if (weight != -1) {
        DrawTextEx(fontNumber, TextFormat("%d", weight), weightPos, 14, 0, curColor);
    }

    return flag;
}

bool NewAnimation::DrawInsertGraphEdgeAnimation(const Color &color, const Font &fontNumber, const bool &isLightMode, float &curAnimation, const float &speed) {  
    if (nodes.size() < 2) return false;

    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);

    if (curAnimation < startAnimation) curAnimation = startAnimation;
    bool flag = false;
    curAnimation += speed;

    if (curAnimation >= 1) {
        curAnimation = 1;
        flag = true;
    }

    float radius = nodes[0].radius;
    Vector2 startPos = nodes[0].position, endPos = nodes[1].position;
    Vector2 direction = {endPos.x - startPos.x, endPos.y - startPos.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < radius * 2) {
        return flag;
    }

    Vector2 unitDir = {direction.x / length, direction.y / length};
    Vector2 newStart = {startPos.x + unitDir.x * radius, startPos.y + unitDir.y * radius};
    Vector2 newEnd = {endPos.x - unitDir.x * radius, endPos.y - unitDir.y * radius};

    // Vẽ cạnh với hiệu ứng xuất hiện dần
    Vector2 newCur = {
        newStart.x + (newEnd.x - newStart.x) * curAnimation,
        newStart.y + (newEnd.y - newStart.y) * curAnimation
    };
    DrawLineEx(newStart, newCur, 3.0f, Fade(curColor, curAnimation));

    // Hiển thị trọng số khi cạnh gần hoàn thành (>= 80%)
    if (curAnimation > 0.8f) {
        Vector2 weightPos = CalculateWeightPosition(newStart, newEnd);
        if (weight != -1) {
            DrawTextEx(fontNumber, TextFormat("%d", weight), weightPos, 14, 0, Fade(color, curAnimation));
        }
    }

    return flag;
}

bool NewAnimation::DrawInsertGraphDirectedEdgeAnimation(const Texture2D &arrowEdge, const Color &color, const Font &fontNumber, const bool &isLightMode, float &curAnimation, const float &speed)
{
    if (nodes.size() < 2) return false;

    Color curColor = (ColorIsEqual(color, BLACK) ? (isLightMode ? BLACK : WHITE) : color);

    if (curAnimation < startAnimation) curAnimation = startAnimation;
    bool flag = false;
    curAnimation += speed;

    if (curAnimation >= 1) {
        curAnimation = 1;
        flag = true;
    }

    float radius = nodes[0].radius;
    Vector2 startPos = nodes[0].position, endPos = nodes[1].position;
    Vector2 direction = {endPos.x - startPos.x, endPos.y - startPos.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < radius * 2) {
        return flag;
    }

    Vector2 unitDir = {direction.x / length, direction.y / length};
    Vector2 newStart = {startPos.x + unitDir.x * radius, startPos.y + unitDir.y * radius};
    Vector2 newEnd = {endPos.x - unitDir.x * radius, endPos.y - unitDir.y * radius};

    // Vẽ cạnh với hiệu ứng xuất hiện dần
    Vector2 newCur = {
        newStart.x + (newEnd.x - newStart.x) * curAnimation,
        newStart.y + (newEnd.y - newStart.y) * curAnimation
    };
    DrawLineEx(newStart, newCur, 3.0f, Fade(curColor, curAnimation));

    float angle = atan2f(direction.y, direction.x) * RAD2DEG;
    Vector2 arrowPos = {newCur.x - unitDir.x * 4, newCur.y - unitDir.y * 4};
    DrawTexturePro(arrowEdge, {0, 0, 16, 12}, {arrowPos.x, arrowPos.y, 16, 12}, {8, 6}, angle, curColor);

    // Hiển thị trọng số khi cạnh gần hoàn thành (>= 80%)
    if (curAnimation > 0.8f) {
        Vector2 weightPos = CalculateWeightPosition(newStart, newEnd);
        if (weight != -1) {
            DrawTextEx(fontNumber, TextFormat("%d", weight), weightPos, 14, 0, Fade(color, curAnimation));
        }
    }

    return flag;
}

// Hàm tính vị trí trọng số để cả 2 hàm vẽ cùng một vị trí
Vector2 NewAnimation::CalculateWeightPosition(const Vector2 &newStart, const Vector2 &newEnd) {
    Vector2 midPoint = {(newStart.x + newEnd.x) / 2, (newStart.y + newEnd.y) / 2};
    float angle = atan2(abs(newEnd.y - newStart.y), abs(newEnd.x - newStart.x));
    float offset = 15;
    return {midPoint.x + offset * sin(angle), midPoint.y - offset * cos(angle)};
}


void NewAnimation::Init(int _type, const std::vector<Node> &_nodes)
{
    type = _type;
    nodes = _nodes;
}

bool NewAnimation::DrawAnimation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontNumber, const Font &fontText, const bool &isLightMode, const float &speed)
{
    if (type == 0)
    {
        DrawHollowNode(hollowCircle, fontNumber, isLightMode);
        return true;
    }
    if (type == 1)
    {
        DrawSolidNode(solidCircle, fontNumber, isLightMode);
        return true;
    }
    if (type == 2)
    {
        return DrawInsertNodeAnimation(solidCircle, fontNumber, isLightMode, curAnimation, speed);
    }
    if (type == 3)
    {
        return DrawDeleteNodeAnimation(solidCircle, fontNumber, isLightMode, curAnimation, speed);
    }
    if (type == 4)
    {
        DrawNormalEdge(arrowEdge, isLightMode);
        return true;
    }   
    if (type == 5)
    {
        return DrawInsertEdgeAnimation(arrowEdge, color, isLightMode, curAnimation, speed);
    }
    if (type == 6)
    {
        return DrawDeleteEdgeAnimation(arrowEdge, color, isLightMode, curAnimation, speed);
    }
    if (type == 7)
    {
        return DrawMoveEdgeAnimation(arrowEdge, color, isLightMode, curAnimation, speed);
    }
    if (type == 8)
    {
        return DrawMoveNodeAnimation(hollowCircle, fontNumber, isLightMode, curAnimation, speed);
    }
    if (type == 9)
    {
        return DrawNormalGraphEdge(fontNumber, isLightMode, curAnimation, speed);
    }
    if (type == 10)
    {
        return DrawInsertGraphEdgeAnimation(color, fontNumber, isLightMode, curAnimation, speed);
    }
    if (type == 11)
    {
        return DrawInsertGraphDirectedEdgeAnimation(arrowEdge, color, fontNumber, isLightMode, curAnimation, speed);
    }
    return false;
}