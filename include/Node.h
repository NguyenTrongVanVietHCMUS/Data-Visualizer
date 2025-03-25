#pragma once
#include <string>
#include "raylib.h"
#include "raymath.h"

class Node
{
    public:
        Vector2 position;
        float radius;
        // int value ;  
        std::string value ;  
        Vector2 offset;


        Node(): position(Vector2{0, 0}), radius(0), value(0) {}
        // Node(Vector2 pos, float r, int val) : position(pos), radius(r), value(value) {}
        Node(Vector2 pos, float r, std::string value ) : position(pos), radius(r), value(value) {}
        void DrawTextNode(const Font &fontValue, const Color &color, const bool &isLightMode);
        void DrawSolidNode(const Texture2D &textureNode, const Color &color, const bool &isLightMode);
        void DrawHollowNode(const Texture2D &textureNode, const Color &color, const bool &isLightMode);
        bool DrawInsertNodeAnimation(const Texture2D &textureNode, const Font &fontValue, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed);
        bool DrawDeleteNodeAnimation(const Texture2D &textureNode, const Font &fontValue, const Color &color, const bool &isLightMode, float &curAnimation, const float &speed);
        bool operator== (const Node &other) const 
        { 
            return position == other.position && value == other.value; 
        }
    private:
};  