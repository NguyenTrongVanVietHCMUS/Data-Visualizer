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
        Texture2D remoteTextures[7] = 
        {
            LoadTexture("../Textures/undo.png"),
            LoadTexture("../Textures/pre.png"),
            LoadTexture("../Textures/next.png"),
            LoadTexture("../Textures/redo.png"),
            LoadTexture("../Textures/pause.png"), 
            LoadTexture("../Textures/continue.png"),
            LoadTexture("../Textures/repeat.png")
        };
        std::vector<Button> remoteButtons;
        std::vector<bool> flagRemote;
        int curRemoteState = 0; 
        // 0 -> continue
        // 1 -> pause
        // 2 -> repeat
        
        
        std::vector<std::vector<bool>> flagToolBarButtons;
        std::vector<std::vector<Button>> toolBarButtons;
        

        std::vector<Node> nodes;
        int isLightMode;

        float speed;
        Button changeSpeed;
        Presentation myPresentation;
        

        void DrawArrow(Vector2 position);
        void DrawToolBar();
        void HandleToolBar();
};
