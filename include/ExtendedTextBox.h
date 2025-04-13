#pragma once
#include <string>
#include "Button.h"

class ExtendedTextBox 
{
    public:
        Font font = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        float oldWidth;
        float oldHeight;
        Button textBox;
        Button confirm;
        bool isSelected;
        std::string value;
        char *text;
        
        void DrawTextBox();
        std::string HandleTextBox();
};