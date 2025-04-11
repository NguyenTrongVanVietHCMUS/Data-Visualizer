#include "Heap.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

void Heap::Run() 
{
    HandleToolBar();
    DrawToolBar();
    Draw();
}

void Heap::Init()
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
        {Vector2{17, 607}, 160, 35, (char *)"Push"}
    };
    toolBarButtons[3] = 
    {
        {Vector2{17, 644}, 160, 35, (char *)"Pop"}
    };
    toolBarButtons[4] = 
    {
        {Vector2{17, 681}, 160, 35, (char *)"Top"}
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

    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }

    nodes.clear();
    values.clear();
    isLightMode = 1;

    speed = 0.01;

    size = 0;
    itemID = new int[capacity];
}

std::vector<Node> Heap::BuildNodeFromValue(const std::vector<std::string> &values)
{
    CalcPosition(nodes);
    return nodes;
}

void Heap::HeapifyDown(Presentation &myPresentation)
{
    myPresentation.CopySetToLast(-1);
    myPresentation.SetStartAnimation(-1, 1);
    if (size > 0)
    {
        myPresentation.EraseAnimation(-1, -1, myPresentation.GetAnimation(-1, -1, -1));
        myPresentation.InsertAnimationToSet(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[0]]}));
    }
    int curID = 0;
    Node from, to;
    while (2 * curID + 1 < size) 
    {
        int nextID = 2 * curID + 1;
        if (nextID + 1 < size)
        {
            if (StringToVector(values[itemID[nextID]])[0] > StringToVector(values[itemID[nextID + 1]])[0])
            {
                ++nextID;
            }
        }
        if (StringToVector(values[itemID[curID]])[0] < StringToVector(values[itemID[nextID]])[0]) 
        {
            break;
        }

        myPresentation.CopySetToLast(-1);
        myPresentation.SetStartAnimation(-1, 1);
        myPresentation.EraseAnimation(-1, NewAnimation(0, 0, BLACK, {nodes[itemID[curID]]}));
        myPresentation.EraseAnimation(-1, NewAnimation(0, 0, BLACK, {nodes[itemID[nextID]]}));
        if (curID != size - 1)
        {
            myPresentation.EraseAnimation(-1, -1, NewAnimation(8, 0, BLACK, {from, to}));
        }
        myPresentation.CreateNewSet(-1);
        myPresentation.SetStartAnimation(-1, 1);
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[curID]], nodes[itemID[nextID]]}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[nextID]], nodes[itemID[curID]]}));

        from = nodes[itemID[curID]];
        to = nodes[itemID[nextID]];
        std::swap(itemID[curID], itemID[nextID]);
        CalcPosition(nodes);
        curID = nextID;
    }
}

void Heap::HeapifyUp(Presentation &myPresentation)
{
    myPresentation.CopySetToLast(-1);
    myPresentation.SetStartAnimation(-1, 1);
    int curID = size - 1;
    Node from, to;
    while (curID > 0 && (curID - 1) / 2 >= 0) 
    {
        int nextID = (curID - 1) / 2;
        if (StringToVector(values[itemID[curID]])[0] >= StringToVector(values[itemID[nextID]])[0]) 
        {
            break;
        }

        myPresentation.CopySetToLast(-1);
        myPresentation.SetStartAnimation(-1, 1);
        myPresentation.EraseAnimation(-1, NewAnimation(0, 0, BLACK, {nodes[itemID[curID]]}));
        myPresentation.EraseAnimation(-1, NewAnimation(0, 0, BLACK, {nodes[itemID[nextID]]}));
        if (curID != size - 1)
        {
            myPresentation.EraseAnimation(-1, -1, NewAnimation(8, 0, BLACK, {from, to}));
        }
        myPresentation.CreateNewSet(-1);
        myPresentation.SetStartAnimation(-1, 1);
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[curID]], nodes[itemID[nextID]]}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[nextID]], nodes[itemID[curID]]}));

        from = nodes[itemID[curID]];
        to = nodes[itemID[nextID]];
        std::swap(itemID[curID], itemID[nextID]);
        CalcPosition(nodes);
        curID = nextID;
    }
}

Presentation Heap::CreateAnimation(const std::vector<Node> &nodes)
{
    Presentation create;
    { // Insert all nodes
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {curNode}));
        }
    }
    { // Insert all edges
        create.CopySetToLast(-1);
        create.SetStartAnimation(-1, 1);
        create.CreateNewSet(-1);
        for (int i = 0; i < size; ++i)
        {
            if (2 * i + 1 < size)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 1]]}));
                if (2 * i + 2 < size)
                {
                    create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 2]]}));
                }
            }
        }
    }
    { // Remove animations
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {curNode}));
        }
        for (int i = 0; i < size; ++i)
        {
            if (2 * i + 1 < size)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[itemID[i]], nodes[itemID[2 * i + 1]]}));
                if (2 * i + 2 < size)
                {
                    create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[itemID[i]], nodes[itemID[2 * i + 2]]}));
                }
            }
        }
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {curNode}));
        }
        for (int i = 0; i < size; ++i)
        {
            if (2 * i + 1 < size)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 1]]}));
                if (2 * i + 2 < size)
                {
                    create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 2]]}));
                }
            }
        }
    }
    return create;
}

void Heap::BuildCreateAnimation()
{
    nodes = BuildNodeFromValue(values);
    myPresentation.currentPresentation = 0;
    myPresentation = CreateAnimation(nodes);
}

int Heap::FindPosition(std::string value)
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

void Heap::ClearAllData()
{
    if (size > 0)
    {
        size = 0;
        nodes.clear();
        values.clear();
        myPresentation.Clear();
        myPresentation.currentPresentation = 0;
    }
}

void Heap::RandomNewData()
{
    ClearAllData();

    int n = GetRandomValue(1, 31);
    for (int i = 1; i <= n; ++i)
    {
        Push(std::to_string(GetRandomValue(1, 999)));
    }

    BuildCreateAnimation();
}

void Heap::InputDataFromFile()
{
    ClearAllData();
    auto HeapifyUp = [&]() -> void 
    {
        int curID = size - 1;
        while (curID > 0 && (curID - 1) / 2 >= 0) 
        {
            int nextID = (curID - 1) / 2;
            if (StringToVector(values[itemID[curID]])[0] > StringToVector(values[itemID[nextID]])[0]) 
            {
                break;
            }
            std::swap(itemID[curID], itemID[nextID]);
            curID = nextID;
        }
    };
    auto AddValueToHeap = [&](std::string value) -> void 
    {
        nodes.push_back(Node(Vector2{0, 0}, 20, value)); 
        values.push_back(value);
        itemID[size] = size;
        size++;
        HeapifyUp();
        CalcPosition(nodes);
    };

    FilePathList droppedFiles = LoadDroppedFiles();
    char *filePath = new char[2448];

    TextCopy(filePath, droppedFiles.paths[0]);
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
                AddValueToHeap(curValue);
                curValue.clear();
            }
        }
    }
    if (!curValue.empty())
    {
        AddValueToHeap(curValue);
    }
    fin.close();
    BuildCreateAnimation();
}

SetOfAnimation Heap::BasicStructure(const std::vector<Node> &nodes)
{
    SetOfAnimation basicStructure;
    if (nodes.empty() == false) { 
        basicStructure.CreateNewSet();
        for (Node curNode: nodes) 
        {
            basicStructure.InsertAnimationToSet(-1, NewAnimation(0, 0, BLACK, {curNode}));
        }
        for (int i = 0; i < size; ++i)
        {
            if (2 * i + 1 < size)
            {
                basicStructure.InsertAnimationToSet(-1, NewAnimation(4, 0, BLACK, {nodes[itemID[i]], nodes[itemID[2 * i + 1]]}));
                if (2 * i + 2 < size)
                {
                    basicStructure.InsertAnimationToSet(-1, NewAnimation(4, 0, BLACK, {nodes[itemID[i]], nodes[itemID[2 * i + 2]]}));
                }
            }
        }
    }
    return basicStructure;
}

Presentation Heap::SearchAnimation(int pos, Color color)
{
    Presentation search;
    search.present = {BasicStructure(nodes)};
    {
        for (int i = 0; i < int(nodes.size()); ++i)
        {
            search.CopySetToLast(-1);
            search.SetStartAnimation(-1, 1);
            if (i > 0)
            {
                search.EraseAnimation(-1, -1, NewAnimation(2, 0, (i - 1 == pos ? color : ORANGE), {nodes[i - 1]}));
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes[i - 1]}));
                search.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes[i - 1]}));
                search.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[i - 1], nodes[i]}));
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}));
            }
            else 
            {
                search.CreateNewSet(-1);
                search.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}));
            }
            if (i == pos) 
            {
                return search;
            }
        }
    }
    if (nodes.empty() == false)
    {
        search.CopySetToLast(-1);
        search.SetStartAnimation(-1, 1);
        search.EraseAnimation(-1, -1, NewAnimation(2, 0, ORANGE, {nodes.back()}));

        search.CreateNewSet(-1);
        search.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, ORANGE, {nodes.back()}));
        search.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, ORANGE, {nodes.back()}));
    }
    return search;
}

void Heap::Search(std::string val)
{
    nodes = BuildNodeFromValue(values);
    int pos = FindPosition(val);
    myPresentation = SearchAnimation(pos, BLUE);
}

void Heap::Top()
{
    if (size > 0)
    {
        myPresentation.currentPresentation = 0;
        myPresentation.present = {BasicStructure(nodes)};
        myPresentation.CopySetToLast(-1);
        myPresentation.CreateNewSet(-1);
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, BLUE, {nodes[itemID[0]]}));
    }
}

void Heap::CalcPosition(std::vector<Node> &nodes, int curID, int height, float x, float y)
{
    if (curID >= size) return;

    nodes[itemID[curID]].position = Vector2{x, y};

    float xOffset = float(1600) / (1 << (height + 2));
    float yOffset = 80;

    CalcPosition(nodes, 2 * curID + 1, height + 1, x - xOffset, y + yOffset);
    CalcPosition(nodes, 2 * curID + 2, height + 1, x + xOffset, y + yOffset);
}

void Heap::Push(std::string val)
{
    myPresentation.currentPresentation = 0;
    myPresentation.present = {BasicStructure(nodes)};
    auto AddValueToHeap = [&](std::string value) -> void 
    {
        nodes.push_back(Node(Vector2{0, 0}, 20, value)); 
        values.push_back(value);
        itemID[size] = size;
        size++;
        
        CalcPosition(nodes);
        int curID = size - 1;
        myPresentation.CopySetToLast(-1);
        myPresentation.CreateNewSet(-1);
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {nodes[curID]}));
        if (curID > 0 && (curID - 1) / 2 >= 0)
        {
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}));
        }
        myPresentation.CopySetToLast(-1);
        myPresentation.SetStartAnimation(-1, 1);
        myPresentation.EraseAnimation(-1, -1, NewAnimation(2, 0, GREEN, {nodes[curID]}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {nodes[curID]}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {nodes[curID]}));
        if (curID > 0 && (curID - 1) / 2 >= 0)
        {
            myPresentation.EraseAnimation(-1, -1, NewAnimation(4, 0, GREEN, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}));
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}));
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}));
        }
        HeapifyUp(myPresentation);
    }; 
    AddValueToHeap(val);
}

void Heap::Pop()
{
    myPresentation.currentPresentation = 0;
    myPresentation.present = {BasicStructure(nodes)};
    auto PopTop = [&]() -> void 
    {
        myPresentation.CopySetToLast(-1);
        myPresentation.CreateNewSet(-1);
        myPresentation.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[0]]}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, RED, {nodes[itemID[0]]}));

        myPresentation.CopySetToLast(-1);
        myPresentation.SetStartAnimation(-1, 1);
        if (size > 1)
        {
            myPresentation.EraseAnimation(-1, 0, NewAnimation(4, 0, BLACK, {nodes[itemID[(size - 2) / 2]], nodes[itemID[size - 1]]}));
            myPresentation.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[size - 1]]}));
            myPresentation.CreateNewSet(-1);
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, RED, {nodes[itemID[(size - 2) / 2]], nodes[itemID[size - 1]]}));
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[size - 1]], nodes[itemID[0]]}));
        } 

        values.erase(values.begin() + itemID[0]);
        nodes.erase(nodes.begin() + itemID[0]);
        for (int i = 1; i < size; ++i) 
        {
            if (itemID[i] > itemID[0]) 
            {
                --itemID[i];
            }
        }
        itemID[0] = itemID[size - 1];
        size--;
        
        CalcPosition(nodes);
        HeapifyDown(myPresentation);
    }; 
    PopTop();
}

void Heap::DrawToolBar()
{
    toolBarButtons[0][0].DrawButton(0.3, 0.1, LIME, true); 
    DrawTextureEx(flagToolBarButtons[0][0] == false ? toolBarRightArrow : toolBarLeftArrow, Vector2{2.5, 570 + (183.0 - 35 -2) / 2 - 16.0 / 2}, 0, 1, WHITE);
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
        if (flagToolBarButtons[2][0] == true) // Push
        {
            Button v = {Vector2{17 + 160 + 2, 607}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[3][0] == true) // Delete
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }

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

void Heap::HandleToolBar()
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
        for (int i = 1; i < 5; ++i) 
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
                    }
                    if (i == 3 && j == 0)
                    {
                        // insertV.oldWidth = 60;
                        // insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35};
                        // insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
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
                ClearAllData();
                auto HeapifyUp = [&]() -> void 
                {
                    int curID = size - 1;
                    while (curID > 0 && (curID - 1) / 2 >= 0) 
                    {
                        int nextID = (curID - 1) / 2;
                        if (StringToVector(values[itemID[curID]])[0] > StringToVector(values[itemID[nextID]])[0]) 
                        {
                            break;
                        }
                        std::swap(itemID[curID], itemID[nextID]);
                        curID = nextID;
                    }
                };
                auto AddValueToHeap = [&](std::string value) -> void 
                {
                    nodes.push_back(Node(Vector2{0, 0}, 20, value)); 
                    values.push_back(value);
                    itemID[size] = size;
                    size++;
                    HeapifyUp();
                    CalcPosition(nodes);
                }; 
                
                std::vector<int> tmp =  StringToVector(listChar);
                for(auto&x : tmp)
                {
                    AddValueToHeap(std::to_string(x));
                }
                BuildCreateAnimation();
            }
            return;
        }
        if (flagToolBarButtons[2][0] == true) // Push
        {
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[2][0] = false;
            std::string vValue = std::to_string(StringToVector(v)[0]);
            Push(vValue);
    
            return;
        }
        if (flagToolBarButtons[3][0] == true) // Pop
        {
            flagToolBarButtons[3][0] = false;
            if (size > 0)
            {
                Pop();
            }
            return;
        }
        if (flagToolBarButtons[4][0] == true) // Top
        {
            flagToolBarButtons[4][0] = false;
            Top();
            return;
        }
    }
}


void Heap::Draw() 
{  
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, fontNumber, GetFontDefault(), isLightMode, speed, curRemoteState);
}

std::vector<int> Heap::StringToVector(std::string listChar)
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
                     