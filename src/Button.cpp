#include "Button.h"

bool Button::CheckMouseInRectangle() 
{
    Vector2 mousePostion = GetMousePosition();
    return (position.x <= mousePostion.x && mousePostion.x <= position.x + width && position.y <= mousePostion.y && mousePostion.y <= position.y + height);
}

bool Button::CheckMouseClickInRectangle()
{
    return CheckMouseInRectangle() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Button::DrawButtonTexture()
{
    Color curColor = SKYBLUE;
    if (CheckMouseInRectangle()) 
    curColor = Fade(curColor, 0.2);
    DrawTexture(texture, position.x, position.y, curColor);
}

void Button::DrawText(Font font, int fontSize, Color textColor)
{
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 1);
    Vector2 textPosition = 
    {
        position.x + width / 2 - textSize.x / 2,
        position.y + height / 2 - textSize.y / 2,
    };
    DrawTextEx(font, text, textPosition, fontSize, 1, textColor);
}

void Button::DrawButton(float rounded, float fade, Color color, bool useFade)
{
    DrawRectangleRounded(Rectangle{position.x, position.y, width, height}, rounded, 0, Fade(color, !useFade || CheckMouseInRectangle() ? 1.0f : 0.8f));
}

void Button::DrawButtonAndText(float rounded, float fade, Color color, bool useFade, Font font, int fontSize, Color textColor)
{
    DrawButton(rounded, fade, color, useFade);
    DrawText(font, fontSize, textColor);
}
