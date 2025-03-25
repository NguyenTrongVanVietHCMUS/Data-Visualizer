#pragma once
#include "Node.h"
#include <vector>
#include "raylib.h"

class NewAnimation 
{
    public:
        int type;
        Color color;
        // 0: Draw hollow node
        // 1: Draw solid node
        // 2: Draw insert solid animation 
        // 3: Draw delete solid animation 
        // 4: Draw normal edge
        // 5: Draw insert edge animation 
        // 6: Draw delete edge animation 
        // 7: Draw move edge animation
        // 8: Draw move node animation

        float curAnimation, startAnimation = 0;
        std::vector<Node> nodes;
        int weight;
        bool isDirected;
        
        void DrawHollowNode(const Texture2D &hollowCircle, const Font &font, const bool &isLightMode);
        void DrawSolidNode(const Texture2D &solidCircle, const Font &font, const bool &isLightMode);
        bool DrawInsertNodeAnimation(const Texture2D &textureNode, const Font &fontValue, const bool &isLightMode, float &curAnimation, const float &speed);
        bool DrawDeleteNodeAnimation(const Texture2D &textureNode, const Font &fontValue, const bool &isLightMode, float &curAnimation, const float &speed);
        bool DrawMoveNodeAnimation(const Texture2D &textureNode, const Font &fontValue, const bool &isLightMode, float &curAnimation, const float &speed);

        void DrawNormalEdge(const Texture2D &arrowEdge, const bool &isLightMode);
        bool DrawInsertEdgeAnimation(const Texture2D &arrowEdge, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed);
        bool DrawDeleteEdgeAnimation(const Texture2D &arrowEdge, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed);
        bool DrawMoveEdgeAnimation(const Texture2D &arrowEdge, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed);


        NewAnimation(int _type = 0, float _curAnimation = 0, Color _color = BLACK, std::vector<Node> _nodes = {}, int _weight = 0, bool _isDirected = false) : 
            type(_type), curAnimation(_curAnimation), color(_color), nodes(_nodes), weight(_weight), isDirected(_isDirected) {}
        void Init(int _type, const std::vector<Node> &_nodes);
        bool DrawAnimation(const Texture2D &hollowCircle, const Texture2D &solidCircle, const Texture2D &arrowEdge, const Font &fontNumber, const Font &fontText, const bool &isLightMode, const float &speed);
};