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
}

void SinglyLinkedList::Init()
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

    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }

    nodes.clear();
    values.clear();
    isLightMode = 1;

    speed = 0.05;
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
            create.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {curNode}));
        }
    }
    { // Insert all edges
        create.CopySetToLast(-1);
        create.SetStartAnimation(-1, -1, 1);
        create.CreateNewSet(-1);
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, GREEN, {nodes[i - 1], nodes[i]}));
        }
    }
    { // Remove animations
        create.CreateNewPresentation();
        create.CreateNewSet(-1);
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(0, 0, BLACK, {curNode}));
        }
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(4, 0, BLACK, {nodes[i - 1], nodes[i]}));
        }
        for (Node curNode: nodes) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(3, 0, GREEN, {curNode}));
        }
        for (int i = 1; i < int(nodes.size()); ++i) 
        {
            create.InsertAnimationToSet(-1, -1, NewAnimation(6, 0, GREEN, {nodes[i - 1], nodes[i]}));
        }
    }
    return create;
}

void SinglyLinkedList::BuildCreateAnimation()
{
    nodes = BuildNodeFromValue(values);
    myPresentation.currentPresentation = 0;
    myPresentation = CreateAnimation(nodes);
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

void SinglyLinkedList::Search(std::string val)
{
    nodes = BuildNodeFromValue(values);
    int pos = FindPosition(val);
    myPresentation = SearchAnimation(pos, BLUE);
}

Presentation SinglyLinkedList::UpdateAnimation(int pos, std::string val)
{
    Presentation update;
    update = SearchAnimation(pos, BLUE);

    update.CopySetToLast(-1);
    update.SetStartAnimation(-1, 1);
    update.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[pos]}));
    
    NewAnimation animation = update.GetAnimation(-1, -1, -1);
    update.EraseAnimation(-1, -1, animation);

    animation.type = 3;
    animation.color = BLUE;
    animation.curAnimation = 0;
    update.InsertAnimationToSet(-1, -1, animation);

    update.CopySetToLast(-1);
    update.SetStartAnimation(-1, 1);

    animation.type = 2;
    animation.color = GREEN;
    animation.curAnimation = 0;
    animation.nodes[0].value = val;
    update.InsertAnimationToSet(-1, -1, animation);

    return update;
}

void SinglyLinkedList::Update(int pos, std::string val)
{
    if (pos >= int(nodes.size())) return;
    nodes = BuildNodeFromValue(values);
    myPresentation.currentPresentation = 0;
    myPresentation = UpdateAnimation(pos, val);
    values[pos] = val;
    nodes[pos].value = val;
}

Presentation SinglyLinkedList::InsertAnimation(int pos, std::string val)
{
    Presentation insert;
 
    if (pos == int(nodes.size()))
    { 
        if (nodes.empty() == true)
        {
            insert.CreateNewPresentation();
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {Node(Vector2{100.0f, 180.0f}, 20, val)}));
        } 
        else
        {
            insert = SearchAnimation(-1, BLUE);
            insert.CopySetToLast(-1);
            insert.SetStartAnimation(-1, 1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {Node(Vector2{nodes.back().position.x + 100.0f, nodes.back().position.y}, 20, val)}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes.back(), Node(Vector2{nodes.back().position.x + 100.0f, nodes.back().position.y}, 20, val)}));
        }
    } 
    else
    {
        insert = SearchAnimation(pos, BLUE);
        
        { // Move edge, Insert new node, Insert new Edge
            insert.CopySetToLast(-1);
            insert.SetStartAnimation(-1, 1);

            NewAnimation tmpAnimation = insert.GetAnimation(-1, -1, -1);
            insert.EraseAnimation(-1, -1, insert.GetAnimation(-1, -1, -1));
            if (pos > 0)
            {
                insert.EraseAnimation(-1, -1, insert.GetAnimation(-1, -1, -1));
            }
            insert.InsertAnimationToSet(-1, -1, tmpAnimation);
            if (pos > 0) 
            {
                insert.EraseAnimation(-1, 0, NewAnimation(4, 0, BLACK, {nodes[pos - 1], nodes[pos]}));
            }
            Node newNode(Vector2{nodes[pos].position.x, nodes[pos].position.y + 100.0f}, 20, val);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, GREEN, {newNode}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {newNode, nodes[pos]}));
            if (pos > 0)
            {
                insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], nodes[pos], newNode}));
            }
            
        }
    
        { // Move new node, nodes[pos], ... nodes.back()
            insert.CopySetToLast(-1);
            insert.SetStartAnimation(-1, 1);
    
            for (int i = int(nodes.size()) - 2; i >= pos; --i)
            {
                insert.EraseAnimation(-1, 0, NewAnimation(4, 0, BLACK, {nodes[i], nodes[i + 1]}));
            }
            for (int i = int(nodes.size()) - 1; i >= pos; --i)
            {
                insert.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[i]}));
            }
            
            {
                Node newNode(Vector2{nodes[pos].position.x, nodes[pos].position.y + 100.0f}, 20, val);
                if (pos > 0)
                {
                    insert.EraseAnimation(-1, -1, NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], nodes[pos], newNode}));
                }
                insert.EraseAnimation(-1, -1, NewAnimation(5, 0, ORANGE, {newNode, nodes[pos]}));
                insert.EraseAnimation(-1, -1, NewAnimation(2, 0, GREEN, {newNode}));          
            }
            
            { // Delete BLUE node
                insert.EraseAnimation(-1, pos + 1, NewAnimation(2, 0, BLUE, {nodes[pos]}));    
            }
    
            Node newNode(Vector2{nodes[pos].position.x, nodes[pos].position.y + 100.0f}, 20, val);
            Node lastNode = nodes.back();
            lastNode.position.x += 100.f;
    
            // Remember to remove nodes.back()
            nodes.push_back(lastNode);
            
            insert.CreateNewSet(-1);
            insert.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, GREEN, {newNode, nodes[pos]}));
            insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, ORANGE, {newNode, nodes[pos], nodes[pos], nodes[pos + 1]}));
            if (pos > 0)
            {
                insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], newNode, nodes[pos]}));
            }
            for (int i = pos; i < int(nodes.size()) - 1; ++i)
            {
                insert.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, (i == pos ? BLUE : BLACK), {nodes[i], nodes[i + 1]}));
                if (i < int(nodes.size()) - 2) {
                    insert.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, BLACK, {nodes[i], nodes[i + 1], nodes[i + 1], nodes[i + 2]}));
                }
            }
    
            nodes.pop_back();
        }
    }

    return insert;
}

void SinglyLinkedList::Insert(int pos, std::string val)
{
    nodes = BuildNodeFromValue(values);
    pos = std::min(pos, int(nodes.size()));
    myPresentation.currentPresentation = 0;
    myPresentation = InsertAnimation(pos, val);
    values.insert(values.begin() + pos, val);
    nodes = BuildNodeFromValue(values);
}

Presentation SinglyLinkedList::DeleteAnimation(int pos)
{
    Presentation _delete;
    if (pos != -1)
    {
        _delete = SearchAnimation(pos, RED);

        _delete.CopySetToLast(-1);
        _delete.SetStartAnimation(-1, 1);
        
        NewAnimation animation = _delete.GetAnimation(-1, -1, -1);
        _delete.EraseAnimation(-1, -1, NewAnimation(2, 0, RED, {nodes[pos]}));
        if (pos > 0) 
        {
            _delete.EraseAnimation(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[pos - 1], nodes[pos]}));
        }
        
        SetOfAnimation tmpGroups = _delete.present.back();
        animation.type = 3;
        animation.color = RED;
        animation.curAnimation = 0;
        _delete.InsertAnimationToSet(-1, -1, animation);

        if (pos < int(nodes.size()) - 1)
        {   
            NewAnimation deleteEdge = _delete.GetAnimation(-1, 0, int(nodes.size()) + pos);
            deleteEdge.type = 6;
            deleteEdge.color = RED;
            deleteEdge.curAnimation = 0;
            _delete.InsertAnimationToSet(-1, -1, animation);
            _delete.EraseAnimation(-1, 0, _delete.GetAnimation(-1, 0, int(nodes.size()) + pos));
        }
        if (pos > 0)
        {
            _delete.EraseAnimation(-1, 0, _delete.GetAnimation(-1, 0, int(nodes.size()) + pos - 1));
        }
        _delete.EraseAnimation(-1, 0, _delete.GetAnimation(-1, 0, pos));

        _delete.CopySetToLast(-1);
        _delete.SetStartAnimation(-1, 1);
        if (pos > 0 && pos < int(nodes.size()) - 1)
        {
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(5, 0, ORANGE, {nodes[pos - 1], nodes[pos + 1]}));
        }

        _delete.present.push_back(tmpGroups);
        if (pos > 0)
        {
            for (int i = int(nodes.size()) - 2; i >= pos; --i)
            {
                _delete.EraseAnimation(-1, 0, NewAnimation(4, 0, BLACK, {nodes[i], nodes[i + 1]}));
            }
            if (pos < int(nodes.size()) - 1)
            {   
                _delete.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[pos]}));
            }
            if (pos > 0)
            {
                _delete.EraseAnimation(-1, 0, NewAnimation(4, 0, BLACK, {nodes[pos - 1], nodes[pos]}));
            }
            for (int i = int(nodes.size()) - 1; i >= pos; --i)
            {
                _delete.EraseAnimation(-1, 0, NewAnimation(0, 0, BLACK, {nodes[i]}));
            }
        }
        else 
        {
            while (_delete.present.back().setOfAnimation[0].size())
            {
                NewAnimation animation = _delete.present.back().setOfAnimation[0].back();
                _delete.EraseAnimation(-1, 0, animation);
            }
        }

        _delete.CreateNewSet(-1);
        if (pos > 0 && pos < int(nodes.size()) - 1)
        {
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], nodes[pos + 1], nodes[pos]}));
        }
        for (int i = int(nodes.size()) - 1; i > pos; --i)
        {
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(8, 0, BLACK, {nodes[i], nodes[i - 1]}));
        }
        for (int i = int(nodes.size()) - 2; i > pos; --i)
        {
            _delete.InsertAnimationToSet(-1, -1, NewAnimation(7, 0, BLACK, {nodes[i], nodes[i - 1], nodes[i + 1], nodes[i]}));
        }
    }
    else 
    {
        myPresentation = SearchAnimation(-1, RED);
    }

    return _delete;
}

void SinglyLinkedList::Delete(std::string val)
{
    nodes = BuildNodeFromValue(values);
    int pos = FindPosition(val);
    myPresentation.currentPresentation = 0;
    myPresentation = DeleteAnimation(pos);
    if (pos != -1)
    {
        values.erase(values.begin() + pos);
        nodes.erase(nodes.begin() + pos);
        nodes = BuildNodeFromValue(values);
    }
}

void SinglyLinkedList::DrawToolBar()
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
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[3][0] = false;
            std::string vValue = std::to_string(StringToVector(v)[0]);

            Delete(vValue);
    
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
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, fontNumber, GetFontDefault(), isLightMode, speed);
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
                     