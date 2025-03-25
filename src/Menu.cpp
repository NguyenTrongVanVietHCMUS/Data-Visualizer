#include "Menu.h"
#include <iostream>


void Menu::Init() 
{   
    buttons = new Button[8];
    buttons[0] = {Vector2{250, 250}, 300, 180, (char *)"One"};
    buttons[1] = {Vector2{650, 250}, 300, 180, (char *)"Two"};
    buttons[2] = {Vector2{1050, 250}, 300, 180, (char *)"Three"};
    buttons[3] = {Vector2{250, 500}, 300, 180, (char *)"Four"};
    buttons[4] = {Vector2{650, 500}, 300, 180, (char *)"Five"};   
    buttons[5] = {Vector2{1050, 500}, 300, 180, (char *)"Six"};
    buttons[6] = {Vector2{1532, 12}, 48, 48, (char *)"Seven"};
    buttons[7] = {Vector2{20, 12}, 48, 48, (char *)"Eight"};

    dataStructure = 0;

    isLightMode = 1;

    logo = LoadTexture("../Textures/Logo.png");
    homeIcon = LoadTexture("../Textures/HomeIcon.png");
    toDarkMode = LoadTexture("../Textures/ToDarkMode.png");
    toLightMode = LoadTexture("../Textures/ToLightMode.png");

    myList.Init();
    myGraph.Init();
}

void Menu::Clear() 
{
    delete[] buttons;
}

void Menu::DrawMenu() 
{
    DrawGradient(isLightMode);
    DrawTexture(logo, 574, 50, WHITE);
    for (int i = 0; i < 6; i++) {
        DrawRectangleRounded(Rectangle{buttons[i].position.x, buttons[i].position.y, buttons[i].width, buttons[i].height}, 0.3, 10, buttons[i].CheckMouseInRectangle() ? DARKBLUE : SKYBLUE);
        DrawText(buttons[i].text, buttons[i].position.x + 50, buttons[i].position.y + 75, 28, DARKGRAY);
    }
}



void Menu::HandleMenu() {
    if (buttons[7].CheckMouseClickInRectangle()) {
        isLightMode = !isLightMode;
        myList.isLightMode = isLightMode;
    }
    if (dataStructure == 0) 
    {
        for (int i = 0; i < 6; i++) 
        {
            if (buttons[i].CheckMouseClickInRectangle())
            {
                dataStructure = i + 1;
                break;
            }
        }
    } 
    else 
    {
        if (buttons[6].CheckMouseClickInRectangle()) {
            dataStructure = 0;
        }
    }
}

void Menu::Run() {
    ClearBackground(RAYWHITE);
    HandleMenu();
    if (dataStructure == 0) {
        DrawMenu();
    } else {
        DrawGradient(isLightMode);
        if (dataStructure == 1) {
            myList.Run();
        }
        if (dataStructure == 2) {
            
        }
        if (dataStructure == 3) {
            
        }
        if (dataStructure == 4) {
        
        }
        if (dataStructure == 5) {
            myGraph.Run();
        }
        if (dataStructure == 6) {   

        }
    }
    Texture2D modeIcon = isLightMode ? toDarkMode : toLightMode;
    DrawTexture(modeIcon, buttons[7].position.x, buttons[7].position.y, WHITE); 
    DrawTexture(homeIcon, buttons[6].position.x, buttons[6].position.y, buttons[6].CheckMouseInRectangle() ? DARKBLUE : SKYBLUE); 
}

void Menu::DrawGradient(bool isLightMode)
{
    for (int y = 0; y < 800; y++)
    {
        float t = (float)y / 800;
        Color gradientColor;

        if (isLightMode)
        {
            gradientColor = {
                (unsigned char)(180 + 50 * t), 
                (unsigned char)(220 + 35 * t), 
                (unsigned char)(250),          
                255
            };
        }
        else
        {
            gradientColor = {
                (unsigned char)(10 + 30 * t), 
                (unsigned char)(30 + 50 * t),  
                (unsigned char)(80 + 60 * t), 
                255
            };
        }

        DrawLine(0, y, 1600, y, gradientColor);
    }
}

Color Menu::GetColorBackground() {
    return isLightMode ? Color{130, 160, 230, 255} : Color{0, 5, 10, 255};
}