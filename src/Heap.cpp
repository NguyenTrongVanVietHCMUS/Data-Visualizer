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

    HandleRemote();
    DrawRemote();
}

void Heap::Init()
{
    toolBarButtons.resize(6);
    toolBarButtons[0] = 
    {
        {Vector2{0, 570 - 80}, 15, 183, (char *)"Open_Close"}
    };
    toolBarButtons[1] = 
    {
        {Vector2{17, 570 - 80}, 160, 35, (char *)"Create"},
        {Vector2{179, 570 - 80}, 80, 35, (char *)"Empty"},
        {Vector2{261, 570 - 80}, 80, 35, (char *)"Random"},
        {Vector2{343, 570 - 80}, 80, 35, (char *)"File"},
        {Vector2{425, 570 - 80}, 80, 35, (char *)"Enter"},
    };
    toolBarButtons[2] = 
    {
        {Vector2{17, 607 - 80}, 160, 35, (char *)"Push"}
    };
    toolBarButtons[3] = 
    {
        {Vector2{17, 644 - 80}, 160, 35, (char *)"Pop"}
    };
    toolBarButtons[4] = 
    {
        {Vector2{17, 681 - 80}, 160, 35, (char *)"Top"}
    };
    toolBarButtons[5] = 
    {
        {Vector2{17, 718 - 80}, 160, 35, (char *)"Update"}
    };
    flagToolBarButtons.resize(7);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }

    // enterList.Init();
    enterList.oldWidth = 80;
    enterList.oldHeight = 35 ; 
    enterList.textBox = {Vector2{425, 570-80+35+2}, 80, 35};
    enterList.confirm = {Vector2{425, 570 + 35 + 2}, 80, 35, (char *)"Confirm"};
    // enterList.textBox = {Vector2{425 + 80 + 2, 570}, 80, 35};
    // enterList.confirm = {Vector2{425 + 80 + 2, 570 + 35 + 2-50}, 80, 35, (char *)"Confirm"};
    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }

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
        "Create new MinHeap!"
    };
    topList = {
        "return Heap[0]"
    };
    pushList = {
        "Heap[size++] = value",
        "HeapifyUp()"
    };
    popList = {
        "Heap[0] = Heap[--size]",
        "HeapifyDown()"
    };
    updateList = {
        "Heap[pos] = newv",
        "shiftup(i); // if newv < oldv",
        "shiftdown(i); // if newv > oldv"
    };
    positionBoard = Vector2{1600 - 400, 800 - 300};
    width = 400;
    height = 300;

    size = 0;
    itemID = new int[capacity];
}

std::vector<Node> Heap::BuildNodeFromValue(const std::vector<std::string> &values)
{
    CalcPosition(nodes);
    return nodes;
}

void Heap::HeapifyDown(Presentation &myPresentation, std::vector<int> highlightLines)
{
    myPresentation.CopySetToLast(-1);
    myPresentation.SetStartAnimation(-1, 1);
    if (size > 0)
    {
        myPresentation.EraseAnimation(-1, -1, myPresentation.GetAnimation(-1, -1, -1));
        myPresentation.InsertAnimationToSet(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[0]]}, 0, highlightLines));
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
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[curID]], nodes[itemID[nextID]]}, 0, highlightLines));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[nextID]], nodes[itemID[curID]]}, 0, highlightLines));

        from = nodes[itemID[curID]];
        to = nodes[itemID[nextID]];
        std::swap(itemID[curID], itemID[nextID]);
        CalcPosition(nodes);
        curID = nextID;
    }
}

void Heap::HeapifyUp(Presentation &myPresentation, std::vector<int> highlightLines)
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
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[curID]], nodes[itemID[nextID]]}, 0, highlightLines));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[nextID]], nodes[itemID[curID]]}, 0, highlightLines));

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
            create.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {curNode}, 0, {0}));
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
                create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 1]]}, 0, {0}));
                if (2 * i + 2 < size)
                {
                    create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 2]]}, 0, {0}));
                }
            }
        }
    }
    { // Remove animations
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {curNode}, 0, {0}));
        }
        for (int i = 0; i < size; ++i)
        {
            if (2 * i + 1 < size)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[itemID[i]], nodes[itemID[2 * i + 1]]}, 0, {0}));
                if (2 * i + 2 < size)
                {
                    create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[itemID[i]], nodes[itemID[2 * i + 2]]}, 0, {0}));
                }
            }
        }
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {curNode}, 0, {0}));
        }
        for (int i = 0; i < size; ++i)
        {
            if (2 * i + 1 < size)
            {
                create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 1]]}, 0, {0}));
                if (2 * i + 2 < size)
                {
                    create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[itemID[i]], nodes[itemID[2 * i + 2]]}, 0, {0}));
                }
            }
        }
    }
    return create;
}

void Heap::ShiftDown(Presentation &myPresentation, std::vector<int> highlightLines, int curID)
{
    myPresentation.CopySetToLast(-1);
    myPresentation.SetStartAnimation(-1, 1);
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
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[curID]], nodes[itemID[nextID]]}, 0, highlightLines));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[nextID]], nodes[itemID[curID]]}, 0, highlightLines));

        from = nodes[itemID[curID]];
        to = nodes[itemID[nextID]];
        std::swap(itemID[curID], itemID[nextID]);
        CalcPosition(nodes);
        curID = nextID;
    }
}

void Heap::ShiftUp(Presentation &myPresentation, std::vector<int> highlightLines, int curID)
{
    myPresentation.CopySetToLast(-1);
    myPresentation.SetStartAnimation(-1, 1);
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
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[curID]], nodes[itemID[nextID]]}, 0, highlightLines));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[nextID]], nodes[itemID[curID]]}, 0, highlightLines));

        from = nodes[itemID[curID]];
        to = nodes[itemID[nextID]];
        std::swap(itemID[curID], itemID[nextID]);
        CalcPosition(nodes);
        curID = nextID;
    }
}

void Heap::BuildCreateAnimation()
{
    nodes = BuildNodeFromValue(values);
    myPresentation = CreateAnimation(nodes);
    myPresentation.InitBoardText(createList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
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
            if (StringToVector(values[itemID[curID]])[0] >= StringToVector(values[itemID[nextID]])[0]) 
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
    while(getline(fin, s))
    {
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

void Heap::Top()
{
    if (size > 0)
    {
        myPresentation.present = {BasicStructure(nodes)};
        for (std::vector<NewAnimation> &curSet: myPresentation.present[0].setOfAnimation)
        {
            for (NewAnimation &curAnimation: curSet)
            {
                curAnimation.listHighlights = {0};
            }
        }
        myPresentation.CopySetToLast(-1);
        myPresentation.CreateNewSet(-1);
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, BLUE, {nodes[itemID[0]]}, 0, {0}));
        myPresentation.InitBoardText(topList, positionBoard, width, height);
        myPresentation.numberOfPresentation = 0;
        myPresentation.currentPresentation = 0;
        curRemoteState = 0;
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
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {nodes[curID]}, 0, {0}));
        if (curID > 0 && (curID - 1) / 2 >= 0)
        {
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}, 0, {0}));
        }

        myPresentation.CopySetToLast(-1);
        myPresentation.SetStartAnimation(-1, 1);
        myPresentation.EraseAnimation(-1, -1, NewAnimation(2, 0, GREEN, {nodes[curID]}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {nodes[curID]}, 0, {0}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {nodes[curID]}, 0, {0}));
        if (curID > 0 && (curID - 1) / 2 >= 0)
        {
            myPresentation.EraseAnimation(-1, -1, NewAnimation(4, 0, GREEN, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}, 0, {0}));
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}, 0, {0}));
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[itemID[(curID - 1) / 2]], nodes[curID]}, 0, {0}));
        }
        HeapifyUp(myPresentation, {1});
    }; 
    AddValueToHeap(val);
    myPresentation.InitBoardText(pushList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

void Heap::Pop()
{
    myPresentation.present = {BasicStructure(nodes)};
    auto PopTop = [&]() -> void 
    {
        myPresentation.CopySetToLast(-1);
        myPresentation.CreateNewSet(-1);
        myPresentation.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[0]]}));
        myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, RED, {nodes[itemID[0]]}, 0, {0}));

        myPresentation.CopySetToLast(-1);
        myPresentation.SetStartAnimation(-1, 1);
        if (size > 1)
        {
            myPresentation.EraseAnimation(-1, 0, NewAnimation(4, 0, BLACK, {nodes[itemID[(size - 2) / 2]], nodes[itemID[size - 1]]}));
            myPresentation.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[size - 1]]}));
            myPresentation.CreateNewSet(-1);
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, RED, {nodes[itemID[(size - 2) / 2]], nodes[itemID[size - 1]]}, 0, {0}));
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[itemID[size - 1]], nodes[itemID[0]]}, 0, {0}));
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
        HeapifyDown(myPresentation, {1});
    }; 
    PopTop();
    myPresentation.InitBoardText(popList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

void Heap::DrawToolBar()
{
    toolBarButtons[0][0].DrawButton(0.3, 0.1, LIME, true); 
    DrawTextureEx(flagToolBarButtons[0][0] == false ? toolBarRightArrow : toolBarLeftArrow, Vector2{2.5, 570 + (183.0) / 2 - 16.0 / 2 - 80}, 0, 1, WHITE);
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
            Button v = {Vector2{17 + 160 + 2, 607 - 80}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[3][0] == true) // Pop
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[5][0] == true) // Update
        {
            Button v = {Vector2{17 + 160 + 2, 718 - 80 + 35 + 2}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();

            Button i = {Vector2{17 + 160 + 2, 718 - 80}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
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
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 - 80}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 - 80}, 60, 35, (char *)"Confirm"};
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
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 80}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2 - 80}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 - 80}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2 - 80}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 5 && j == 0)
                    {
                        insertI.oldWidth = 60;
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 718 - 80}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 718 - 80 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 718 - 80 + 35 + 2}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 718 - 80 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
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
                        if (StringToVector(values[itemID[curID]])[0] >= StringToVector(values[itemID[nextID]])[0]) 
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
        if (flagToolBarButtons[5][0] == true) // Top
        {
            std::string v = insertV.HandleTextBox();
            std::string i = insertI.HandleTextBox();
            if (v.empty() == true || i.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[5][0] = false;
            std::string vValue = std::to_string(StringToVector(v)[0]);
            int iValue = StringToVector(i)[0];
            Update(iValue, vValue);
        }
    }
}


void Heap::Draw() 
{  
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, fontNumber, robotoFont, isLightMode, speed, curRemoteState);
}


void Heap::HandleRemote()
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

void Heap::DrawRemote()
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

std::vector<int> Heap::StringToVector(std::string listChar)
{
    std::vector<int> values;
    int cur = -1;
    for (int i = 0; i < int(listChar.size()); ++i) 
    {
        if (listChar[i] == ' ' || listChar[i] == ','||listChar[i]=='\n')
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

void Heap::Update(int pos, std::string val)
{
    if (size > 0)
    {
        pos = std::min(pos, size - 1);
        myPresentation.present = {BasicStructure(nodes)};
        auto UpdateHeap = [&](int pos, std::string value) -> void 
        {
            myPresentation.CopySetToLast(-1);
            myPresentation.CreateNewSet(-1);
            myPresentation.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[pos]]}, 0, {0}));
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, RED, {nodes[itemID[pos]]}, 0, {0}));

            values[itemID[pos]] = value;
            nodes[itemID[pos]].value = value;
            myPresentation.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {nodes[itemID[pos]]}, 0, {0}));

            CalcPosition(nodes);
            myPresentation.CopySetToLast(-1);
            myPresentation.SetStartAnimation(-1, 1);
            myPresentation.EraseAnimation(-1, -1, NewAnimation(2, 0, GREEN, {nodes[itemID[pos]]}, 0, {0}));
            myPresentation.InsertAnimationToSet(-1, 0, NewAnimation(0, 0, BLACK, {nodes[itemID[pos]]}, 0, {0}));
            
            ShiftUp(myPresentation, {1}, pos);
            ShiftDown(myPresentation, {2}, pos);
        }; 
        UpdateHeap(pos, val);
        myPresentation.InitBoardText(updateList, positionBoard, width, height);
        myPresentation.numberOfPresentation = 0;
        myPresentation.currentPresentation = 0;
        curRemoteState = 0;
    }
}