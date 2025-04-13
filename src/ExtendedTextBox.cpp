#include "ExtendedTextBox.h"
#include "iostream"

void ExtendedTextBox::DrawTextBox()
{
    // Vector2 textSize = MeasureTextEx(font, value.c_str(), 20, 1);
    // if (textSize.x + 20 > textBox.width) 
    // {
    //     textBox.width = textSize.x + 20;
    //     confirm.width = textSize.x + 20;
    // }
    // if (textSize.x + 20 < oldWidth)
    // {
    //     textBox.width = oldWidth;
    //     confirm.width = oldWidth;
    // }
    // textBox.DrawButton(0.3, 0.8, LIME, true);
    // confirm.DrawButtonAndText(0.3, 0.8, LIME, true, font, 20, RAYWHITE);
    // DrawTextEx(font, value.c_str(), Vector2{textBox.position.x + 10, textBox.position.y + 8}, 20, 1, PINK);
    {
        float GAP = 0.0 ; 
        std::string line = "" ; 
        float width = 0 , height = 0 ; 
        for(int i=0;i<value.size();i++)
        {
            if(value[i]=='\n')
            {
                Vector2 textSize = MeasureTextEx(font, line.c_str(), 20, 1) ;
                width = std::max(width,textSize.x) ;
                height += textSize.y + GAP ;
                std::cout<<textSize.y<<std::endl;
                line.clear() ; 
            }
            else line+=value[i] ;
        }
        if(line.size()>0)
        {
            Vector2 textSize = MeasureTextEx(font, line.c_str(), 20, 1) ;
            width = std::max(width,textSize.x) ;
            height += textSize.y + GAP ; 
        }
        if(width + 20 > textBox.width) 
        {
            textBox.width = width + 20;
        }
        if(width + 20 < oldWidth)
        {
            textBox.width = oldWidth;
        }
        textBox.height = height  ; 
        if(height<oldHeight)
        {
            textBox.height = oldHeight ; 
        }
        textBox.DrawButton(0.3, 0.8, LIME, true);
        confirm.position = Vector2{textBox.position.x, textBox.position.y+textBox.height+2} ;
        confirm.DrawButtonAndText(0.3, 0.8, LIME, true, font, 20, RAYWHITE);
    }
    {
        std::string line = ""; 
        float withdth = 0 , height = 0 ;
        float GAP  = 0.2 ; 
        for(int i=0;i<value.size();i++)
        {
            if(value[i] == '\n')
            {
                Vector2 textSize = MeasureTextEx(font, line.c_str(), 20, 1) ;
                DrawTextEx(font, line.c_str(), Vector2{textBox.position.x + 10, textBox.position.y +   height}, 20, 1, PINK);
                height += textSize.y + GAP ;
                line.clear() ; 
            }
            else line+=value[i] ; 
        }
        if(line.size()>0)
        {
            DrawTextEx(font, line.c_str(), Vector2{textBox.position.x + 10, textBox.position.y + height}, 20, 1, PINK);
        }
    }
}

std::string ExtendedTextBox::HandleTextBox()
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
        if(IsKeyPressed(KEY_SPACE))
        {
            if(!value.empty())
            {
                if(value.back()==' '||value.back()=='\n')
                {
                    value.pop_back() ; 
                }
                value+=' ';
            }
        }
        if(IsKeyPressed(KEY_ENTER))
        {
            if(count(value.begin(),value.end(),'\n')!=7)
            {
                if(!value.empty())
                {
                    if(value.back()==' '||value.back()=='\n')
                    {
                        value.pop_back() ; 
                    }
                    value+='\n';
                }
            }
        }
    }
    return "";
}
