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
        TextBox enterList;
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
        int height(AVLNode *N) ;  
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
        

        void DrawToolBar();
        void HandleToolBar();
        
        
        
        
        

        void Draw();
        
};
