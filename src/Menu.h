#pragma once
#include "Node.h"
#include "NewAnimation.h"
#include "Button.h"
#include "raylib.h"
#include "SinglyLinkedList.h"

class Menu
{
    private:
        Texture2D logo; 
        Texture2D homeIcon;
        Texture2D toDarkMode;
        Texture2D toLightMode; 

        Button *buttons;
        Font currentFont;
        
        SinglyLinkedList myList;
        

        Color GetColorBackground();
        void DrawGradient(const bool isLightMode);
    public:
        int dataStructure;
        int isLightMode;
        
        void Run();
        void Init();
        void Clear();
        void DrawMenu();
        void HandleMenu();
};



