#pragma once
#include "DataStructure.h"

class Heap : public DataStructure 
{
    public:
        const int capacity = 1 << 10;
        int size = 0;
        int *itemID;

        std::vector<int> StringToVector(std::string listChar);
        Font fontRoboto = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        ExtendedTextBox enterList;
        TextBox insertI;
        TextBox insertV;
        std::vector<std::string>values ; 
        int FindPosition(std::string value);        
        void ClearAllData();
        void RandomNewData();
        void InputDataFromFile();
        void BuildCreateAnimation();
        void Push(std::string val);
        void Pop();
        void Top();
        void Search(std::string val);
        void CalcPosition(std::vector<Node> &nodes, int curID = 0, int height = 0, float x = 800, float y = 50);

        std::vector<Node> BuildNodeFromValue(const std::vector<std::string> &values);
        
        void HeapifyDown(Presentation &myPresentation, std::vector<int> highlightLines);
        void HeapifyUp(Presentation &myPresentation, std::vector<int> highlightLines);



        SetOfAnimation BasicStructure(const std::vector<Node> &nodes);
        Presentation UpdateAnimation(int pos, std::string val);
        Presentation DeleteAnimation(int pos );
        Presentation SearchAnimation(int val, Color color);
        Presentation CreateAnimation(const std::vector<Node> &nodes);

        void Run();
        void Init();
        void Clear();
        
        
        

        void Draw();
        void DrawRemote();
        void HandleRemote();
        void DrawToolBar();
        void HandleToolBar();
    
        


        std::vector<std::string> createList;
        std::vector<std::string> topList;
        std::vector<std::string> pushList;
        std::vector<std::string> popList;
        Vector2 positionBoard;
        float width, height;
        
};