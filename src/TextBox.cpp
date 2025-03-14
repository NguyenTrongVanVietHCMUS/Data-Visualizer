#include "TextBox.h"
#include "iostream"

void TextBox::DrawTextBox()
{
    Vector2 textSize = MeasureTextEx(font, value.c_str(), 20, 1);
    if (textSize.x + 20 > textBox.width) 
    {
        textBox.width = textSize.x + 20;
        confirm.width = textSize.x + 20;
    }
    if (textSize.x + 20 < oldWidth)
    {
        textBox.width = oldWidth;
        confirm.width = oldWidth;
    }
    textBox.DrawButton(0.3, 0.8, LIME, true);
    confirm.DrawButtonAndText(0.3, 0.8, LIME, true, font, 20, RAYWHITE);
    DrawTextEx(font, value.c_str(), Vector2{textBox.position.x + 10, textBox.position.y + 8}, 20, 1, PINK);


}

std::string TextBox::HandleTextBox()
{
    if (confirm.CheckMouseClickInRectangle() == true) 
    {
        textBox.width = oldWidth;
        confirm.width = oldWidth;
        std::string tmp = value;
        isSelected = false;
        value.clear();
        return tmp;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
    {
        isSelected = textBox.CheckMouseInRectangle() == true;
    }
    if (isSelected == true) 
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125))
            {
                value.push_back(key);
            }
            key = GetCharPressed(); 
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (value.empty() == false)
            {
                value.pop_back();
            }
        }
    }
    return "";
}
