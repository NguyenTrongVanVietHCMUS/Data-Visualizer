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
        {(Vector2){0, 570}, 15, 183, (char *)"Open_Close"}
    };
    toolBarButtons[1] = 
    {
        {(Vector2){17, 570}, 160, 35, (char *)"Create"},
        {(Vector2){179, 570}, 80, 35, (char *)"Empty"},
        {(Vector2){261, 570}, 80, 35, (char *)"Random"},
        {(Vector2){343, 570}, 80, 35, (char *)"File"},
        {(Vector2){425, 570}, 80, 35, (char *)"Enter"},
    };
    toolBarButtons[2] = 
    {
        {(Vector2){17, 607}, 160, 35, (char *)"Insert"}
    };
    toolBarButtons[3] = 
    {
        {(Vector2){17, 644}, 160, 35, (char *)"Delete"}
    };
    toolBarButtons[4] = 
    {
        {(Vector2){17, 681}, 160, 35, (char *)"Update"}
    };
    toolBarButtons[5] = 
    {
        {(Vector2){17, 718}, 160, 35, (char *)"Search"}
    };
    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }

    // enterList.Init();
    enterList.oldWidth = 80;
    enterList.textBox = {(Vector2){425 + 80 + 2, 570}, 80, 35};
    enterList.confirm = {(Vector2){425 + 80 + 2, 570 + 35 + 2}, 80, 35, (char *)"Confirm"};

    nodes.clear();
    values.clear();
    isLightMode = 1;

    speed = 0.05;
    posAnimation = 0;
    animations.clear();
}

std::vector<Node> SinglyLinkedList::BuildNodeFromValue(const std::vector<int> &values)
{
    float x = 100.0f, y = 180.0f;
    std::vector<Node> nodes;
    for (const int &value: values) 
    {
        nodes.push_back({(Vector2){x, y}, 24, value});
        x += 100.0f;
    }
    return nodes;
}

std::vector<std::vector<std::vector<NewAnimation>>> SinglyLinkedList::CreateAnimation(const std::vector<Node> &nodes)
{
    std::vector<std::vector<std::vector<NewAnimation>>> animations;

    { // Insert all nodes
        animations.push_back({});
        animations.back().push_back({});
        for (Node curNode: nodes) animations.back().back().push_back(NewAnimation(2, 0, GREEN, {curNode}));
    }

    { // Insert all edges
        animations.push_back(animations.back());
        for (NewAnimation &animation: animations.back().back()) animation.curAnimation = 1;
    
        animations.back().push_back({});
        for (int i = 1; i < int(nodes.size()); ++i) animations.back().back().push_back(NewAnimation(5, 0, GREEN, {nodes[i - 1], nodes[i]}));
    }

    { // Remove animations
        animations.push_back({animations.back()});
        for (NewAnimation &animation: animations.back()[0]) 
        {
            animation.type = 3;
            animation.curAnimation = 0;
        }
        for (NewAnimation &animation: animations.back()[1]) 
        {
            animation.type = 6;
            animation.curAnimation = 0;
        }
        animations.back().push_back(animations.back()[0]);
        for (NewAnimation &animation: animations.back().back())
        {
            animation.type = 0;
            animation.curAnimation = 0;
            animation.color = BLACK;
        }
        animations.back().push_back(animations.back()[1]);
        for (NewAnimation &animation: animations.back().back())
        {
            animation.type = 4;
            animation.curAnimation = 0;
            animation.color = BLACK;
        }
        for (int i = 0; i < 2; ++i) 
        {
            std::swap(animations.back()[i], animations.back()[i + 2]);
        }
        for (NewAnimation animation: animations.back()[3])
        {
            animations.back()[2].push_back(animation);
        }
        animations.back().pop_back();
    }

    return animations;
}

void SinglyLinkedList::BuildCreateAnimation()
{
    nodes = BuildNodeFromValue(values);
    posAnimation = 0;
    animations.clear();
    animations = CreateAnimation(nodes);
}

int SinglyLinkedList::FindPosition(int value)
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
    posAnimation = 0;
    animations.clear();
}

void SinglyLinkedList::RandomNewData()
{
    ClearAllData();

    int n = GetRandomValue(1, 15);
    for (int i = 1; i <= n; ++i)
    {
        values.push_back(GetRandomValue(0, 999));
    }
    BuildCreateAnimation();
}

void SinglyLinkedList::InputDataFromFile()
{
    ClearAllData();

    FilePathList droppedFiles = LoadDroppedFiles();
    char *filePath = new char[2048];

    TextCopy(filePath, droppedFiles.paths[0]);
    UnloadDroppedFiles(droppedFiles);

    std::ifstream fin(filePath);
    std::string s;
    getline(fin, s);

    int curValue = -1;
    for (char &c: s)
    {
        if ('0' <= c && c <= '9')
        {
            if (curValue == -1) curValue = (c - '0'); else curValue = 10 * curValue + (c - '0');
            continue;
        }
        if (curValue != -1)
        {
            values.push_back(curValue);
            curValue = -1;
        }
    }
    if (curValue != -1)
    {
        values.push_back(curValue);
    }
    fin.close();
    BuildCreateAnimation();
}

std::vector<std::vector<NewAnimation>> SinglyLinkedList::BasicStructure(const std::vector<Node> &nodes)
{
    std::vector<std::vector<NewAnimation>> animations;
    if (nodes.empty() == false) { 
        // Insert all nodes and edges
        animations.push_back({});
        for (Node curNode: nodes) animations[0].push_back(NewAnimation(0, 0, BLACK, {curNode}));
        for (int i = 1; i < int(nodes.size()); ++i) animations[0].push_back(NewAnimation(4, 0, BLACK, {nodes[i - 1], nodes[i]}));
    }
    return animations;
}

std::vector<std::vector<std::vector<NewAnimation>>> SinglyLinkedList::SearchAnimation(int pos, Color color)
{
    std::vector<std::vector<std::vector<NewAnimation>>> animations;
    animations.push_back({BasicStructure(nodes)});
    {
        for (int i = 0; i < int(nodes.size()); ++i)
        {
            animations.push_back(animations.back());
            for (NewAnimation &animation: animations.back().back())
            {
                animation.curAnimation = 1;
            }
            if (i > 0)
            {
                animations.back().back().pop_back();
                animations.back().push_back({   
                                            NewAnimation(0, 0, ORANGE, {nodes[i - 1]}), // Draw hollow node
                                            NewAnimation(3, 0, ORANGE, {nodes[i - 1]}), // Draw delete node
                                            NewAnimation(5, 0, ORANGE, {nodes[i - 1], nodes[i]}), // Draw insert edge
                                            });
                animations.back().back().push_back(NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]}));
            }
            else 
            {
                animations.back().push_back({NewAnimation(2, 0, (i == pos ? color : ORANGE), {nodes[i]})});
            }
            if (i == pos) 
            {
                return animations;
            }
        }
    }
    if (nodes.empty() == false)
    {
        animations.push_back(animations.back());
        for (NewAnimation &animation: animations.back().back())
        {
            animation.curAnimation = 1;
        }
        animations.back().back().pop_back();
        animations.back().push_back({
                                    NewAnimation(0, 0, ORANGE, {nodes.back()}),
                                    NewAnimation(3, 0, ORANGE, {nodes.back()})
                                    });
    }
    return animations;
}

void SinglyLinkedList::Search(int val)
{
    nodes = BuildNodeFromValue(values);
    int pos = FindPosition(val);
    posAnimation = 0;
    animations = SearchAnimation(pos, BLUE);
}

std::vector<std::vector<std::vector<NewAnimation>>> SinglyLinkedList::UpdateAnimation(int pos, int val)
{
    std::vector<std::vector<std::vector<NewAnimation>>> animations;
    animations = SearchAnimation(pos, BLUE);

    animations.push_back(animations.back());
    for (NewAnimation &animation: animations.back().back()) {
        animation.curAnimation = 1;
    }
    animations.back()[0].erase(animations.back()[0].begin() + pos);
    NewAnimation animation = animations.back().back().back();
    animations.back().back().pop_back();

    animation.type = 3;
    animation.color = BLUE;
    animation.curAnimation = 0;
    animations.back().back().push_back(animation);

    animations.push_back(animations.back());
    for (NewAnimation &animation: animations.back().back()) {
        animation.curAnimation = 1;
    }
    animation.type = 2;
    animation.color = GREEN;
    animation.curAnimation = 0;
    animation.nodes[0].value = val;
    animations.back().back().push_back(animation);

    return animations;
}

void SinglyLinkedList::Update(int pos, int val)
{
    if (pos >= int(nodes.size())) return;
    nodes = BuildNodeFromValue(values);
    posAnimation = 0;
    animations = UpdateAnimation(pos, val);
    values[pos] = val;
    nodes[pos].value = val;
}

std::vector<std::vector<std::vector<NewAnimation>>> SinglyLinkedList::InsertAnimation(int pos, int val)
{
    std::vector<std::vector<std::vector<NewAnimation>>> animations;
 
    if (pos == int(nodes.size()))
    { 
        if (nodes.empty() == true)
        {
            animations.push_back({{}});
            Node newNode = Node((Vector2){100.0f, 180.0f}, 24, val);
            animations.back().back().push_back(NewAnimation(2, 0, GREEN, {newNode}));
        } 
        else
        {
            animations = SearchAnimation(-1, BLUE);
            animations.push_back(animations.back());
            for (NewAnimation &animation: animations.back().back())
            {
                animation.curAnimation = 1;
            }

            Node newNode = Node((Vector2){nodes.back().position.x + 100.0f, nodes.back().position.y}, 24, val);
            animations.back().back().push_back(NewAnimation(2, 0, GREEN, {newNode}));
            animations.back().back().push_back(NewAnimation(5, 0, ORANGE, {nodes.back(), newNode}));

            // animations.push_back(animations.back());
            // animations.back().back().pop_back();
            // animations.back().back().pop_back();
            // animations.back().back().push_back(NewAnimation(0, 0, BLACK, {newNode}));
            // animations.back().back().push_back(NewAnimation(3, 0, GREEN, {newNode}));
            // animations.back().back().push_back(NewAnimation(4, 0, BLACK, {nodes.back(), newNode}));
            // animations.back().back().push_back(NewAnimation(6, 0, ORANGE, {nodes.back(), newNode}));
        }
    } 
    else
    {
        animations = SearchAnimation(pos, BLUE);
        
        { // Move edge, Insert new node, Insert new Edge
            animations.push_back({animations.back()});
            for (NewAnimation &animation: animations.back().back())
            {
                animation.curAnimation = 1;
            }
            NewAnimation tmpAnimation = animations.back().back().back();
            animations.back().back().pop_back();
            if (pos > 0)
            {
                animations.back().back().pop_back();
            }
            animations.back().back().push_back(tmpAnimation);
            if (pos > 0) 
            {
                animations.back()[0].erase(animations.back()[0].begin() + int(nodes.size()) + pos - 1);
            }
            Node newNode((Vector2){nodes[pos].position.x, nodes[pos].position.y + 100.0f}, 24, val);
            animations.back().back().push_back(NewAnimation(2, 0, GREEN, {newNode}));
            animations.back().back().push_back(NewAnimation(5, 0, ORANGE, {newNode, nodes[pos]}));
            if (pos > 0)
            {
                animations.back().back().push_back(NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], nodes[pos], newNode}));
            }
        }
    
        { // Move new node, nodes[pos], ... nodes.back()
            animations.push_back({animations.back()});
            for (NewAnimation &animation: animations.back().back())
            {
                animation.curAnimation = 1;
            }
    
            for (int i = int(nodes.size()) - 2; i >= pos; --i)
            {
                animations.back()[0].erase(animations.back()[0].begin() + int(nodes.size()) + i);
            }
            for (int i = int(nodes.size()) - 1; i >= pos; --i)
            {
                animations.back()[0].erase(animations.back()[0].begin() + i);
            }
    
            animations.back().back().pop_back();
            animations.back().back().pop_back();
            if (pos > 0)
            {
                animations.back().back().pop_back();
            }
            
            { // Delete BLUE node
                animations.back()[pos + 1].pop_back();            
            }
    
            Node newNode((Vector2){nodes[pos].position.x, nodes[pos].position.y + 100.0f}, 24, val);
            Node lastNode = nodes.back();
            lastNode.position.x += 100.f;
    
            // Remember to remove nodes.back()
            nodes.push_back(lastNode);
    
            animations.back().push_back({});
            animations.back().back().push_back(NewAnimation(8, 0, GREEN, {newNode, nodes[pos]}));
            animations.back().back().push_back(NewAnimation(7, 0, ORANGE, {newNode, nodes[pos], nodes[pos], nodes[pos + 1]}));
            if (pos > 0)
            {
                animations.back().back().push_back(NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], newNode, nodes[pos]}));
            }
            for (int i = pos; i < int(nodes.size()) - 1; ++i)
            {
                animations.back().back().push_back(NewAnimation(8, 0, (i == pos ? BLUE : BLACK), {nodes[i], nodes[i + 1]}));
                if (i < int(nodes.size()) - 2) {
                    animations.back().back().push_back(NewAnimation(7, 0, BLACK, {nodes[i], nodes[i + 1], nodes[i + 1], nodes[i + 2]}));
                }
            }
    
            nodes.pop_back();
        }
    }

    return animations;
}

void SinglyLinkedList::Insert(int pos, int val)
{
    posAnimation = 0;
    nodes = BuildNodeFromValue(values);
    pos = std::min(pos, int(nodes.size()));
    animations = InsertAnimation(pos, val);
    values.insert(values.begin() + pos, val);
    nodes = BuildNodeFromValue(values);
}

std::vector<std::vector<std::vector<NewAnimation>>> SinglyLinkedList::DeleteAnimation(int pos)
{
    std::vector<std::vector<std::vector<NewAnimation>>> animations;
    if (pos != -1)
    {
        animations = SearchAnimation(pos, RED);
        animations.insert(animations.begin(), BasicStructure(nodes));

        animations.push_back(animations.back());
        for (NewAnimation &animation: animations.back().back())
        {
            animation.curAnimation = 1;
        }
        
        NewAnimation animation = animations.back().back().back();
        animations.back().back().pop_back();
        if (pos > 0) 
        {
            animations.back().back().pop_back();
        }
        
        std::vector<std::vector<NewAnimation>> tmpGroups = animations.back();
        animation.type = 3;
        animation.color = RED;
        animation.curAnimation = 0;
        animations.back().back().push_back(animation);

    
        if (pos < int(nodes.size()) - 1)
        {   
            NewAnimation deleteEdge = animations.back()[0][int(nodes.size()) + pos];
            deleteEdge.type = 6;
            deleteEdge.color = RED;
            deleteEdge.curAnimation = 0;
            animations.back().back().push_back(deleteEdge);
            animations.back()[0].erase(animations.back()[0].begin() + int(nodes.size()) + pos);
        }
        if (pos > 0)
        {
            animations.back()[0].erase(animations.back()[0].begin() + int(nodes.size()) + pos - 1);
        }
        animations.back()[0].erase(animations.back()[0].begin() + pos);

        animations.push_back(animations.back());
        for (NewAnimation &animation: animations.back().back())
        {
            animation.curAnimation = 1;
        }
        if (pos > 0 && pos < int(nodes.size()) - 1)
        {
            animations.back().back().push_back(NewAnimation(5, 0, ORANGE, {nodes[pos - 1], nodes[pos + 1]}));
        }

        animations.push_back(tmpGroups);
        if (pos > 0)
        {
            for (int i = int(nodes.size()) - 2; i >= pos; --i)
            {
                animations.back()[0].erase(animations.back()[0].begin() + int(nodes.size()) + i);
            }
            if (pos < int(nodes.size()) - 1)
            {   
                animations.back()[0].erase(animations.back()[0].begin() + int(nodes.size()) + pos);
            }
            if (pos > 0)
            {
                animations.back()[0].erase(animations.back()[0].begin() + int(nodes.size()) + pos - 1);
            }
            for (int i = int(nodes.size()) - 1; i >= pos; --i)
            {
                animations.back()[0].erase(animations.back()[0].begin() + i);
            }
        }
        else 
        {
            animations.back().erase(animations.back().begin());
        }
        animations.back().push_back({});
        if (pos > 0 && pos < int(nodes.size()) - 1)
        {
            animations.back().back().push_back(NewAnimation(7, 0, ORANGE, {nodes[pos - 1], nodes[pos - 1], nodes[pos + 1], nodes[pos]}));
        }
        for (int i = int(nodes.size()) - 1; i > pos; --i)
        {
            animations.back().back().push_back(NewAnimation(8, 0, BLACK, {nodes[i], nodes[i - 1]}));
        }
        for (int i = int(nodes.size()) - 2; i > pos; --i)
        {
            animations.back().back().push_back(NewAnimation(7, 0, BLACK, {nodes[i], nodes[i - 1], nodes[i + 1], nodes[i]}));
        }
    }
    else 
    {
        animations = SearchAnimation(-1, RED);
    }

    return animations;
}

void SinglyLinkedList::Delete(int val)
{
    nodes = BuildNodeFromValue(values);
    int pos = FindPosition(val);
    posAnimation = 0;
    animations = DeleteAnimation(pos);
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
    DrawTextureEx(flagToolBarButtons[0][0] == false ? toolBarRightArrow : toolBarLeftArrow, (Vector2){2.5, 570 + 183.0 / 2 - 16.0 / 2}, 0, 1, WHITE);
    if (flagToolBarButtons[1][3] == true) 
    {
        DrawTextureEx(dropFile, (Vector2){float(GetScreenWidth() / 2.0 - 354.25), float(GetScreenHeight() / 2.0 - 200)}, 0, 0.5, Fade(LIME, 0.8));
    }
    if (flagToolBarButtons[0][0] == true) 
    {
        if (flagToolBarButtons[1][4] == true)
        {
            enterList.DrawTextBox();
        }
        if (flagToolBarButtons[2][0] == true) // Insert
        {
            Button v = {(Vector2){17 + 160 + 2, 607 + 35 + 2}, 40, 35, (char *)"v ="};
            Button i = {(Vector2){17 + 160 + 2, 607}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[3][0] == true) // Delete
        {
            Button v = {(Vector2){17 + 160 + 2, 607 + 35 + 2}, 40, 35, (char *)"v ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[4][0] == true) // Update
        {
            Button v = {(Vector2){17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 40, 35, (char *)"v ="};
            Button i = {(Vector2){17 + 160 + 2, 607 + 35 + 2 + 35 + 2}, 40, 35, (char *)"i ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[5][0] == true) // Search
        {
            Button v = {(Vector2){17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 40, 35, (char *)"v ="};
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
                        insertI.textBox = {(Vector2){17 + 160 + 2 + 35, 607}, 60, 35};
                        insertI.confirm = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35};
                        insertV.confirm = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 3 && j == 0)
                    {
                        insertV.oldWidth = 60;
                        insertV.textBox = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2}, 60, 35};
                        insertV.confirm = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 4 && j == 0)
                    {
                        insertI.oldWidth = 60;
                        insertI.textBox = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2}, 60, 35};
                        insertI.confirm = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35};
                        insertV.confirm = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 5 && j == 0)
                    {
                        insertV.oldWidth = 60;
                        insertV.textBox = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35};
                        insertV.confirm = {(Vector2){17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 + 35 + 2 + 35 + 2}, 60, 35, (char *)"Confirm"};
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
                values = StringToVector(listChar);
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
            int vValue = StringToVector(v)[0];
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
            int vValue = StringToVector(v)[0];

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
            int vValue = StringToVector(v)[0];
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
            int vValue = StringToVector(v)[0];
            Search(vValue);
    
            return;
        }
    }
}



void SinglyLinkedList::Draw() 
{  
    if (animations.empty() == false)
    {
        if (posAnimation == int(animations.size())) 
        {
            posAnimation = animations.size() - 1;
        }
        if (animations[posAnimation].empty() == false)
        {
            for (std::vector<NewAnimation> &group: animations[posAnimation])
            {
                bool done = true;
                for (NewAnimation &animation: group) 
                {   
                    done &= animation.DrawAnimation(hollowCircle, solidCircle, arrowEdge, GetFontDefault(), isLightMode, speed);
                }
                if (done == false)
                {
                    return;
                }
            }
            ++posAnimation;
        }
    }
}

std::vector<int> StringToVector(std::string listChar)
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
                     