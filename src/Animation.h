#pragma once
#include "Node.h"

class Animation 
{
    private:
        Texture2D solidCircle = LoadTexture("Textures/SolidCircle.png");
        Texture2D arrowEdge = LoadTexture("Textures/ArrowEdge.png");
        bool type; // 0: Node, 1: Edge
        
        float curAnimation;
        Node firstNodeStart, firstNodeEnd;
        Node secondNodeStart, secondNodeEnd;
        
        bool drawAnimationNode(bool isLightMode, float speed);
        void drawNormalEdge(bool isLightMode);
        bool DrawAnimationEdge(bool isLightMode, float speed);
    public:
        bool useAnimation; 
        void Init(bool _type, bool _useAnimation, float _curAnimation, Node _firstNodeStart, Node _firstNodeEnd, Node _secondNodeStart, Node _secondNodeEnd);
        bool DrawAnimation(bool isLightMode, float speed);
};
