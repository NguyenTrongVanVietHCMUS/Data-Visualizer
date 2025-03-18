#include "Trie.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

void Trie::Run() 
{
    HandleToolBar();
    DrawToolBar();
    Draw();
}

void Trie::Init()
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

    // enterList.Init();
    enterList.oldWidth = 80;
    enterList.textBox = {Vector2{425 + 80 + 2, 570}, 80, 35};
    enterList.confirm = {Vector2{425 + 80 + 2, 570 + 35 + 2}, 80, 35, (char *)"Confirm"};

    nodes.clear();
    root = new TrieNode({(float)GetScreenWidth() / 2, 50.0 }, '\0');
    isLightMode = 1;

    speed = 0.05;
    posAnimation = 0;
    animations.clear();
}

Vector2 Trie::calcPosition(TrieNode *root) {
    Vector2 width = {0, 0};
    std::vector<Vector2> widthList;
    std::vector<TrieNode*> queue;
    for (auto &child : root->children) {
        queue.push_back(child);
        widthList.push_back(calcPosition(child));
    }
    if (queue.size() == 0) return width;
    int mid1, mid2;
    if (queue.size() % 2 == 0) {
        mid1 = queue.size() / 2 - 1;
        mid2 = queue.size() / 2;
        queue[mid1]->setTargetedPosition({- (widthList[mid1].y + xOFFSET / 2), yOFFSET});
        queue[mid2]->setTargetedPosition({widthList[mid2].x + xOFFSET / 2, yOFFSET});
        width.x = widthList[mid1].x + widthList[mid1].y + xOFFSET / 2;
        width.y = widthList[mid2].x + widthList[mid2].y + xOFFSET / 2;
    } else {
        mid1 = queue.size() / 2;
        mid2 = queue.size() / 2;
        queue[mid1]->setTargetedPosition({0, yOFFSET});
        width = widthList[mid1];
    }
    for (int i = mid1 - 1; i >= 0; i--) {
        queue[i]->setTargetedPosition({- (width.x + widthList[i].y + xOFFSET), yOFFSET});
        width.x += widthList[i].x + widthList[i].y + xOFFSET;
    }
    for (int i = mid2 + 1; i < queue.size(); i++) {
        queue[i]->setTargetedPosition({width.y + widthList[i].x + xOFFSET, yOFFSET});
        width.y += widthList[i].x + widthList[i].y + xOFFSET;
    }
    return width;
}

Presentation Trie::CreateAnimation(TrieNode* root)
{
    calcPosition(root) ;    
    Presentation create;
    create.CreateNewPresentation() ; 
    create.CreateNewSet(-1) ;
    {// Add all nodes
        auto AddNode = [&](TrieNode* node, auto&& AddNode_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            // draw the node first 
            create.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {Node(cur,24,node->c)}));
            for (auto& child : node->children) 
            {
                AddNode_ref(child, AddNode_ref,cur+child->TargetedPosition);
            }
        };
        AddNode(root, AddNode,{0,0});
    }
    { // Insert all edges
        auto AddEdge = [&](TrieNode* node, auto&& AddEdge_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            for (auto& child : node->children) 
            {
                AddEdge_ref(child, AddEdge_ref,cur+child->TargetedPosition);
                create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {Node(cur,24,node->c),Node(cur+child->TargetedPosition,24,node->c)}));
            }
        };
        AddEdge(root, AddEdge,{0,0}); 
    }
    { // Remove animations
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        auto AddNode = [&](TrieNode* node, auto&& AddNode_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            // draw the node first 
            create.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, GREEN, {Node(cur,24,node->c)}));
            for (auto& child : node->children) 
            {
                AddNode_ref(child, AddNode_ref,cur+child->TargetedPosition);
            }
        };
        AddNode(root, AddNode,{0,0});
        auto AddEdge = [&](TrieNode* node, auto&& AddEdge_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            for (auto& child : node->children) 
            {
                AddEdge_ref(child, AddEdge_ref,cur+child->TargetedPosition);
                create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, GREEN, {Node(cur,24,node->c),Node(cur+child->TargetedPosition,24,node->c)}));
            }
        };
        AddEdge(root, AddEdge,{0,0});
        auto DelNode = [&](TrieNode* node, auto&& DelNode_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            // draw the node first 
            create.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {Node(cur,24,node->c)}));
            for (auto& child : node->children) 
            {
                DelNode_ref(child, DelNode_ref,cur+child->TargetedPosition);
            }
        };
        DelNode(root, DelNode,{0,0});
        auto DelEdge = [&](TrieNode* node, auto&& DelEdge_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            for (auto& child : node->children) 
            {
                DelEdge_ref(child, DelEdge_ref,cur+child->TargetedPosition);
                create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {Node(cur,24,node->c),Node(cur+child->TargetedPosition,24,node->c)}));
            }
        };
        DelEdge(root, DelEdge,{0,0});
    }
    return create;
}

void Trie::BuildCreateAnimation()
{
    myPresentation.currentPresentation = 0;
    myPresentation = CreateAnimation(root);
}

void Trie::ClearAllData()
{
    root = new TrieNode({(float)GetScreenWidth() / 2, 50.0 }, '\0');
    myPresentation.Clear();
}
void Trie::Insert(std::string s)
{
    TrieNode *cur = root ; 
    for(auto c:s)
    {
        bool found = 0 ; 
        for(auto x : cur->children)
        {   
            if(x->c == c)
            {
                cur = x ; 
                found = 1 ; 
                break ; 
            }
        }
        if(found==0)
        {
            cur->children.push_back(new TrieNode({0,0},c)) ;
            sort(cur->children.begin(),cur->children.end(),[](TrieNode* a , TrieNode *b ){return a->c < b->c ; }) ;
            for(auto x:cur->children)
            {
                if(x->c == c)
                {
                    cur = x ; 
                    break ; 
                }
            }
        }
    }
}
void Trie::BuildNodeFromValue(std::vector<std::string>list)
{
    str = list ;  
    root = new TrieNode({(float)GetScreenWidth() / 2, 50.0 }, '\0');
    for(auto s:str)
    {
        Insert(s) ; 
    }
}
void Trie::RandomNewData()
{
    ClearAllData();

    int n = GetRandomValue(1, 5);
    std::vector<std::string> str ;
    for (int i = 1; i <= n; ++i)
    {
        int len = GetRandomValue(3,6) ; 
        std::string tmp = "" ;  
        for(int j=0;j<len;j++)
        {   
            tmp+=char(GetRandomValue('A','Z')) ;
        }
        str.push_back(tmp) ; 
    }
    BuildNodeFromValue(str) ; 
    BuildCreateAnimation();
}
void Trie::InputDataFromFile()
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
        if (('A' <= c && c <= 'Z'))
        {
            curValue+=c ; 
            flag = 1 ; 
        }
        else if('a' <= c && c <= 'z')
        {
            curValue+=c-'a'+'A' ; 
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

SetOfAnimation Trie::BasicStructure(const std::vector<Node> &nodes)
{
    SetOfAnimation basicStructure;
    if (nodes.empty() == false) { 
        basicStructure.CreateNewSet();
        for (Node curNode: nodes) basicStructure.InsertAnimationToSet(-1, NewAnimation(0, 0, BLACK, {curNode}));
        for (int i = 1; i < int(nodes.size()); ++i) basicStructure.InsertAnimationToSet(-1, NewAnimation(4, 0, BLACK, {nodes[i - 1], nodes[i]}));
    }
    return basicStructure;
}

Presentation Trie::SearchAnimation( std::string s , Color color)
{
    Presentation search;

    return search;
}

void Trie::Search(std::string s )
{
}

Presentation Trie::InsertAnimation( std::string s )
{
    Presentation insert ; 
    return insert ; 
}

Presentation Trie::DeleteAnimation(std::string s )
{
    Presentation _delete ;
    return _delete;
}

void Trie::Delete(std::string s )
{

}

void Trie::DrawToolBar()
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
                    toolBarButtons[i][j].DrawButtonAndText(0.3, 0.1, LIME, true, toolBarFont, 22, RAYWHITE);
                }
            }
        }
    }
}

void Trie::HandleToolBar()
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
            std::string v = insertV.HandleTextBox();
            // if (i.empty() == true) 
            // {
            //     return;
            // }
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

std::vector<std::string> Trie::StringToVector(std::string listChar)
{
    std::vector<std::string> res;
    std::string curValue = "";
    for (char &c: listChar)
    {
        bool flag = 0 ;  
        if (('A' <= c && c <= 'Z'))
        {
            curValue+=c ; 
            flag = 1 ; 
        }
        else if('a' <= c && c <= 'z')
        {
            curValue+=c-'a'+'A' ; 
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


void Trie::Draw() 
{  
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, GetFontDefault(), GetFontDefault(), isLightMode, speed);
}
