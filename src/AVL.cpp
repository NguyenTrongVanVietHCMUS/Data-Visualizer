#include "AVL.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

void AVL::Run() 
{
    HandleToolBar();
    DrawToolBar();
    Draw();
}

void AVL::Init()
{
    toolBarButtons.resize(6);
    toolBarButtons[0] = 
    {
        {Vector2{0, 570}, 15, 183, (char *)"Open_Close"}
    };
    toolBarButtons[1] = 
    {
        {Vector2{17, 570}, 160, 35, (char *)"Create"},
        {Vector2{179, 570}, 80, 35, (char *)"Empty"},
        {Vector2{261, 570}, 80, 35, (char *)"Random"},
        {Vector2{343, 570}, 80, 35, (char *)"File"},
        {Vector2{425, 570}, 80, 35, (char *)"Enter"},
    };
    toolBarButtons[2] = 
    {
        {Vector2{17, 607}, 160, 35, (char *)"Insert"}
    };
    toolBarButtons[3] = 
    {
        {Vector2{17, 644}, 160, 35, (char *)"Delete"}
    };
    toolBarButtons[4] = 
    {
        {Vector2{17, 681}, 160, 35, (char *)"Update"}
    };
    toolBarButtons[5] = 
    {
        {Vector2{17, 718}, 160, 35, (char *)"Search"}
    };
    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }
    flagToolBarButtons[0][0] = true ; 

    // enterList.Init();
    enterList.oldWidth = 80;
    enterList.textBox = {Vector2{425 + 80 + 2, 570}, 80, 35};
    enterList.confirm = {Vector2{425 + 80 + 2, 570 + 35 + 2}, 80, 35, (char *)"Confirm"};

    nodes.clear();
    root = nullptr ; 
    isLightMode = 1;

    speed = 0.05;
}
std::vector<AVLNode*> AVL::getNodes(AVLNode *root) {
    std::vector<AVLNode*> nodes;
    if (root == NULL) return nodes;
    // if (root->valid == false) return nodes;
    if (root->left != NULL) root->left->setTargetedPosition({0, root->getTargetedPosition().y + yOFFSET});
    if (root->right != NULL) root->right->setTargetedPosition({0, root->getTargetedPosition().y + yOFFSET});
    std::vector<AVLNode*> left = getNodes(root->left), right = getNodes(root->right);
    nodes.insert(nodes.end(), left.begin(), left.end());
    nodes.push_back(root);
    nodes.insert(nodes.end(), right.begin(), right.end());
    root->height = 1 + std::max(root->left == NULL ? 0 : root->left->height, root->right == NULL ? 0 : root->right->height);
    root->balance = (root->left == NULL ? 0 : root->left->height) - (root->right == NULL ? 0 : root->right->height);
    root->heightLeft = root->left == NULL ? 0 : root->left->height;
    root->heightRight = root->right == NULL ? 0 : root->right->height;
    root->balanceLeft = root->left == NULL ? 0 : root->left->balance;
    root->balanceRight = root->right == NULL ? 0 : root->right->balance;
    return nodes;
}

void AVL::calcPosition(AVLNode*root) {
    if (root == NULL) return;
    root->setTargetedPosition({0, 100});
    std::vector<AVLNode*> nodes = getNodes(root);
    if (nodes.size() == 0) return;
    int mid = nodes.size() / 2;
    nodes[mid]->setTargetedPosition({700, nodes[mid]->getTargetedPosition().y});
    for (int i = mid - 1; i >= 0; i--) 
        nodes[i]->setTargetedPosition({nodes[i + 1]->getTargetedPosition().x - xOFFSET / 2, nodes[i]->getTargetedPosition().y});
    for (int i = mid + 1; i < nodes.size(); i++)
        nodes[i]->setTargetedPosition({nodes[i - 1]->getTargetedPosition().x + xOFFSET / 2, nodes[i]->getTargetedPosition().y});
}
Presentation AVL::CreateAnimation(AVLNode* root)
{
    calcPosition(root) ;    
    Presentation create;
    create.CreateNewPresentation() ; 
    create.CreateNewSet(-1) ;
    {// Add all nodes
        auto AddNode = [&](AVLNode* node, auto&& AddNode_ref) -> void 
        {
            if (!node) return;
            // draw the node first 
            create.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {Node(node->TargetedPosition,20,node->c)}));
            if(node->left)AddNode_ref(node->left, AddNode_ref);  
            if(node->right)AddNode_ref(node->right, AddNode_ref);
        };
        AddNode(root, AddNode);
    }
    { // Insert all edges
        create.CopySetToLast(-1);
        create.SetStartAnimation(-1, -1, 1);
        create.CreateNewSet(-1);
        auto AddEdge = [&](AVLNode* node, auto&& AddEdge_ref) -> void 
        {
            if (!node) return;
            if(node->left)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {Node(node->TargetedPosition,20,node->c),Node(node->left->TargetedPosition,20,node->left->c)}));
                AddEdge_ref(node->left, AddEdge_ref);
            }
            if(node->right)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {Node(node->TargetedPosition,20,node->c),Node(node->right->TargetedPosition,20,node->right->c)}));
                AddEdge_ref(node->right, AddEdge_ref);
            }
        };
        AddEdge(root, AddEdge); 
    }
    { // Remove animations
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        auto AddNode = [&](AVLNode* node, auto&& AddNode_ref) -> void 
        {
            if (!node) return;
            create.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {Node(node->TargetedPosition,20,node->c)}));
            if(node->left)
            {
                AddNode_ref(node->left, AddNode_ref);
            }
            if(node->right)
            {
                AddNode_ref(node->right, AddNode_ref);
            }
        };
        AddNode(root, AddNode);
        auto AddEdge = [&](AVLNode* node, auto&& AddEdge_ref) -> void 
        {
            if (!node) return;
            if(node->left)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {Node(node->TargetedPosition,20,node->c),Node(node->left->TargetedPosition,20,node->left->c)}));
                AddEdge_ref(node->left, AddEdge_ref);
            }
            if(node->right)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {Node(node->TargetedPosition,20,node->c),Node(node->right->TargetedPosition,20,node->right->c)}));
                AddEdge_ref(node->right, AddEdge_ref);
            }
        };
        AddEdge(root, AddEdge);
        auto DelNode = [&](AVLNode* node, auto&& DelNode_ref) -> void 
        {
            if (!node) return;
            // draw the node first 
            create.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {Node(node->TargetedPosition,20,node->c)}));
            if(node->left)
            {
                DelNode_ref(node->left, DelNode_ref);
            }
            if(node->right)
            {
                DelNode_ref(node->right, DelNode_ref);
            }
        };
        DelNode(root, DelNode);
        auto DelEdge = [&](AVLNode* node, auto&& DelEdge_ref) -> void 
        {
            if (!node) return;
            if(node->left)
            {
                
                create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {Node(node->TargetedPosition,20,node->c),Node(node->left->TargetedPosition,20,node->left->c)}));
                DelEdge_ref(node->left, DelEdge_ref);
            }
            if(node->right)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {Node(node->TargetedPosition,20,node->c),Node(node->right->TargetedPosition,20,node->right->c)}));
                DelEdge_ref(node->right,DelEdge_ref) ; 
            }
        };
        DelEdge(root, DelEdge);
    }
    return create;
}

void AVL::BuildCreateAnimation()
{
    myPresentation.currentPresentation = 0;
    myPresentation = CreateAnimation(root);
}

void AVL::ClearAllData()
{
    root = nullptr ;
    str.clear() ; 
    myPresentation.Clear();
}

AVLNode* AVL::leftRotate(AVLNode *x) { 
    AVLNode *y = x->right; 
    AVLNode *T2 = y->left; 

    // Perform rotation 
    y->left = x; 
    x->right = T2;  

    // Update heights 
    x->height = 1 + std::max(height(x->left), 
                        height(x->right)); 
    y->height = 1 + std::max(height(y->left), 
                        height(y->right)); 

    // Return new root 
    return y; 
} 
AVLNode* AVL::rightRotate(AVLNode *y) { 
    AVLNode *x = y->left; 
    AVLNode *T2 = x->right; 

    // Perform rotation 
    x->right = y; 
    y->left = T2; 

    // Update heights 
    y->height = 1 + std::max(height(y->left), 
                    height(y->right)); 
    x->height = 1 + std::max(height(x->left), 
                        height(x->right)); 

    // Return new root 
    return x; 
} 
int AVL::height(AVLNode *N) { 
    if (N == nullptr) 
        return 0; 
    return N->height; 
} 
int AVL::getBalance(AVLNode *N) { 
    if (N == nullptr) 
        return 0; 
    return height(N->left) - height(N->right); 
} 
void AVL::Add(AVLNode*& node, std::string c )
{
    if (!node) { node = new AVLNode({0,0},c,1); return; }
    if (stoi(c) < stoi(node->c))
        Add(node->left, c);
    else if (stoi(c) > stoi(node->c))
        Add(node->right, c);
    else
        return;

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && stoi(c) < stoi(node->left->c)) {
        node = rightRotate(node);
        return;
    }
    if (balance < -1 && stoi(c) > stoi(node->right->c)) {
        node = leftRotate(node);
        return;
    }
    if (balance > 1 && stoi(c) > stoi(node->left->c)) {
        node->left = leftRotate(node->left);
        node = rightRotate(node);
        return;
    }
    if (balance < -1 && stoi(c) < stoi(node->right->c)) {
        node->right = rightRotate(node->right);
        node = leftRotate(node);
        return;
    }
}
void AVL::BuildNodeFromValue(std::vector<std::string>list)
{
    str = list ;  
    root = nullptr ; 
    for(auto s:str)
    {
        Add(root,s) ; 
    }
    calcPosition(root) ; 
}
void AVL::RandomNewData()
{
    ClearAllData();

    // int n  = 1;  
    int n = GetRandomValue(1, 20);
    std::vector<std::string> str ;
    std::set<std::string> value;  
    for (int i = 1; i <= n; ++i)
    {
        value.insert(std::to_string(GetRandomValue(1,50))) ; 
    }
    for(auto x:value)
    {
        str.push_back(x) ; 
    }
    BuildNodeFromValue(str) ; 
    BuildCreateAnimation() ;
}
void AVL::InputDataFromFile()
{
    ClearAllData();

    FilePathList droppedFiles = LoadDroppedFiles();
    char *filePath = new char[2048];

    TextCopy(filePath, droppedFiles.paths[0]);
    UnloadDroppedFiles(droppedFiles);

    std::ifstream fin(filePath);
    std::string s;
    getline(fin, s);

    std::string curValue = "" ;
    for (char &c: s)
    {
        bool flag = 0 ;  
        if (('0' <= c && c <= '9'))
        {
            curValue+=c ; 
            flag = 1 ; 
        }
        if(!flag)
        {
            str.push_back(curValue);
            curValue = "" ;  
        }
    }
    if (curValue != "") ; 
    {
        str.push_back(curValue);
    }
    fin.close();
    BuildCreateAnimation();
}

SetOfAnimation AVL::BasicStructure(AVLNode* root )
{
    SetOfAnimation basicStructure;
    { 
        basicStructure.CreateNewSet() ;
        auto AddNode = [&](AVLNode* node, auto&& AddNode_ref) -> void 
        {
            if (!node) return;
            // draw the node first 
            basicStructure.InsertAnimationToSet(-1,NewAnimation(0, 0, BLACK, {Node(node->TargetedPosition,20,node->c)}));
            if(node->left)
            {
                AddNode_ref(node->left, AddNode_ref);
            }
            if(node->right)
            {
                AddNode_ref(node->right, AddNode_ref);
            }
        };
        AddNode(root, AddNode);
        auto AddEdge = [&](AVLNode* node, auto&& AddEdge_ref) -> void 
        {
            if (!node) return;
            if(node->left)
            {
                basicStructure.InsertAnimationToSet(-1, NewAnimation(4, 0, BLACK, {Node(node->TargetedPosition,20,node->c),Node(node->left->CurrentPosition,20,node->left->c)}));
                AddEdge_ref(node->left, AddEdge_ref);
            }
            if(node->right)
            {
                basicStructure.InsertAnimationToSet(-1, NewAnimation(4, 0, BLACK, {Node(node->TargetedPosition,20,node->c),Node(node->right->CurrentPosition,20,node->right->c)}));
                AddEdge_ref(node->right, AddEdge_ref);
            }
        };
        AddEdge(root, AddEdge);
    }
    return basicStructure;
}

Presentation AVL::SearchAnimation( std::string s , Color color)
{
    Presentation search;
    AVLNode* cur = root ; 
    search.present = {BasicStructure(root)};
    auto FindVal = [&](AVLNode* node , std::string s, auto&& FindVal_ref)->void
    {
        if(!node)return ; 
        search.CopySetToLast(-1) ;
        search.SetStartAnimation(-1,1) ;
        search.InsertAnimationToSet(-1,-1,NewAnimation(2,0,color,{Node(node->CurrentPosition,20,node->c)})) ;
        if(node->c==s)
        {
            search.CopySetToLast(-1) ; 
            search.SetStartAnimation(-1,1) ;
            search.InsertAnimationToSet(-1,-1,NewAnimation(2,0,GREEN,{Node(node->CurrentPosition,20,node->c)})) ; 
            return ; 
        } 
        search.CopySetToLast(-1) ;
        search.SetStartAnimation(-1,1) ;
        search.EraseAnimation(-1,-1,NewAnimation(2,0,color,{Node(node->CurrentPosition,20,node->c)})) ;
        search.InsertAnimationToSet(-1,-1,NewAnimation(3,0,color,{Node(node->CurrentPosition,20,node->c)})) ;
        if(stoi(s) < stoi(node->c))
        {
            search.CopySetToLast(-1) ;
            search.SetStartAnimation(-1,1) ;
            search.InsertAnimationToSet(-1,-1,NewAnimation(5,0,color,{Node(node->CurrentPosition,20,node->c),Node(node->left->CurrentPosition,20,node->left->c)})) ;
            FindVal_ref(node->left,s,FindVal_ref) ; 
        }
        else 
        {
            search.CopySetToLast(-1) ;
            search.SetStartAnimation(-1,1) ;
            search.InsertAnimationToSet(-1,-1,NewAnimation(5,0,color,{Node(node->CurrentPosition,20,node->c),Node(node->right->CurrentPosition,20,node->right->c)})) ;
            FindVal_ref(node->right,s,FindVal_ref) ;        
        };
    };
    FindVal(root,s,FindVal) ;
    return search;
}

void AVL::Search(std::string s )
{
    myPresentation = SearchAnimation(s, BLUE);
}
Presentation AVL::InsertAnimation( std::string s )
{
    Presentation insert ;
    if(!root)
    {
        root = new AVLNode({0,0},s,1) ;
        calcPosition(root) ; 
        insert.CreateNewPresentation() ;
        insert.CreateNewSet(-1) ;
        insert.InsertAnimationToSet(-1,-1,NewAnimation(2,0,GREEN,{Node(root->TargetedPosition,20,s)})) ;
        insert.CreateNewPresentation() ; 
        insert.CreateNewSet(-1) ; 
        insert.InsertAnimationToSet(-1,-1,NewAnimation(0,0,BLACK,{Node(root->TargetedPosition,20,s)})) ;
        insert.InsertAnimationToSet(-1,-1,NewAnimation(3,0,GREEN,{Node(root->TargetedPosition,20,s)})) ;
        return insert ; 
    } 
    insert.present = {BasicStructure(root)};
    auto MoveNode=[&](AVLNode* node,auto&&MoveNode_ref)
    {
        if(!node)return ;    
        insert.InsertAnimationToSet(-1, -1,NewAnimation(8, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c)}));
        if(node->left)
        {
            MoveNode_ref(node->left,MoveNode_ref) ; 
        }
        if(node->right)
        {
            MoveNode_ref(node->right,MoveNode_ref) ; 
        }
    } ; 
    auto MoveEdge=[&](AVLNode* node, auto&&MoveEdge_ref)
    {
        if(!node)return ;    
        if(node->left)
        {
            insert.InsertAnimationToSet(-1,-1, NewAnimation(7, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c),Node(node->left->CurrentPosition,20,node->left->c),Node(node->left->TargetedPosition,20,node->left->c)}));
            MoveEdge_ref(node->left,MoveEdge_ref) ; 
        }
        if(node->right)
        {
            insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c),Node(node->right->CurrentPosition,20,node->right->c),Node(node->right->TargetedPosition,20,node->right->c)}));
            MoveEdge_ref(node->right,MoveEdge_ref) ; 
        }
    }; 
    auto move =[&]()
    {
        ResetNodesPosition(root) ; 
        insert.CreateNewPresentation() ; 
        insert.CreateNewSet(-1) ; 
        calcPosition(root) ;
        MoveNode(root,MoveNode) ; 
        MoveEdge(root,MoveEdge) ; 
        ResetNodesPosition(root) ; 
        auto find = [&](AVLNode* node,auto &&find_ref)->void
        {
            if(!node)return ;  
            if(node->running)
            {
                insert.CopySetToLast(-1) ; 
                insert.SetStartAnimation(-1,1) ; 
                insert.InsertAnimationToSet(-1,-1,NewAnimation(2,0,ORANGE,{Node(node->CurrentPosition,20,node->c)})),node->running=0;
                insert.CopySetToLast(-1) ; 
                insert.SetStartAnimation(-1,1) ; 
                insert.EraseAnimation(-1,-1,NewAnimation(2,0,ORANGE,{Node(node->CurrentPosition,20,node->c)})); ; 
                insert.InsertAnimationToSet(-1,-1,NewAnimation(3,0,ORANGE,{Node(node->CurrentPosition,20,node->c)})) ; 
            }
            find_ref(node->left,find_ref) ; 
            find_ref(node->right,find_ref) ; 
        }; 
        find(root,find) ; 
    };
    auto RotateEdge = [&](AVLNode* node,std::set<std::pair<std::string,std::string>>&exist,auto&& RotateEdge_ref)
    {
        if(!node)return  ; 
        if(node->left)
        {
            if(exist.find({node->c,node->left->c})!=exist.end())insert.InsertAnimationToSet(-1,-1, NewAnimation(7, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c),Node(node->left->CurrentPosition,20,node->left->c),Node(node->left->TargetedPosition,20,node->left->c)}));
            RotateEdge_ref(node->left,exist,RotateEdge_ref) ; 
        }
        if(node->right)
        {
            if(exist.find({node->c,node->right->c})!=exist.end())insert.InsertAnimationToSet(-1,-1, NewAnimation(7, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c),Node(node->right->CurrentPosition,20,node->right->c),Node(node->right->TargetedPosition,20,node->right->c)}));
            RotateEdge_ref(node->right,exist,RotateEdge_ref) ; 
        }
    } ; 
    auto AddNode = [&](AVLNode*&node , std::string c,auto&&AddNode_ref )->void
    {
        insert.CopySetToLast(-1) ; 
        insert.SetStartAnimation(-1,1) ; 
        insert.InsertAnimationToSet(-1,-1,NewAnimation(2,0,ORANGE,{Node(node->CurrentPosition,20,node->c)}));
        insert.CopySetToLast(-1) ; 
        insert.SetStartAnimation(-1,1) ; 
        insert.EraseAnimation(-1,-1,NewAnimation(2,0,ORANGE,{Node(node->CurrentPosition,20,node->c)}));
        insert.InsertAnimationToSet(-1,-1,NewAnimation(3,0,ORANGE,{Node(node->CurrentPosition,20,node->c)}));
        if(stoi(c)!=stoi(node->c))
        {
            if(stoi(c)<stoi(node->c)) 
            {
                if(node->left)
                {
                    AddNode_ref(node->left,c,AddNode_ref) ;
                }
                else
                {
                    node->left = new AVLNode({30,30},c,1);       
                    node->left->running = 1 ;       
                    move() ; 
                }
            }
            else if(stoi(c)>stoi(node->c))
            {
                if(node->right)
                {
                    AddNode_ref(node->right,c,AddNode_ref) ; 
                }
                else
                {
                    node->right = new AVLNode({30,30},c,1);
                    node->right->running = 1 ; 
                    move() ; 
                }
            }
            insert.CopySetToLast(-1) ; 
            insert.SetStartAnimation(-1,1) ; 
            insert.InsertAnimationToSet(-1,-1,NewAnimation(2,0,ORANGE,{Node(node->CurrentPosition,20,node->c)}));
            insert.CopySetToLast(-1) ; 
            insert.SetStartAnimation(-1,1) ; 
            insert.EraseAnimation(-1,-1,NewAnimation(2,0,ORANGE,{Node(node->CurrentPosition,20,node->c)}));
            insert.InsertAnimationToSet(-1,-1,NewAnimation(3,0,ORANGE,{Node(node->CurrentPosition,20,node->c)}));
            node->height = 1+std::max(height(node->left),height(node->right)) ; 
            
            insert.CreateNewPresentation() ; 
            insert.CreateNewSet(-1) ; 
            
            
            int balance = getBalance(node) ;  
            std::set<std::pair<std::string,std::string>> edgeset = root->EdgeSet()  ;
            if (balance > 1 && stoi(c) < stoi(node->left->c)) 
            {
                node = rightRotate(node);
            }
            else if (balance < -1 && stoi(c) > stoi(node->right->c)) 
            {
                node = leftRotate(node);  
            }
            else if (balance > 1 && stoi(c) > stoi(node->left->c)) 
            {
                node->left = leftRotate(node->left);
                node = rightRotate(node) ;  
            }
            else if (balance < -1 && stoi(c) < stoi(node->right->c)) 
            {
                node->right = rightRotate(node->right);
                node = leftRotate(node);
            }
            calcPosition(root) ; 
            MoveNode(root,MoveNode) ; 
            RotateEdge(root,edgeset,RotateEdge) ;
            ResetNodesPosition(root) ; 
            insert.CopySetToLast(-1) ; 
            insert.SetStartAnimation(-1,1) ; 
            insert.present.push_back(BasicStructure(root)) ; 
        }
    };
    AddNode(root,s,AddNode) ;
    return insert ; 
}

void AVL::Insert(std::string s )
{
    if(find(str.begin(),str.end(),s)==str.end())str.push_back(s) ;  
    myPresentation.currentPresentation = 0 ; 
    myPresentation = InsertAnimation(s) ; 

}
Presentation AVL::DeleteAnimation(std::string s, Color color)
{
    Presentation _delete;
    if (!root) return _delete;

    _delete.present = {BasicStructure(root)};
    auto MoveNode=[&](AVLNode* node,auto&&MoveNode_ref)
    {
        if(!node)return ;    
        _delete.InsertAnimationToSet(-1, -1,NewAnimation(8, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c)}));
        if(node->left)
        {
            MoveNode_ref(node->left,MoveNode_ref) ; 
        }
        if(node->right)
        {
            MoveNode_ref(node->right,MoveNode_ref) ; 
        }
    } ; 
    auto RotateEdge = [&](AVLNode* node,std::set<std::pair<std::string,std::string>>&exist,auto&& RotateEdge_ref)
    {
        if(!node)return  ; 
        if(node->left)
        {
            if(exist.find({node->c,node->left->c})!=exist.end())_delete.InsertAnimationToSet(-1,-1, NewAnimation(7, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c),Node(node->left->CurrentPosition,20,node->left->c),Node(node->left->TargetedPosition,20,node->left->c)}));
            RotateEdge_ref(node->left,exist,RotateEdge_ref) ; 
        }
        if(node->right)
        {
            if(exist.find({node->c,node->right->c})!=exist.end())_delete.InsertAnimationToSet(-1,-1, NewAnimation(7, 0, BLACK, {Node(node->CurrentPosition,20,node->c),Node(node->TargetedPosition,20,node->c),Node(node->right->CurrentPosition,20,node->right->c),Node(node->right->TargetedPosition,20,node->right->c)}));
            RotateEdge_ref(node->right,exist,RotateEdge_ref) ; 
        }
    } ; 
    auto DeleteNode = [&](AVLNode*& node, std::string c, auto&& DeleteNode_ref) -> void {
        if (!node) return;
        
        // Animation before deleting node
        _delete.CopySetToLast(-1) ; 
        _delete.SetStartAnimation(-1,1) ; 
        _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(node->CurrentPosition, 20, node->c)}));
        _delete.CopySetToLast(-1);
        _delete.SetStartAnimation(-1, 1);
        
        if (stoi(c) < stoi(node->c)) {
            DeleteNode_ref(node->left, c, DeleteNode_ref);
        }
        else if (stoi(c) > stoi(node->c)) {
            DeleteNode_ref(node->right, c, DeleteNode_ref);
        }
        else {
            // Node to be deleted is found
            
            // Case 1: Node has one or no child
            if (!node->left) {
                AVLNode* temp = node->right;
                delete node;
                node = temp;
            }
            else if (!node->right) {
                AVLNode* temp = node->left;
                delete node;
                node = temp;
            }
            // Case 2: Node has two children
            else {
                // Find the in-order successor (smallest node in right subtree)
                AVLNode* temp = node->right->minNode();
                node->c = temp->c; // Copy the in-order successor's data
                // Delete the in-order successor
                DeleteNode_ref(node->right, temp->c, DeleteNode_ref);
            }
        }

        // If the tree has only one node, just return
        if (!node) return;
        
        // Update the height of the current node
        node->height = 1 + std::max(height(node->left), height(node->right));
        // Balance the node
        int balance = getBalance(node);
        _delete.CreateNewPresentation() ; 
        _delete.CreateNewSet(-1) ; 
        std::set<std::pair<std::string,std::string>> edgeset = root->EdgeSet()  ;

        if (balance > 1 && getBalance(node->left) >= 0) {
            node = rightRotate(node);
        }
        else if (balance < -1 && getBalance(node->right) <= 0) {
            node = leftRotate(node);
        }
        else if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            node = rightRotate(node);
        }
        else if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            node = leftRotate(node);
        }

        // Update the position and handle the animation for movement

        calcPosition(root);
        MoveNode(root, MoveNode);
        RotateEdge(root,edgeset,RotateEdge) ; 
        ResetNodesPosition(root);
        _delete.CopySetToLast(-1);
        _delete.SetStartAnimation(-1, 1);
        _delete.present.push_back(BasicStructure(root));
    };
    
    DeleteNode(root, s, DeleteNode);

    return _delete;
}

void AVL::Delete(std::string s )
{
    myPresentation.currentPresentation = 0 ; 
    if(find(str.begin(),str.end(),s)!=str.end())str.erase(find(str.begin(),str.end(),s)) ; 
    myPresentation = DeleteAnimation(s,RED) ; 
}

void AVL::DrawToolBar()
{
    toolBarButtons[0][0].DrawButton(0.3, 0.1, LIME, true); 
    DrawTextureEx(flagToolBarButtons[0][0] == false ? toolBarRightArrow : toolBarLeftArrow, Vector2{2.5, 570 + 183.0 / 2 - 16.0 / 2}, 0, 1, WHITE);
    if (flagToolBarButtons[1][3] == true) 
    {
        DrawTextureEx(dropFile, Vector2{float(GetScreenWidth() / 2.0 - 354.25), float(GetScreenHeight() / 2.0 - 200)}, 0, 0.5, Fade(LIME, 0.8));
    }
    if (flagToolBarButtons[0][0] == true) 
    {
        if (flagToolBarButtons[1][4] == true)
        {
            enterList.DrawTextBox();
        }
        if (flagToolBarButtons[2][0] == true) // Insert
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2}, 40, 35, (char *)"v ="};
            Button i = {Vector2{17 + 160 + 2, 607}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[3][0] == true) // Delete
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[4][0] == true) // Update
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 40, 35, (char *)"v ="};
            Button i = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[5][0] == true) // Search
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }

        for (int i = 1; i < 6; ++i) 
        {
            for (int j = 0; j < (int)toolBarButtons[i].size(); ++j) 
            {
                if (j == 0 || flagToolBarButtons[i][0] == true)
                {
                    toolBarButtons[i][j].DrawButtonAndText(0.3, 0.1, LIME, true, robotoFont, 22, RAYWHITE);
                }
            }
        }
    }
}

void AVL::HandleToolBar()
{
    if (toolBarButtons[0][0].CheckMouseClickInRectangle()) // Open_Close Bar
    {
        flagToolBarButtons[0][0] = !flagToolBarButtons[0][0];
        for (int i = 1; i < 6; ++i)
        {
            std::fill(flagToolBarButtons[i].begin(), flagToolBarButtons[i].end(), false);
        }
    }
    if (flagToolBarButtons[0][0] == true) 
    {
        for (int i = 1; i < 6; ++i) 
        {
            for (int j = 0; j < int(toolBarButtons[i].size()); ++j) 
            {
                if (toolBarButtons[i][j].CheckMouseClickInRectangle()) {
                    for (int i = 1; i < 6; ++i) 
                    {
                        std::fill(flagToolBarButtons[i].begin() + (j > 0), flagToolBarButtons[i].end(), false);
                    }
                    flagToolBarButtons[i][j] = true;
                    if (i == 2 && j == 0)
                    {
                        insertI.oldWidth = 60;
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 3 && j == 0)
                    {
                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 4 && j == 0)
                    {
                        insertI.oldWidth = 60;
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 5 && j == 0)
                    {
                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                }
            }
        }
        if (flagToolBarButtons[1][1] == true)  // Clear data
        {
            flagToolBarButtons[1][1] = false;
            ClearAllData();
            return;
        }
        if (flagToolBarButtons[1][2] == true)  // Creat random data
        {
            flagToolBarButtons[1][2] = false;
            RandomNewData();
            return;
        }
        if (flagToolBarButtons[1][3] == true)  // Input data from file
        {
            flagToolBarButtons[1][0] = false;
            ClearAllData();
            if (IsFileDropped() == true)
            {
                flagToolBarButtons[1][3] = false;
                InputDataFromFile();
            }
            return;
        }
        if (flagToolBarButtons[1][4] == true)  // Input data from keyboard
        {
            std::string listChar = enterList.HandleTextBox();
            if (listChar.empty() == false)
            {
                flagToolBarButtons[1][4] = false;
                std::vector<std::string> str = StringToVector(listChar);
                
                BuildNodeFromValue(str);
                BuildCreateAnimation();
            }
            return;
        }
        if (flagToolBarButtons[2][0] == true) // Insert
        {
            std::string i = insertI.HandleTextBox();
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[2][0] = false;
            std::string vValue = StringToVector(v)[0];
            Insert(vValue);
    
            return;
        }
        if (flagToolBarButtons[3][0] == true) // Delete
        {
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[3][0] = false;
            std::string vValue = StringToVector(v)[0];

            Delete(vValue);
    
            return;
        }
        if (flagToolBarButtons[5][0] == true) // Search
        {
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[5][0] = false;
            std::string vValue = StringToVector(v)[0];
            Search(vValue);
    
            return;
        }
    }
}

std::vector<std::string> AVL::StringToVector(std::string listChar)
{
    std::vector<std::string> res;
    std::string curValue = "";
    for (char &c: listChar)
    {
        bool flag = 0 ;  
        if('0'<=c&&c<='9')
        {
            curValue+=c ; 
            flag = 1 ;
        }
        if(!flag)
        {
            res.push_back(curValue);
            curValue = "" ;  
        }
    }
    if (curValue != "") ; 
    {
        res.push_back(curValue);
    }
    return res;
}

void AVL::ResetNodesPosition(AVLNode* root)
{
    if(!root)return ; 
    root->CurrentPosition = root->TargetedPosition;
    ResetNodesPosition(root->left) ; 
    ResetNodesPosition(root->right) ; 
}
void AVL::Draw() 
{  
    ResetNodesPosition(root) ;
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, fontNumber, GetFontDefault(), isLightMode, speed, curRemoteState);
}
