#pragma once
#include<set> 
#include<string.h> 
class AVLNode
{
    public:
        int balance , heightLeft,  heightRight, balanceLeft,balanceRight,height; 
        Vector2 CurrentPosition ,  TargetedPosition ;
        std::string c ; 
        bool running ; 
        AVLNode *left , *right; 
        AVLNode(Vector2 CurrentPosition, std::string c, int height) ; 
        void setTargetedPosition(Vector2 pos); 
        Vector2 getTargetedPosition() ; 
        std::set<std::pair<std::string,std::string>> EdgeSet() ;
        // AVLNode* rightRotate(std::string c ); 
        // AVLNode* leftRotate(std::string c ); 
        AVLNode* minNode() ; 
};