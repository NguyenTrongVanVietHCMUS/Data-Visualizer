#include "SinglyLinkedList.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

void SinglyLinkedList::Run() 
{
    HandleToolBar();
    DrawToolBar();

    Draw();

    HandleRemote();
    DrawRemote();
}

void SinglyLinkedList::Init()
{
    toolBarButtons.resize(6);
    toolBarButtons[0] = 
    {
        {Vector2{0, 570 - 50}, 15, 183, (char *)"Open_Close"}
    };
    toolBarButtons[1] = 
    {
        {Vector2{17, 570 - 50}, 160, 35, (char *)"Create"},
        {Vector2{179, 570 - 50}, 80, 35, (char *)"Empty"},
        {Vector2{261, 570 - 50}, 80, 35, (char *)"Random"},
        {Vector2{343, 570 - 50}, 80, 35, (char *)"File"},
        {Vector2{425, 570 - 50}, 80, 35, (char *)"Enter"},
    };
    toolBarButtons[2] = 
    {
        {Vector2{17, 607 - 50}, 160, 35, (char *)"Insert"}
    };
    toolBarButtons[3] = 
    {
        {Vector2{17, 644 - 50}, 160, 35, (char *)"Delete"}
    };
    toolBarButtons[4] = 
    {
        {Vector2{17, 681 - 50}, 160, 35, (char *)"Update"}
    };
    toolBarButtons[5] = 
    {
        {Vector2{17, 718 - 50}, 160, 35, (char *)"Search"}
    };
    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }

    // enterList.Init();
    enterList.oldWidth = 80;
    enterList.oldHeight = 35 ;
    enterList.textBox = {Vector2{425 + 80 + 2, 570 - 50}, 80, 35};
    enterList.confirm = {Vector2{425 + 80 + 2, 570 + 35 + 2 - 50}, 80, 35, (char *)"Confirm"};

    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }
    flagToolBarButtons[0][0] = true ; 
    nodes.clear();
    values.clear();
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
        "Create new SinglyLinkedList!"
    };
    searchList = {
        "if (!pHead || pHead->value == value) return pHead;",
        "Node *cur = pHead;",
        "while (cur != nullptr) {",
        "   if (cur->value == value) return cur;",
        "   cur = cur->pNext;",
        "}",
        "return nullptr;"
    };
    updateList = {
        "Node *cur = pHead;",
        "while (!cur && k > 0) {",
        "   k -= 1;",
        "   cur = cur->pNext;",
        "}",
        "cur->value = value;"
    };
    insertList = {
        "if (pHead == nullptr)",
        "   return new Node(value);",
        "if (k == 0)",
        "   return new Node(value, pHead);",
        "Node *cur = pHead, prev = nullptr;",
        "while (!cur && k > 0) {",
        "   k -= 1;",
        "   prev = cur",
        "   cur = cur->pNext;",
        "}",
        "prev->pNext = new Node(value, cur);",
        "return pHead;"
    };
    deleteList = {
        "if (k == 0)",
        "   Node *nHead = pHead->pNext;",
        "   delete pHead;",
        "   return nHead;",
        "Node *cur = pHead, prev = nullptr;",
        "while (!cur && k > 0)",
        "   k -= 1;",
        "   prev = cur",
        "   cur = cur->pNext;",
        "prev->pNext = cur->pNext",
        "delete cur;",
        "return pHead;"
    };
    positionBoard = Vector2{1600 - 400, 800 - 300};
    width = 400;
    height = 300;
}

std::vector<Node> SinglyLinkedList::BuildNodeFromValue(const std::vector<std::string> &values)
{
    float x = 50.0f, y = 200.0f;
    std::vector<Node> nodes;
    for (const std::string &value: values) 
    {
        nodes.push_back({Vector2{x, y}, 20, value});
        x += 80.0f;
    }
    return nodes;
}

Presentation SinglyLinkedList::CreateAnimation(const std::vector<Node> &nodes)
{
    Presentation create;
    { // Insert all nodes
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {curNode}, 0, {0}));
        }
    }
    { // Insert all edges
        create.CopySetToLast(-1);
        create.SetStartAnimation(-1, -1, 1);
        create.CreateNewSet(-1);
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[i - 1], nodes[i]}, 0, {0}));
        }
    }
    { // Remove animations
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {curNode}, 0, {0}));
        }
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[i - 1], nodes[i]}, 0, {0}));
        }
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {curNode}, 0, {0}));
        }
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[i - 1], nodes[i]}, 0, {0}));
        }
    }
    return create;
}

void SinglyLinkedList::BuildCreateAnimation()
{
    nodes = BuildNodeFromValue(values);
    myPresentation.currentPresentation = 0;
    myPresentation = CreateAnimation(nodes);
    myPresentation.InitBoardText(createList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

int SinglyLinkedList::FindPosition(std::string value)
{
    for (int i = 0; i < int(values.size()); ++i)
    {
        if (values[i] == value)
        {
            return i;
        }
    }
    return -1;
}

void SinglyLinkedList::ClearAllData()
{
    nodes.clear();
    values.clear();
    myPresentation.Clear();
}

void SinglyLinkedList::RandomNewData()
{
    ClearAllData();

    int n = GetRandomValue(1, 20);
    for (int i = 1; i <= n; ++i)
    {
        values.push_back(std::to_string(GetRandomValue(0, 999)));
    }
    BuildCreateAnimation();
}

void SinglyLinkedList::InputDataFromFile()
{
    ClearAllData();

    FilePathList droppedFiles = LoadDroppedFiles();
    char *filePath = new char[2448];

    TextCopy(filePath, droppedFiles.paths[0]);
    std::cout<<filePath<<std::endl; 
    // abort(); 
    UnloadDroppedFiles(droppedFiles);

    std::ifstream fin(filePath);
    std::string s;
    getline(fin, s);

    std::string curValue;
    for (char &c: s)
    {
        if ('0' <= c && c <= '9')
        {
            curValue += c;
        }
        else
        {
            if (!curValue.empty())
            {
                values.push_back(curValue);
                curValue.clear();
            }
        }
    }
    if (!curValue.empty())
    {
        values.push_back(curValue);
    }
    fin.close();
    BuildCreateAnimation();
}

SetOfAnimation SinglyLinkedList::BasicStructure(const std::vector<Node> &nodes)
{
    SetOfAnimation basicStructure;
    if (nodes.empty() == false) { 
        basicStructure.CreateNewSet();
        for (Node curNode: nodes) basicStructure.InsertAnimationToSet(-1, NewAnimation(0, 0, BLACK, {curNode}));
        for (int i = 1; i < int(nodes.size()); ++i) basicStructure.InsertAnimationToSet(-1, NewAnimation(4, 0, BLACK, {nodes[i - 1], nodes[i]}));
    }
    return basicStructure;
}

Presentation SinglyLinkedList::SearchAnimation(int pos, Color color)
{
    Presentation search;
    search.present = {BasicStructure(nodes)};
    search.CopySetToLast(-1);
    search.CreateNewSet(-1);
    search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {0}));
    if (nodes.size() && pos == 0)
    {
        search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, color, {nodes[0]}, 0, {0}));
        return search;
    }
    {
        for (int i = 0; i < int(nodes.size()); ++i)
        {
            search.CopySetToLast(-1);
            search.SetStartAnimation(-1, 1);
            if (i > 0)
            {
                search.EraseAnimation(-1, NewAnimation(2, 0, ORANGE, {nodes[i - 1]}, 0));
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));
                search.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes[i - 1]}, 0, {2}));
                search.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes[i - 1]}, 0, {2}));
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {3}));
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[i - 1], nodes[i]}, 0, {4}));
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {4}));
            }
            else 
            {
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {1}));
            }
            if (i == pos) 
            {
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {3}));
                return search;
            }
        }
    }
    if (nodes.empty() == false)
    {
        search.CopySetToLast(-1);
        search.SetStartAnimation(-1, 1);
        search.EraseAnimation(-1, -1, NewAnimation(2, 0, ORANGE, {nodes.back()}, 0, {}));

        search.CreateNewSet(-1);
        search.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes.back()}));
        search.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes.back()}));
        search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));

        search.CreateNewSet(-1);
        search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {6}));
        
    }
    return search;
}

void SinglyLinkedList::Search(std::string val)
{
    nodes = BuildNodeFromValue(values);
    int pos = FindPosition(val);
    myPresentation = SearchAnimation(pos, BLUE);
    myPresentation.InitBoardText(searchList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

Presentation SinglyLinkedList::UpdateAnimation(int pos, std::string val)
{
    Presentation update;
    Color color = BLUE;

    update.present = {BasicStructure(nodes)};
    update.CopySetToLast(-1);
    {
        for (int i = 0; i < int(nodes.size()); ++i)
        {
            update.CopySetToLast(-1);
            update.SetStartAnimation(-1, 1);
            if (i > 0)
            {
                update.EraseAnimation(-1, NewAnimation(2, 0, ORANGE, {nodes[i - 1]}));
                update.CreateNewSet(-1);
                update.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
                update.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes[i - 1]}, 0, {1}));
                update.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes[i - 1]}, 0, {1}));
                update.CreateNewSet(-1);
                update.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));
                update.CreateNewSet(-1);
                update.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[i - 1], nodes[i]}, 0, {3}));
                update.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {3}));
            }
            else 
            {
                update.CreateNewSet(-1);
                update.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {0}));
                update.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {0}));
            }
            if (i == pos) 
            {
                update.CreateNewSet(-1);
                update.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
                break;
            }
        }
    }

    update.CopySetToLast(-1);
    update.SetStartAnimation(-1, 1);
    update.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[pos]}));
    
    NewAnimation animation = NewAnimation(2, 0, color, {nodes[pos]});
    update.EraseAnimation(-1, animation);

    update.CreateNewSet(-1);
    animation.type = 3;
    animation.color = BLUE;
    animation.curAnimation = 0;
    animation.listHighlights = {5};
    update.InsertAnimationToSet(-1, -1, animation);

    animation.type = 2;
    animation.color = GREEN;
    animation.curAnimation = 0;
    animation.nodes[0].value = val;
    update.InsertAnimationToSet(-1, -1, animation);

    return update;
}

void SinglyLinkedList::Update(int pos, std::string val)
{
    nodes = BuildNodeFromValue(values);
    myPresentation.currentPresentation = 0;
    if (pos < 0 || pos >= int(nodes.size())) 
    {
        myPresentation.present = {BasicStructure(nodes)};
        myPresentation.CopySetToLast(-1);
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {0}));
        std::vector<std::string> outSideRange = {
            "The position is out side of range!!"  
        };
        myPresentation.InitBoardText(outSideRange, positionBoard, width, height);
        myPresentation.numberOfPresentation = 0;
        myPresentation.currentPresentation = 0;
        curRemoteState = 0;
        return;   
    }
    myPresentation = UpdateAnimation(pos, val);
    myPresentation.InitBoardText(updateList, positionBoard, width, height);
    values[pos] = val;
    nodes[pos].value = val;
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

Presentation SinglyLinkedList::InsertAnimation(int pos, std::string val)
{
    Presentation insert;
    Color color = BLUE;

    insert.present = {BasicStructure(nodes)};
    insert.CopySetToLast(-1);

    insert.CreateNewSet(-1);
    insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {0}));

    if (nodes.empty() == true)
    {
        insert.CreateNewSet(-1);
        insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
        insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {Node(Vector2{100.0f, 180.0f}, 20, val)}, 0, {1}));
        return insert;
    } 

    insert.CreateNewSet(-1);
    insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));

    if (pos == 0)
    {
        Node newNode(Vector2{nodes[0].position.x, nodes[0].position.y + 80.f}, 20, val);
        insert.CopySetToLast(-1);
        insert.CreateNewSet(-1);
        insert.SetStartAnimation(-1, 1);
        insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {newNode}, 0, {3}));
        insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {newNode, nodes[0]}, 0, {3}));
        
        insert.CreateNewPresentation();
        insert.CreateNewSet(-1);
        std::vector<Node> tmpNodes = nodes;
        tmpNodes.push_back(Node(Vector2{nodes.back().position.x + 80.f, nodes.back().position.y}, 20, val));

        insert.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, GREEN, {newNode, nodes[0]}, 0, {3}));
        for (int i = 0; i < int(tmpNodes.size()) - 1; ++i)
        {
            insert.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {tmpNodes[i], tmpNodes[i + 1]}, 0, {3}));
        }
        insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, ORANGE, {newNode, tmpNodes[0], tmpNodes[0], tmpNodes[1]}, 0, {3}));
        for (int i = 0; i < int(tmpNodes.size()) - 2; ++i)
        {
            insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, BLACK, {tmpNodes[i], tmpNodes[i + 1], tmpNodes[i + 1], tmpNodes[i + 2]}, 0, {3}));
        }

        return insert;
    }

    for (int i = 0; i < int(nodes.size()); ++i)
    {
        insert.CopySetToLast(-1);
        insert.SetStartAnimation(-1, 1);
        if (i > 0)
        {
            insert.EraseAnimation(-1, NewAnimation(2, 0, ORANGE, {nodes[i - 1]}));
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {5}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes[i - 1]}, 0, {5}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes[i - 1]}, 0, {5}));
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {6}));
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {7}));
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[i - 1], nodes[i]}, 0, {8}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {8}));
        }
        else 
        {
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {4}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {4}));
        }
        if (i == pos) 
        {
            insert.CopySetToLast(-1);
            insert.CreateNewSet(-1);
            insert.SetStartAnimation(-1, 1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {5}));

            Node newNode(Vector2{nodes[pos].position.x, nodes[pos].position.y + 80.f}, 20, val);
            insert.CreateNewSet(-1);
            insert.SetStartAnimation(-1, 1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {newNode}, 0, {10}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[pos - 1], newNode}, 0, {10}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {newNode, nodes[pos]}, 0, {10}));
            insert.EraseAnimation(-1, NewAnimation(4, 0, BLACK, {nodes[pos - 1], nodes[pos]}, 0, {10}));
            insert.EraseAnimation(-1, NewAnimation(5, 0, ORANGE, {nodes[pos - 1], nodes[pos]}, 0, {10}));

            insert.CreateNewPresentation();
            insert.CreateNewSet(-1);
            for (int i = 0; i < pos; ++i)
            {
                insert.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {nodes[i]}, 0, {10}));
                if (i + 1 < pos)
                {
                    insert.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[i], nodes[i + 1]}, 0, {10}));
                }
            }
            nodes.push_back(Node(Vector2{nodes.back().position.x + 80.f, nodes.back().position.y}, 20, val));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], newNode, nodes[pos]}, 0, {10}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, ORANGE, {newNode, nodes[pos], nodes[pos], nodes[pos + 1]}, 0, {10}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, ORANGE, {newNode, nodes[pos]}, 0, {10}));

            for (int i = pos; i < int(nodes.size()) - 1; ++i)
            {
                insert.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[i], nodes[i + 1]}, 0, {10}));
                if (i + 1 < int(nodes.size()) - 1)
                {
                    insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, BLACK, {nodes[i], nodes[i + 1], nodes[i + 1], nodes[i + 2]}, 0, {10}));
                }
            }  

            nodes.pop_back();
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {11}));

            return insert;
        }
    }

    if (pos == int(nodes.size()))
    { 
        insert.CopySetToLast(-1);
        insert.SetStartAnimation(-1, 1);
        insert.EraseAnimation(-1, NewAnimation(2, 0, ORANGE, {nodes.back()}));
        insert.CreateNewSet(-1);
        insert.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes.back()}, 0, {5}));
        insert.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes.back()}, 0, {5}));

        insert.CreateNewSet(-1);
        insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {Node(Vector2{nodes.back().position.x + 80.0f, nodes.back().position.y}, 20, val)}, 0, {10}));
        insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes.back(), Node(Vector2{nodes.back().position.x + 80.0f, nodes.back().position.y}, 20, val)}, 0, {10}));

        insert.CreateNewSet(-1);
        insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {11}));        
    } 

    return insert;
}

void SinglyLinkedList::Insert(int pos, std::string val)
{
    nodes = BuildNodeFromValue(values);
    pos = std::min(pos, int(nodes.size()));
    myPresentation = InsertAnimation(pos, val);
    myPresentation.InitBoardText(insertList, positionBoard, width, height);
    values.insert(values.begin() + pos, val);
    nodes = BuildNodeFromValue(values);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

Presentation SinglyLinkedList::DeleteAnimation(int pos)
{
    Presentation _delete;
    Color color = RED;

    _delete.present = {BasicStructure(nodes)};
    _delete.CopySetToLast(-1);

    _delete.CreateNewSet(-1);
    _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {0}));

    if (pos == 0)
    {
        _delete.CreateNewSet(-1);
        _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));

        _delete.CreateNewPresentation();
        _delete.CreateNewSet(-1);
        _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));
        _delete.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, RED, {nodes[0]}, 0, {2}));
        if (nodes.size() > 1) 
        {
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, RED, {nodes[0], nodes[1]}, 0, {2}));
        }
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {nodes[i]}, 0, {2}));
            if (i + 1 < int(nodes.size()))
            {
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[i], nodes[i + 1]}, 0, {2}));
            }
        }

        _delete.CreateNewPresentation();
        _delete.CreateNewSet(-1);
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[i], nodes[i - 1]}, 0, {3}));
            if (i + 1 < int(nodes.size()))
            {
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, BLACK, {nodes[i], nodes[i - 1], nodes[i + 1], nodes[i]}, 0, {3}));
            }
        }

        return _delete;
    }

    {
        for (int i = 0; i < int(nodes.size()); ++i)
        {
            _delete.CopySetToLast(-1);
            _delete.SetStartAnimation(-1, 1);
            if (i > 0)
            {
                _delete.EraseAnimation(-1, NewAnimation(2, 0, ORANGE, {nodes[i - 1]}));
                _delete.CreateNewSet(-1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {5}));
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes[i - 1]}, 0, {5}));
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes[i - 1]}, 0, {5}));
                _delete.CreateNewSet(-1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {6}));
                _delete.CreateNewSet(-1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {7}));
                _delete.CreateNewSet(-1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, (i == pos ? color : ORANGE), {nodes[i - 1], nodes[i]}, 0, {8}));
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {8}));
            }
            else 
            {
                _delete.CreateNewSet(-1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {4}));
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}, 0, {4}));
            }
            if (i == pos) 
            {
                _delete.CreateNewSet(-1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {5}));

                if (i == int(nodes.size() - 1))
                {
                    _delete.CreateNewSet(-1);
                    _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {9}));
                    
                    _delete.CopySetToLast(-1);
                    _delete.SetStartAnimation(-1, 1);
                    _delete.EraseAnimation(-1, NewAnimation(0, 0, BLACK, {nodes[pos]}, 0, {10}));
                    _delete.EraseAnimation(-1, NewAnimation(2, 0, RED, {nodes[pos]}, 0, {10}));
                    if (pos > 0)
                    {
                        _delete.EraseAnimation(-1, NewAnimation(4, 0, BLACK, {nodes[pos - 1], nodes[pos]}, 0, {10}));
                        _delete.EraseAnimation(-1, NewAnimation(5, 0, RED, {nodes[pos - 1], nodes[pos]}, 0, {10}));
                    }
                    _delete.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, RED, {nodes[pos]}, 0, {10}));
                    if (pos > 0)
                    {
                        _delete.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, RED, {nodes[pos - 1], nodes[pos]}, 0, {10}));
                    }
                }
                else
                {
                    _delete.CreateNewPresentation();
                    _delete.CreateNewSet(-1);
                    for (int i = 0; i < int(nodes.size()); ++i)
                    {
                        if (i != pos) 
                        {
                            _delete.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {nodes[i]}, 0, {9}));
                        }
                        if (i != pos && i + 1 != pos && i + 1 < int(nodes.size()))
                        {
                            _delete.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[i], nodes[i + 1]}, 0, {9}));
                        }
                    }
                    Node newNode = Node(Vector2{nodes[i].position.x, nodes[i].position.y + 80.0f}, 20, nodes[i].value);
                    _delete.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[i - 1], nodes[i + 1]}, 0, {9}));
                    _delete.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, RED, {nodes[i], newNode}, 0, {9}));
                    _delete.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, RED, {nodes[i], newNode, nodes[i + 1], nodes[i + 1]}, 0, {9}));

                    _delete.CopySetToLast(-1);
                    _delete.SetStartAnimation(-1, 1);
                    _delete.EraseAnimation(-1, NewAnimation(8, 0, RED, {nodes[i], newNode}));
                    _delete.EraseAnimation(-1, NewAnimation(7, 0, RED, {nodes[i], newNode, nodes[i + 1], nodes[i + 1]}));
                    _delete.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, RED, {newNode}, 0, {10}));
                    _delete.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, RED, {newNode, nodes[i + 1]}, 0, {10}));
                }   
                _delete.CreateNewSet(-1);
                _delete.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {11}));
                return _delete;
            }
        }
    }

    return _delete;
}

void SinglyLinkedList::Delete(std::string _pos)
{
    nodes = BuildNodeFromValue(values);
    int pos = StringToVector(_pos)[0];
    pos = std::min(pos, int(nodes.size()) - 1);
    std::cerr << _pos << ' ' << pos << '\n';
    if (pos < 0) 
    {
        myPresentation.present = {BasicStructure(nodes)};
        myPresentation.CopySetToLast(-1);
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {0}));
        std::vector<std::string> outSideRange = {
            "The position is out side of range!!"  
        };
        myPresentation.InitBoardText(outSideRange, positionBoard, width, height);
        myPresentation.numberOfPresentation = 0;
        myPresentation.currentPresentation = 0;
        curRemoteState = 0;
        return;   
    }
    myPresentation = DeleteAnimation(pos);
    myPresentation.currentPresentation = 0;
    myPresentation.InitBoardText(deleteList, positionBoard, width, height);
    if (pos != -1)
    {
        values.erase(values.begin() + pos);
        nodes.erase(nodes.begin() + pos);
        nodes = BuildNodeFromValue(values);
    }
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

void SinglyLinkedList::DrawToolBar()
{
    toolBarButtons[0][0].DrawButton(0.3, 0.1, LIME, true); 
    DrawTextureEx(flagToolBarButtons[0][0] == false ? toolBarRightArrow : toolBarLeftArrow, Vector2{2.5, 570 - 50 + 183.0 / 2 - 16.0 / 2}, 0, 1, WHITE);
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
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 - 50}, 40, 35, (char *)"v ="};
            Button i = {Vector2{17 + 160 + 2, 607 - 50}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[3][0] == true) // Delete
        {
            Button i = {Vector2{17 + 160 + 2, 607 + 35 + 2 - 50}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
        }
        if (flagToolBarButtons[4][0] == true) // Update
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 40, 35, (char *)"v ="};
            Button i = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 - 50}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[5][0] == true) // Search
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 40, 35, (char *)"v ="};
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

void SinglyLinkedList::HandleToolBar()
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
                    if (j != 0 && flagToolBarButtons[i][0] == false) continue;
                    for (int i = 1; i < 6; ++i) 
                    {
                        std::fill(flagToolBarButtons[i].begin() + (j > 0), flagToolBarButtons[i].end(), false);
                    }
                    flagToolBarButtons[i][j] = true;
                    if (i == 2 && j == 0)
                    {
                        insertI.oldWidth = 60;
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607 - 50}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 - 50}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 3 && j == 0)
                    {
                        insertI.oldWidth = 60;
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 - 50}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 4 && j == 0)
                    {
                        insertI.oldWidth = 60;
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 50}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 5 && j == 0)
                    {
                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};
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
                values.clear() ; 
                
                std::vector<int> tmp =  StringToVector(listChar);
                for(auto&x : tmp)
                {
                    values.push_back(std::to_string(x));
                }
                nodes = BuildNodeFromValue(values);
                BuildCreateAnimation();
            }
            return;
        }
        if (flagToolBarButtons[2][0] == true) // Insert
        {
            std::string i = insertI.HandleTextBox();
            std::string v = insertV.HandleTextBox();
            if (i.empty() == true) 
            {
                return;
            }
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[2][0] = false;
            int iValue = StringToVector(i)[0];
            std::string vValue = std::to_string(StringToVector(v)[0]);
            Insert(iValue, vValue);
    
            return;
        }
        if (flagToolBarButtons[3][0] == true) // Delete
        {
            std::string i = insertI.HandleTextBox();
            if (i.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[3][0] = false;
            std::string iValue = std::to_string(StringToVector(i)[0]);

            Delete(iValue);
    
            return;
        }
        if (flagToolBarButtons[4][0] == true) // Update
        {
            std::string i = insertI.HandleTextBox();
            std::string v = insertV.HandleTextBox();
            if (i.empty() == true) 
            {
                return;
            }
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[4][0] = false;
            int iValue = StringToVector(i)[0];
            std::string vValue = std::to_string(StringToVector(v)[0]);
            Update(iValue, vValue);
    
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
            std::string vValue = std::to_string(StringToVector(v)[0]);
            Search(vValue);
    
            return;
        }
    }
}


void SinglyLinkedList::Draw() 
{  
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, fontNumber, robotoFont, isLightMode, speed, curRemoteState);
}

void SinglyLinkedList::HandleRemote()
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

void SinglyLinkedList::DrawRemote()
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

std::vector<int> SinglyLinkedList::StringToVector(std::string listChar)
{
    std::vector<int> values;
    int cur = -1;
    for (int i = 0; i < int(listChar.size()); ++i) 
    {
        if (listChar[i] == ' ' || listChar[i] == ',')
        {
            if (cur > -1) values.push_back(cur);
            cur = -1;
            continue;
        }
        cur = cur == -1 ? (listChar[i] - '0') : 10 * cur + (listChar[i] - '0');
    }
    if (cur > -1)
    {
        values.push_back(cur);
    }
    return values;
}
                     