#pragma once
#include "DataStructure.h"

class SinglyLinkedList : public DataStructure 
{
    public:
        std::vector<int> StringToVector(std::string listChar);
        Font fontRoboto = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        TextBox enterList;
        TextBox insertI;
        TextBox insertV;
        std::vector<std::string>values ; 
        int FindPosition(std::string value);        
        void ClearAllData();
        void RandomNewData();
        void InputDataFromFile();
        void BuildCreateAnimation();
        void Insert(int pos, std::string val);
        void Delete(std::string pos);
        void Search(std::string val);
        void Update(int pos, std::string val);

        std::vector<Node> BuildNodeFromValue(const std::vector<std::string> &values);
        SetOfAnimation BasicStructure(const std::vector<Node> &nodes);
        Presentation InsertAnimation(int pos, std::string val);
        Presentation UpdateAnimation(int pos, std::string val);
        Presentation DeleteAnimation(int pos);
        Presentation SearchAnimation(int val, Color color);
        Presentation CreateAnimation(const std::vector<Node> &nodes);

        int CountNumberOfAnimation();

        void Run();
        void Init();
        void Clear();

        void Draw();
        void DrawRemote();
        void HandleRemote();
        void DrawToolBar();
        void HandleToolBar();
    
        


        std::vector<std::string> createList;
        std::vector<std::string> searchList;
        std::vector<std::string> updateList;
        std::vector<std::string> insertList;
        std::vector<std::string> deleteList;
        std::vector<std::string> notFound;
        Vector2 positionBoard;
        float width, height;
};