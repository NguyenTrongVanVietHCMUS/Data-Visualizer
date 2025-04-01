#include "AVL.h" 

AVLNode::AVLNode(Vector2 CurrentPosition,std::string c , int height):c(c),CurrentPosition(CurrentPosition),height(height)
{ 
    left = right  =nullptr ;
    running = 0 ; 
    TargetedPosition = CurrentPosition ; 
    balance = heightLeft = heightRight = balanceLeft = balanceRight= 0 ; 
} 
void AVLNode::setTargetedPosition(Vector2 pos)
{
    TargetedPosition = pos ; 
}
Vector2 AVLNode::getTargetedPosition()
{
    return TargetedPosition ; 
}
std::set<std::pair<std::string,std::string>> AVLNode:: EdgeSet()
{
    std::set<std::pair<std::string,std::string>>res ; 
    auto go = [&](AVLNode* node,  auto&& go_ref)->void
    {
        if(!node)return ; 
        if(node->left)
        {
            res.insert({node->c,node->left->c}) ; 
            go_ref(node->left,go_ref) ; 
        }
        if(node->right)
        {
            res.insert({node->c,node->right->c}) ; 
            go_ref(node->right,go_ref) ; 
        }
    }; 
    go(this,go) ;
    return res ; 
}
AVLNode* AVLNode::minNode()
{
    AVLNode* current = this;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}   