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
        TextBox enterList;
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
        void Run();
        void Init();
        void Clear();
        void ResetNodesPosition(TrieNode* root ) ; 
        
        

        void DrawToolBar();
        void HandleToolBar();
        
        
        
        
        

        void Draw();
        
};