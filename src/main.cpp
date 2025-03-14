#include <raylib.h>
#include "Menu.h"
#include <iostream>

const int screenWidth = 1600;
const int screenHeight = 800;

int main() 
{
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    InitWindow(screenWidth, screenHeight, "Data Structure Visualization"); 
    SetTargetFPS(60);
    
    Menu application = Menu();
    application.Init();

    while (WindowShouldClose() == false) {
        BeginDrawing();
        application.Run();
        EndDrawing();
    }
    application.Clear();

    CloseWindow();
    return 0;
}   