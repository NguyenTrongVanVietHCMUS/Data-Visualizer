#pragma once
#include "DataStructure.h"

std::vector<int> StringToVector(std::string listChar);

class SinglyLinkedList : public DataStructure 
{
    public:
        Font fontRoboto = LoadFont("Fonts/Roboto_Condensed-SemiBold.ttf");
        TextBox enterList;
        TextBox insertI;
        TextBox insertV;

        int FindPosition(int value);        
        void ClearAllData();
        void RandomNewData();
        void InputDataFromFile();
        void BuildCreateAnimation();
        void Insert(int pos, int val);
        void Delete(int val);
        void Search(int val);
        void Update(int pos, int val);

        std::vector<Node> BuildNodeFromValue(const std::vector<int> &values);
        std::vector<std::vector<NewAnimation>> BasicStructure(const std::vector<Node> &nodes);
        std::vector<std::vector<std::vector<NewAnimation>>> InsertAnimation(int pos, int val);
        std::vector<std::vector<std::vector<NewAnimation>>> UpdateAnimation(int pos, int val);
        std::vector<std::vector<std::vector<NewAnimation>>> DeleteAnimation(int val);
        std::vector<std::vector<std::vector<NewAnimation>>> SearchAnimation(int val, Color color);
        std::vector<std::vector<std::vector<NewAnimation>>> CreateAnimation(const std::vector<Node> &nodes);

        void Run();
        void Init();
        void Clear();
        
        
        

        void DrawToolBar();
        void HandleToolBar();
        
        
        
        
        

        void Draw();
        
};