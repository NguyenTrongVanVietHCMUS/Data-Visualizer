#pragma once
#include "DataStructure.h"
#include <string.h>  
#include <vector>
class TrieNode
{
    public:
        bool running = 0 ; 
        int sl = 0 ; 
        std::string c ; 
        std::vector<TrieNode*>children ; 
        Vector2 CurrentPosition ;
        Vector2 TargetedPosition ;
        TrieNode(Vector2 CurrentPosition, std::string c ):CurrentPosition(CurrentPosition),c(c)
        {
            TargetedPosition = CurrentPosition ; 
        }

        void setTargetedPosition(Vector2 TargetedPosition)
        {
            this->TargetedPosition = TargetedPosition ; 
        }
};
class Trie : public DataStructure 
{
    public:
        Font fontRoboto = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        ExtendedTextBox enterList;
        TextBox insertI;
        TextBox insertV;
        float xOFFSET = 75;
        float yOFFSET = 80; 
        TrieNode *root;
        std::vector<std::string> str; 
        Vector2 calcPosition(TrieNode *root);
        int FindPosition(int value);        
        void BuildNodeFromValue(std::vector<std::string>list);
        void ClearAllData();
        void RandomNewData();
        void InputDataFromFile();
        void BuildCreateAnimation();
        void Add(std::string s);
        void Insert(std::string s);
        void Search(std::string s );
        void Delete(std::string s );

        SetOfAnimation BasicStructure(TrieNode* root);
        Presentation InsertAnimation( std::string s );
        Presentation DeleteAnimation(std::string s ,Color color);
        Presentation SearchAnimation(std::string s ,  Color color);
        Presentation CreateAnimation(TrieNode* root);

        std::vector<std::string> StringToVector(std::string listChar);
        void ResetNodesPosition(TrieNode* root ) ; 
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