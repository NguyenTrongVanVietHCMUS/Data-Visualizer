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
    
    HandleRemote();
    DrawRemote();
}

void Trie::Init()
{
    toolBarButtons.resize(6);
    toolBarButtons[0] = 
    {
        {Vector2{0, 570}, 15, 183 - 35 - 2, (char *)"Open_Close"}
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
        {Vector2{17, 681}, 160, 35, (char *)"Search"}
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
    enterList.oldHeight = 35 ;
    enterList.textBox = {Vector2{425 + 80 + 2, 570}, 80, 35};
    enterList.confirm = {Vector2{425 + 80 + 2, 570 + 35 + 2}, 80, 35, (char *)"Confirm"};

    nodes.clear();
    root = new TrieNode({(float)GetScreenWidth() / 2, 50.0 }, "");
    isLightMode = 1;


    speed = 0.01;
    changeSpeed.position = Vector2{0, 750};
    changeSpeed.width = 45;
    changeSpeed.height = 45;
    changeSpeed.text = "x1";


    remoteButtons.resize(7);
    remoteButtons[0] = {{71, 754}, 31, 34, (char*)"undo"};
    remoteButtons[1] = {{132, 757}, 43, 31, (char*)"pre"};
    remoteButtons[2] = {{272, 757}, 44, 32, (char*)"next"};
    remoteButtons[3] = {{350, 754}, 31, 34, (char*)"redo"};
    remoteButtons[4] = {{202, 747}, 44, 50, (char*)"pause"};
    remoteButtons[5] = {{205, 750}, 37, 47, (char*)"continue"};
    remoteButtons[6] = {{198, 743}, 52, 54, (char*)"repeat"};

    flagRemote.assign(7, true);
    flagRemote[5] = false;
    flagRemote[6] = false;
    for (int i = 0; i < 7; ++i)
    {
        remoteButtons[i].texture = remoteTextures[i];
    }

    createList = {
        "Create new Trie!"
    };
    searchList = {
        "cur = root",
        "for(auto x : s)",
        "   if cur->children[x] exist  : cur = cur->children[x]" ,
        "   else : return 0 ;", 
        "return 1" 
    };
    updateList = {
        
    };
    insertList = {
        "cur = root",
        "for(auto x : s)",
        "   if cur->children[x]  doesn't exist : create cur->children[x]", 
        "   cur = cur->pNext,cur->sl++",
    };
    deleteList = {
        "cur = root",
        "for(auto x : s)",
        "   cur = cur->children[x]" ,
        "   cur->sl--",  
        "   if cur->sl==0 : delete cur",
    };
    positionBoard = Vector2{1600 - 400, 800 - 300};
    width = 400;
    height = 300;


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
            create.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {Node(cur,20,node->c)},0,{0}));
            for (auto& child : node->children) 
            {
                AddNode_ref(child, AddNode_ref,cur);
            }
        };
        AddNode(root, AddNode,{0,0});
    }
    { // Insert all edges
        create.CopySetToLast(-1);
        create.SetStartAnimation(-1, -1, 1);
        create.CreateNewSet(-1);
        auto AddEdge = [&](TrieNode* node, auto&& AddEdge_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            for (auto& child : node->children) 
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {Node(cur,20,node->c),Node(cur+child->TargetedPosition,20,node->c)},0,{0}));
                AddEdge_ref(child, AddEdge_ref,cur);
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
            create.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {Node(cur,20,node->c)},0,{0}));
            for (auto& child : node->children) 
            {
                AddNode_ref(child, AddNode_ref,cur);
            }
        };
        AddNode(root, AddNode,{0,0});
        auto AddEdge = [&](TrieNode* node, auto&& AddEdge_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            for (auto& child : node->children) 
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {Node(cur,20,node->c),Node(cur+child->TargetedPosition,20,node->c)},0,{0}));
                AddEdge_ref(child, AddEdge_ref,cur);
            }
        };
        AddEdge(root, AddEdge,{0,0});
        auto DelNode = [&](TrieNode* node, auto&& DelNode_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            // draw the node first 
            create.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {Node(cur,20,node->c)},0,{0}));
            for (auto& child : node->children) 
            {
                DelNode_ref(child, DelNode_ref,cur);
            }
        };
        DelNode(root, DelNode,{0,0});
        auto DelEdge = [&](TrieNode* node, auto&& DelEdge_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->TargetedPosition ;
            for (auto& child : node->children) 
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {Node(cur,20,node->c),Node(cur+child->TargetedPosition,20,node->c)},0,{0}));
                DelEdge_ref(child, DelEdge_ref,cur);
            }
        };
        DelEdge(root, DelEdge,{0,0});
    }
    return create;
}

void Trie::BuildCreateAnimation()
{
    myPresentation = CreateAnimation(root);
    myPresentation.InitBoardText(createList, positionBoard, width, height);
    myPresentation.currentPresentation = 0;
    myPresentation.numberOfPresentation = 0;
    curRemoteState = 0;
}

void Trie::ClearAllData()
{
    root = new TrieNode({(float)GetScreenWidth() / 2, 50.0 }, "");
    str.clear() ; 
    myPresentation.Clear();
}
void Trie::Add(std::string s)
{
    TrieNode *cur = root ; 
    root->sl++ ; 
    for(auto c:s)
    {
        bool found = 0 ; 
        for(auto x : cur->children)
        {   
            if(x->c[0]== c)
            {
                cur = x ; 
                found = 1 ; 
                break ; 
            }
        }
        if(found==0)
        {
            cur->children.push_back(new TrieNode({0,0},{c})) ;
            sort(cur->children.begin(),cur->children.end(),[](TrieNode* a , TrieNode *b ){return a->c < b->c ; }) ;
            for(auto x:cur->children)
            {
                if(x->c[0] == c)
                {
                    cur = x ; 
                    break ; 
                }
            }
        }
        cur->sl++; 
    }
}
void Trie::BuildNodeFromValue(std::vector<std::string>list)
{
    str = list ;  
    root = new TrieNode({(float)GetScreenWidth() / 2, 50.0 }, "");
    for(auto s:str)
    {
        Add(s) ; 
    }
}
void Trie::RandomNewData()
{
    ClearAllData();

    int n = GetRandomValue(1, 5);
    std::vector<std::string> str ;
    for (int i = 1; i <= n; ++i)
    {
        int len = GetRandomValue(3,8) ; 
        std::string tmp = "" ;  
        for(int j=0;j<len;j++)
        {   
            tmp+=char(GetRandomValue('A','C')) ;
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

    while(getline(fin, s))
    {

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
    }
    fin.close();
    BuildNodeFromValue(str);
    BuildCreateAnimation();
}
    
SetOfAnimation Trie::BasicStructure(TrieNode* root )
{
    SetOfAnimation basicStructure;
    { 
        basicStructure.CreateNewSet() ;
        auto AddNode = [&](TrieNode* node, auto&& AddNode_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->CurrentPosition ;
            // draw the node first 
            basicStructure.InsertAnimationToSet(-1, NewAnimation(0, 0, BLACK, {Node(cur,20,node->c)}));
            for (auto& child : node->children) 
            {
                AddNode_ref(child, AddNode_ref,cur);
            }
        };
        AddNode(root, AddNode,{0,0});
        auto AddEdge = [&](TrieNode* node, auto&& AddEdge_ref,Vector2 cur) -> void 
        {
            if (!node) return;
            cur+=node->CurrentPosition ;
            for (auto& child : node->children) 
            {
                basicStructure.InsertAnimationToSet(-1, NewAnimation(4, 0, BLACK, {Node(cur,20,node->c),Node(cur+child->CurrentPosition,20,child->c)}));
                AddEdge_ref(child, AddEdge_ref,cur);
            }
        };
        AddEdge(root, AddEdge,{0,0});
    }
    return basicStructure;
}

Presentation Trie::SearchAnimation( std::string s , Color color)
{
    Presentation search;
    search.present = {BasicStructure(root)};
    {
        TrieNode* cur = root ;  
        Vector2 curPos = {0,0} ;
        for(auto c  : s)
        {   
            curPos+=cur->CurrentPosition ;
            search.CopySetToLast(-1);
            search.SetStartAnimation(-1, 1);
            if(cur==root)
            {
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(curPos,20,cur->c)},0,{0}));
            }
            bool ok = 0  ;
            for(auto x : cur->children)
            {
                search.CreateNewSet(-1) ; 
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
                if(x->c[0]==c)
                {
                    search.CreateNewSet(-1);

                    search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(curPos+x->CurrentPosition,20,x->c)},0,{2}));
                    ok = 1 ; 
                    cur = x ; 
                    break ; 
                }
            }
            if(ok==0)
            {
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {3}));
                break;  
            }
        }
    }
    return search;
}

void Trie::Search(std::string s )
{
    myPresentation = SearchAnimation(s, BLUE);
    myPresentation.InitBoardText(searchList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}
// #include <char>
Presentation Trie::InsertAnimation( std::string s )
{
    Presentation insert ;
    auto SearchAnimation = [&]( std::string s , Color color)->Presentation
    {
        Presentation search;
        search.present = {BasicStructure(root)};
        {
            TrieNode* cur = root ;
            
            Vector2 curPos = {0,0} ;
            for(auto c  : s)
            {   
                curPos+=cur->CurrentPosition ;
                search.CopySetToLast(-1);
                search.SetStartAnimation(-1, 1);
                if(cur==root)
                {
                    search.CreateNewSet(-1);
                    search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(curPos,20,cur->c)},0,{0}));
                    if(cur->children.empty())return search;    
                }
                bool ok = 0  ;
                for(auto x : cur->children)
                {
                    search.CreateNewSet(-1) ; 
                    search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
                    if(x->c[0]==c)
                    {
                        search.CreateNewSet(-1);
                        search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(curPos+x->CurrentPosition,20,x->c)},0,{3}));
                        ok = 1 ; 
                        cur = x ; 
                        break ; 
                    }
                }
            }
        }
        return search;
    };
    insert =  SearchAnimation(s,ORANGE) ;  
    {
        TrieNode* cur = root ;
        int pos = -1 ; 
        for(int i=0;i<s.size();i++)
        {
            bool found = 0 ; 
            for(auto x : cur->children)
            {
                if(x->c[0]==s[i])
                {
                    cur = x ; 
                    found = 1;  
                    break ; 
                }
            }
            if(found==0)
            {
                pos = i ; 
                break; 
            }
            else cur->sl++ ; 
        }
        if(pos==-1)return insert ; 
        for(int i=pos;i<s.size();i++)
        {
            cur->children.push_back(new TrieNode({0,0},{s[i]})) ; 
            cur->children.back()->sl++;
            cur->children.back()->running = 1 ; 
            sort(cur->children.begin(),cur->children.end(),[](TrieNode* a , TrieNode *b ){return a->c < b->c ; }) ;
            calcPosition(root) ; 
             
            insert.CreateNewPresentation() ;
            insert.CreateNewSet(-1) ;  
            auto MoveNode = [&](TrieNode* node, auto&& MoveNode_ref,Vector2 cur, Vector2 nex)->void
            {
                if(!node)return ; 
                cur+=node->CurrentPosition ;
                nex+=node->TargetedPosition ;
                insert.InsertAnimationToSet(-1, -1,NewAnimation(8, 0, BLACK, {Node(cur,20,node->c),Node(nex,20,node->c)}));
                for(auto x:node->children)
                {
                    if(!x->running)
                    {
                        insert.InsertAnimationToSet(-1,-1, NewAnimation(7, 0, BLACK, {Node(cur,20,node->c),Node(nex,20,node->c),Node(cur+x->CurrentPosition,20,node->c),Node(nex+x->TargetedPosition,20,x->c)}));
                        MoveNode_ref(x,MoveNode_ref,cur,nex) ; 
                    }
                }
            };
            MoveNode(root,MoveNode,{0,0},{0,0}) ;
            Vector2 curPos ={0,0} ; 
            auto calc = [&](TrieNode* node, auto&& calc_ref,Vector2 cur)->void
            {
                if(!node)return ; 
                cur+=node->TargetedPosition;
                // if(node->running)curPos = cur ;  
                for(auto x:node->children)
                {
                    if(x->running)
                    {
                        curPos = cur ; 
                        return ; 
                    }
                    calc_ref(x,calc_ref,cur) ; 
                }
            };
            calc(root,calc,{0,0}) ; 
            insert.CopySetToLast(-1) ;
            insert.SetStartAnimation(-1,1) ;
            for(auto x:cur->children)
            {
                if(x->c[0] == s[i])
                {
                    insert.CreateNewSet(-1) ; 
                    insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
                    insert.CreateNewSet(-1) ; 
                    insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {Node(curPos,20,cur->c),Node(curPos+x->TargetedPosition,20,x->c)},0,{2}));
                    insert.InsertAnimationToSet(-1,-1, NewAnimation(2, 0, ORANGE, {Node(curPos+x->TargetedPosition,20,x->c)},0,{2}));
                    x->running = 0 ;                                          
                    cur = x ;
                    break ;
                }
            }
            ResetNodesPosition(root) ; 
        }
    }
    return insert ; 
}

void Trie::Insert(std::string s )
{
    if(find(str.begin(),str.end(),s)==str.end())str.push_back(s) ;  
    myPresentation = InsertAnimation(s) ; 
    myPresentation.InitBoardText(insertList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;

}
Presentation Trie::DeleteAnimation(std::string s , Color color)
{
    Presentation _delete  ; 
    auto SearchAnimation = [&]( std::string s , Color color)->Presentation
    {
        Presentation search;
        search.present = {BasicStructure(root)};
        {
            TrieNode* cur = root ;
            
            Vector2 curPos = {0,0} ;
            for(auto c  : s)
            {   
                curPos+=cur->CurrentPosition ;
                search.CopySetToLast(-1);
                search.SetStartAnimation(-1, 1);
                if(cur==root)
                {
                    search.CreateNewSet(-1);
                    search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(curPos,20,cur->c)},0,{0}));
                    if(cur->children.empty())return search;    
                }
                if(cur->sl==1)
                {
                    search.CreateNewSet(-1) ;
                    search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {4}));
                    return search;
                }
                bool ok = 0  ;
                for(auto x : cur->children)
                {
                    if(x->c[0]==c)
                    {
                        search.CreateNewSet(-1) ; 
                        search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
                        search.CreateNewSet(-1);
                        search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(curPos+x->CurrentPosition,20,x->c)},0,{2}));
                        search.CreateNewSet(-1);
                        search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {3}));
                        ok = 1 ; 
                        cur = x ; 
                        break ; 
                    }
                }
            }
        }
        return search;
    };
    _delete = SearchAnimation(s,ORANGE) ;
    ResetNodesPosition(root) ; 
    {
        root->sl = 971419248;
        auto DelNode =[&](TrieNode* node, Vector2 cur,auto&& DelNode_ref,std::string&s , int pos)->void
        {
            if(!node)return ; 
            cur+=node->CurrentPosition ; 
            node->sl-- ; 
            _delete.CreateNewSet(-1) ; 
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(cur,20,node->c)})); 
            if(node->sl==0)
            {
                _delete.CopySetToLast(-1) ; 
                _delete.SetStartAnimation(-1,1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {Node(cur,20,node->c)})); 
                _delete.EraseAnimation(-1,-1,NewAnimation(2,0,color,{Node(cur,20,node->c)})) ;
                _delete.EraseAnimation(-1,0,NewAnimation(0,0,BLACK,{Node(cur,20,node->c)})) ;
                return ; 
            }
            if(pos==s.size())return ;  
            for(auto child:node->children)
            {
                if(child->c[0]==s[pos])
                {
                    DelNode_ref(child,cur,DelNode_ref,s,pos+1) ; 

                    if(child->sl==0)
                    {
                        _delete.CopySetToLast(-1) ; 
                        _delete.SetStartAnimation(-1,1) ; 
                        _delete.EraseAnimation(-1,0,NewAnimation(4,0,BLACK,{Node(cur,20,node->c),Node(cur+child->CurrentPosition,20,child->c)})) ;
                    }
                    break ; 

                }
            }
        };
        DelNode(root,{0,0},DelNode,s,0) ;
        auto calcPosition = [&](TrieNode *root,auto&& calcPosition_ref)->Vector2 
        {
            Vector2 width = {0, 0};
            std::vector<Vector2> widthList;
            std::vector<TrieNode*> queue;
            for (auto &child : root->children)if(child->sl)
            {
                queue.push_back(child);
                widthList.push_back(calcPosition_ref(child,calcPosition_ref));
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
        }; 
        calcPosition(root,calcPosition) ;
        _delete.CreateNewPresentation() ; 
        _delete.CreateNewSet(-1) ; 
        auto MoveEdge =[&](TrieNode* root , Vector2 cur , Vector2 nex , auto&& MoveEdge_ref)->void
        {
            if(!root)return ; 
            cur+=root->CurrentPosition ; 
            nex+=root->TargetedPosition ; 
            _delete.InsertAnimationToSet(-1,-1,NewAnimation(8,0,BLACK,{Node(cur,20,root->c),Node(nex,20,root->c)})) ;
            for(auto child:root->children)if(child->sl)
            {
                _delete.InsertAnimationToSet(-1,-1,NewAnimation(7,0,BLACK,{Node(cur,20,root->c),Node(nex,20,root->c),Node(cur+child->CurrentPosition,20,root->c),Node(nex+child->TargetedPosition,20,child->c)})) ;
                MoveEdge_ref(child,cur,nex,MoveEdge_ref) ; 
            }
        };
        MoveEdge(root,{0,0},{0,0},MoveEdge) ; 
        auto RemoveNode = [&](TrieNode*&node,auto&& RemoveNode_ref)->void
        {
            for(auto&child:node->children)
            {
                if(child->sl==0)
                {
                    node->children.erase(std::find(node->children.begin(),node->children.end(),child)) ;
                }
                RemoveNode_ref(child,RemoveNode_ref) ; 
            }
        };
        RemoveNode(root,RemoveNode) ;
    }
    return _delete;
}

void Trie::Delete(std::string s )
{
    myPresentation.currentPresentation = 0 ; 
    if(std::find(str.begin(),str.end(),s)==str.end())
    {
        myPresentation.present = {BasicStructure(root)};
        return ; 
    }
    str.erase(std::find(str.begin(),str.end(),s)) ;
    myPresentation = DeleteAnimation(s,RED) ; 
    myPresentation.InitBoardText(deleteList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
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
            Button v = {Vector2{17 + 160 + 2, 607 }, 40, 35, (char *)"v ="};
            // Button i = {Vector2{17 + 160 + 2, 607}, 40, 35, (char *)"i ="};
            // i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            // insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[3][0] == true) // Delete
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[4][0] == true) // search
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 }, 40, 35, (char *)"v ="};
            // Button i = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2}, 40, 35, (char *)"i ="};
            // i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            // insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        // if (flagToolBarButtons[5][0] == true) // Search
        // {
        //     Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 40, 35, (char *)"v ="};
        //     v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
        //     insertV.DrawTextBox();
        // }

        for (int i = 1; i < 5; ++i) 
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
                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35, (char *)"Confirm"};

                        // insertV.oldWidth = 60;
                        // insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35};
                        // insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 3 && j == 0)
                    {
                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 4 && j == 0)
                    {
                        // insertI.oldWidth = 60;
                        // insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35};
                        // insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    // if (i == 5 && j == 0)
                    // {
                    //     insertV.oldWidth = 60;
                    //     insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35};
                    //     insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    // }
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
            // std::string i = insertI.HandleTextBox();
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
        if (flagToolBarButtons[4][0] == true) // Search
        {
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[4][0] = false;
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
        if(c==' '||c=='\n')
        {
            if(curValue!="")
            {
                res.push_back(curValue);
                curValue = "" ; 
            }
        }
        else curValue+=c ; 
    }
    if (curValue != "") ; 
    {
        res.push_back(curValue);
    }
    return res;
}

void Trie::ResetNodesPosition(TrieNode* root)
{
    root->CurrentPosition = root->TargetedPosition;
    for (auto& child : root->children)
    {
        ResetNodesPosition(child);
    }
}
void Trie::Draw() 
{  
    ResetNodesPosition(root) ;
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, fontNumber,robotoFont, isLightMode, speed, curRemoteState);
}

void Trie::HandleRemote()
{
    auto SetCurAnimation = [&](int _start, int _end, int _cur)
    {
        for (int k = _start; k <= _end; ++k)
        {
            for (int i = 0; i < int(myPresentation.present[k].setOfAnimation.size()); ++i)
            {
                for (NewAnimation &curAnimation: myPresentation.present[k].setOfAnimation[i])
                {
                    curAnimation.curAnimation = _cur;
                }
            }
        }
        return;
    };
    auto _SetCurAnimation = [&](int currentPresentation, int _end, int _cur)
    {
        for (int i = 0; i <= _end; ++i)
        {
            for (NewAnimation &curAnimation: myPresentation.present[currentPresentation].setOfAnimation[i])
            {
                curAnimation.curAnimation = _cur;
            }
        }
        return;
    };
    Vector2 mousePos = GetMousePosition();
    const int screenWidth = 1600;
    const int screenHeight = 800;

    // Vị trí và kích thước thanh slider giống như trong DrawRemote
    int sliderX = 450;
    int sliderY = screenHeight - 30;
    int sliderWidth = screenWidth - 450 - 450;
    int sliderHeight = 8;

    // Bán kính nút trượt
    int handleRadius = 10;

    // Kiểm tra nếu chuột nằm trong vùng thanh slider
    Rectangle sliderRect = { (float)sliderX, (float)(sliderY - handleRadius), 
                             (float)sliderWidth, (float)(sliderHeight + 2 * handleRadius) };

    // Kéo thanh tua
    if (CheckCollisionPointRec(mousePos, sliderRect) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        float relativeX = mousePos.x - sliderX;

        // Giới hạn giá trị nằm trong khoảng cho phép
        if (relativeX < 0) relativeX = 0;
        if (relativeX > sliderWidth) relativeX = (float)sliderWidth;

        float progress = relativeX / sliderWidth;
        int totalFrames = myPresentation.CountNumberOfAnimation();
        int newFrame = (int)(progress * totalFrames + 0.5f);

        // Cập nhật trạng thái trình chiếu
        int i, j;
        myPresentation.numberOfPresentation = newFrame;
        myPresentation.FindPosition(newFrame, i, j);
        // std::cerr << newFrame << ' ' << i << ' ' << j << '\n';
        myPresentation.currentPresentation = i;
        myPresentation.SetCurToStartAnimation(i + 1, -1);
        myPresentation.SetCurToStartAnimationInOnePresentation(i, j + 1);
        SetCurAnimation(0, i - 1, 1);
        _SetCurAnimation(i, j + 0, 1);
        
        
        curRemoteState = 1; // Chuyển về trạng thái play nếu đang pause
    }

    // Các xử lý nút điều khiển khác giữ nguyên
    if (remoteButtons[4 + curRemoteState].CheckMouseClickInRectangle() == true)
    {
        if (curRemoteState == 2)
        {
            myPresentation.SetCurToStartAnimation(0, -1);
            myPresentation.currentPresentation = 0;
            myPresentation.numberOfPresentation = 0;
        }
        curRemoteState = !curRemoteState;
    }
    if (remoteButtons[0].CheckMouseClickInRectangle() == true)
    {
        curRemoteState = 1;
        myPresentation.SetCurToStartAnimation(0, -1);
        myPresentation.currentPresentation = 0;
        myPresentation.numberOfPresentation = 0;
    }
    if (remoteButtons[3].CheckMouseClickInRectangle() == true)
    {
        if (myPresentation.numberOfPresentation != myPresentation.CountNumberOfAnimation())
        {
            curRemoteState = 1;
            myPresentation.SetCurAnimation(0, -1, 1);
            myPresentation.numberOfPresentation = myPresentation.CountNumberOfAnimation();
        }
    }
    if (remoteButtons[1].CheckMouseClickInRectangle() == true)
    {
        int i, j;
        int newFrame = std::max(0, myPresentation.numberOfPresentation - 1);
        myPresentation.numberOfPresentation = newFrame;
        myPresentation.FindPosition(newFrame, i, j);
        // std::cerr << newFrame << ' ' << i << ' ' << j << '\n';
        myPresentation.currentPresentation = i;
        myPresentation.SetCurToStartAnimation(i + 1, -1);
        myPresentation.SetCurToStartAnimationInOnePresentation(i, j + 1);
        SetCurAnimation(0, i - 1, 1);
        _SetCurAnimation(i, j + 0, 1);
        curRemoteState = 1;
    }
    if (remoteButtons[2].CheckMouseClickInRectangle() == true)
    {
        int i, j;
        int newFrame = std::min(myPresentation.CountNumberOfAnimation(), myPresentation.numberOfPresentation + 1);
        myPresentation.numberOfPresentation = newFrame;
        myPresentation.FindPosition(newFrame, i, j);
        // std::cerr << newFrame << ' ' << i << ' ' << j << '\n';
        myPresentation.currentPresentation = i;
        myPresentation.SetCurToStartAnimation(i + 1, -1);
        myPresentation.SetCurToStartAnimationInOnePresentation(i, j + 1);
        SetCurAnimation(0, i - 1, 1);
        _SetCurAnimation(i, j + 0, 1);
        curRemoteState = 1;
    }
    // std::cerr << myPresentation.numberOfPresentation << '\n';

    myPresentation.NormPresentation(curRemoteState);

    if (changeSpeed.CheckMouseClickInRectangle() == true)
    {
        if (speed == 0.01f)
        {
            changeSpeed.text = "x2";
            speed = 0.02f;
        }
        else if (speed == 0.02f)
        {
            changeSpeed.text = "x4";
            speed = 0.04f;
        } 
        else if (speed == 0.04f)
        {
            changeSpeed.text = "x8";
            speed = 0.08f;
        }
        else if (speed == 0.08f)
        {
            changeSpeed.text = "x1";
            speed = 0.01f;
        }
    }
}

void Trie::DrawRemote()
{
    for (int i = 0; i < 4; ++i)
    {
        if (flagRemote[i] == true) 
        {
            remoteButtons[i].DrawButtonTexture();
        }
    }
    if (myPresentation.numberOfPresentation == myPresentation.CountNumberOfAnimation()) {
        curRemoteState = 2;
    }
    remoteButtons[4 + curRemoteState].DrawButtonTexture();

    const int screenWidth = 1600;
    const int screenHeight = 800;

    // Toạ độ & kích thước thanh slider
    int sliderX = 450;                 // Cách trái xa hơn để né trọn bộ điều khiển
    int sliderY = screenHeight - 30;   // Gần đáy
    int sliderWidth = screenWidth - 450 - 450; // Ngắn bên phải lại (100px)
    int sliderHeight = 8;

    float progress = myPresentation.numberOfPresentation / (float)myPresentation.CountNumberOfAnimation();

    // Vẽ thanh slider
    DrawRectangle(sliderX, sliderY, sliderWidth, sliderHeight, DARKGRAY);
    DrawRectangle(sliderX, sliderY, sliderWidth * progress, sliderHeight, SKYBLUE);

    // Vẽ nút trượt
    int handleRadius = 10;
    int handleX = sliderX + (int)(sliderWidth * progress);
    int handleY = sliderY + sliderHeight / 2;
    DrawCircle(handleX, handleY, handleRadius, SKYBLUE);

    changeSpeed.DrawButtonAndText(0.3, changeSpeed.CheckMouseInRectangle() ? 1 : 0.2f, SKYBLUE, true, robotoFont, 20, RAYWHITE);
}
