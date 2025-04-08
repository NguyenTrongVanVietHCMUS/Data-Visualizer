#pragma once
#include "raylib.h"

struct Button
{
    Vector2 position;
    float width;
    float height;
    char *text;
    Texture2D texture;

    bool CheckMouseInRectangle();
    bool CheckMouseClickInRectangle();

    void DrawButtonTexture();
    void DrawText(Font font, int fontSize, Color textColor);
    void DrawButton(float rounded, float fade, Color color, bool useFade);
    void DrawButtonAndText(float rounded, float fade, Color color, bool useFade, Font font, int fontSize, Color textColor);
};