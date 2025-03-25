#pragma once
#include "Node.h"
#include <vector>
#include "raylib.h"
#include "Button.h"
#include "TextBox.h"
#include "NewAnimation.h"
#include "Presentation.h"

class DataStructure
{
    public:
        Texture2D toolBarRightArrow = LoadTexture("../Textures/ToolBarRightArrow.png");
        Texture2D toolBarLeftArrow = LoadTexture("../Textures/ToolBarLeftArrow.png");
        Texture2D trash = LoadTexture("../Textures/Trash.png");
        Texture2D shuffle = LoadTexture("../Textures/Shuffle.png");
        Texture2D dropFile = LoadTexture("../Textures/DropFile.png");
        Texture2D uploadFile = LoadTexture("../Textures/UploadFile.png");
        Texture2D hollowCircle = LoadTexture("../Textures/HollowCircle.png");
        Texture2D solidCircle = LoadTexture("../Textures/SolidCircle.png");
        Texture2D arrowEdge = LoadTexture("../Textures/ArrowEdge.png");
        Font robotoFont = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        Font fontNumber = LoadFont("../Fonts/InterBold.ttf");
        
        std::vector<std::vector<bool>> flagToolBarButtons;
        std::vector<std::vector<Button>> toolBarButtons;
        // std::vector<Button> removeButtons;

        std::vector<Node> nodes;
        std::vector<int> values;
        int isLightMode;

        float speed;
        Presentation myPresentation;
        

        void DrawArrow(Vector2 position);
        void DrawToolBar();
        void HandleToolBar();
};
