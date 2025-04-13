#pragma once
#include "DataStructure.h"
#include "AVLNode.h" 
#include <string.h>  
#include <vector>
#include <set> 
#include <stdlib.h>
class AVL : public DataStructure 
{
    public:
        Font fontRoboto = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        ExtendedTextBox enterList;
        TextBox insertI;
        TextBox insertV;
        float xOFFSET = 75;
        float yOFFSET = 80; 
        AVLNode *root;
        std::vector<std::string> str; 
        void calcPosition(AVLNode *root); 
        void BuildNodeFromValue(std::vector<std::string>list);
        void ClearAllData();
        void RandomNewData();
        void InputDataFromFile();
        void BuildCreateAnimation();
        std::vector<AVLNode*> AVL::getNodes(AVLNode *root) ; 
        int HEIGHT(AVLNode *N) ;  
        int getBalance(AVLNode *N) ; 


        AVLNode* leftRotate(AVLNode *x) ;
        AVLNode* rightRotate(AVLNode *y) ;
        // AVLNode* Add(AVLNode* node, std::string c) ;
        void Add(AVLNode*& node, std::string c) ;         
        void Insert(std::string s);
        void Search(std::string s );
        void Delete(std::string s );

        SetOfAnimation BasicStructure(AVLNode* root);
        Presentation InsertAnimation( std::string s );
        Presentation DeleteAnimation(std::string s ,Color color);
        Presentation SearchAnimation(std::string s ,  Color color);
        Presentation CreateAnimation(AVLNode* root);

        std::vector<std::string> StringToVector(std::string listChar);
        void Run();
        void Init();
        void Clear();
        void ResetNodesPosition(AVLNode* root ) ; 
        

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
