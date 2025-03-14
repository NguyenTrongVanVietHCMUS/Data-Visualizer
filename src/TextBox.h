#pragma once
#include <string>
#include "Button.h"

class TextBox 
{
    public:
        Font font = LoadFont("Fonts/Roboto_Condensed-SemiBold.ttf");
        float oldWidth;
        Button textBox;
        Button confirm;
        bool isSelected;
        std::string value;
        char *text;
        
        void DrawTextBox();
        std::string HandleTextBox();
};