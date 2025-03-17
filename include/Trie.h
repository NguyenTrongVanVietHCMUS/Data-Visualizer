#pragma once
#include "DataStructure.h"
#include <string.h>  
#include <vector>.h
class TrieNode
{
    public:
        char c ; 
        vector<TrieNode*>children ; 
        TrieNode()
        {
            isEndOfWord = false ; 
        }
}
class Trie : public DataStructure 
{
    public:
        Font fontRoboto = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        TextBox enterList;
        TextBox insertI;
        TextBox insertV;
        
        TrieNode *root;

        std::vector<std::string> str;
        int FindPosition(int value);        
        void ClearAllData();
        void RandomNewData();
        void InputDataFromFile();
        void BuildCreateAnimation();
        void Insert(int pos, int val);
        void Delete(int val);
        void Search(int val);
        void Update(int pos, int val);

        std::vector<Node> BuildNodeFromTrie();
        SetOfAnimation BasicStructure(const std::vector<Node> &nodes);
        Presentation InsertAnimation(int pos, int val);
        Presentation UpdateAnimation(int pos, int val);
        Presentation DeleteAnimation(int val);
        Presentation SearchAnimation(int val, Color color);
        Presentation CreateAnimation(const std::vector<Node> &nodes);

        void Run();
        void Init();
        void Clear();
        
        
        

        void DrawToolBar();
        void HandleToolBar();
        
        
        
        
        

        void Draw();
        
};
