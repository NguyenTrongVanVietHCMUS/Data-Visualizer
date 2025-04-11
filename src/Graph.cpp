#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <random> 
#include <numeric> 
#include <algorithm>



void Graph::Run()
{
    HandleToolBar();
    DrawToolBar();

    Draw();

    HandleRemote();
    DrawRemote();
    HandleDragging();
}

void Graph::Init()
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
        {Vector2{17, 607 - 50}, 160, 35, (char *)"Settings"}
    };
    toolBarButtons[3] = 
    {
        {Vector2{17, 644 - 50}, 160, 35, (char *)"Source"}
    };
    toolBarButtons[4] = 
    {
        {Vector2{17, 681 - 50}, 160, 35, (char *)"Shortest Path"}
    };
    toolBarButtons[5] = 
    {
        {Vector2{17, 718 - 50}, 160, 35, (char *)"MST"}
    };
    flagToolBarButtons.resize(6);
    for (int i = 0; i < 6; ++i) {
        flagToolBarButtons[i].assign(toolBarButtons[i].size(), false);
    }

    // enterList.Init();
    enterList.oldWidth = 80;
    enterList.textBox = {Vector2{425 + 80 + 2, 570 - 50}, 80, 35};
    enterList.confirm = {Vector2{425 + 80 + 2, 570 + 35 + 2 - 50}, 80, 35, (char *)"Confirm"};

    nodes.clear();
    edges.clear();
    isLightMode = 1;
    myPresentation.Clear();

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
    sourceList = {
        "dist[] = INF",
        "PQ.emplace(dist[source] = 0, source)",
        "while !PQ.empty() // PQ is a Priority Queue",
        "   [du, u] = PQ.top(), PQ.pop()",
        "   if (du == dist[u])",
        "       Fix shortestPath to u",
        "       for (each neighbor v of u)",
        "           relax(u, v, w(u, v)) + update PQ"
    };
    PathList = {
        "dist[] = INF",
        "PQ.emplace(dist[source] = 0, source)",
        "while !PQ.empty() // PQ is a Priority Queue",
        "   [du, u] = PQ.top(), PQ.pop()",
        "   if (du == dist[u])",
        "       Fix shortestPath to u",
        "       if (u == end) return TracePath()",
        "       for (each neighbor v of u)",
        "           relax(u, v, w(u, v)) + update PQ",
        "return INF"
    };
    MSTList = {
        "T = {s}",
        "enqueue {0, s} in PQ (by inc weight)",
        "while (!PQ.isEmpty)",
        "if (vertex v linked with e = PQ.remove NOT IN T)",
        "   T = T ∪ {v, e, w}, enqueue edges connected to v: {w(v), v}",
        "MST = T"
    };

    positionBoard = Vector2{1600 - 400, 800 - 300};
    width = 400;
    height = 300;
}

void Graph::UpdateGraph()
{
    myPresentation.currentPresentation = 0;
    myPresentation.present = {BasicStructure(nodes)};
}

void Graph::ClearAllData()
{
    nodes.clear();
    edges.clear();
    myPresentation.Clear();
}

#include <map>

Vector2 Graph::GenerateNodePositionSpread(int i, int n) {
    float angle = 2.0f * PI * i / n;
    float radius = std::min(GetScreenWidth(), GetScreenHeight()) * 0.35f;
    float centerX = GetScreenWidth() / 2.0f;
    float centerY = GetScreenHeight() / 2.0f;

    float x = centerX + radius * cos(angle) + GetRandomValue(-100, 100); // nhỏ nhiễu chút
    float y = centerY + radius * sin(angle) + GetRandomValue(-100, 100);
    return {x, y};
}


void Graph::RandomNewData() {
    ClearAllData();
    UnFixGraph();

    int n = GetRandomValue(5, 10);
    nodes.reserve(n);

    float centerX = GetScreenWidth() / 2.0f;
    float centerY = GetScreenHeight() / 2.0f;
    float radius = std::min(centerX, centerY) - 150;

    for (int i = 0; i < n; ++i) {
        Vector2 pos = GenerateNodePositionSpread(i, n);
        nodes.emplace_back(pos, NODE_RADIUS, std::to_string(i + 1));
    }

    edges.clear();
    std::map<std::pair<int, int>, int> edgeMap;

    bool isConnectedGraph = (GetRandomValue(1, 100) <= 60);

    if (isConnectedGraph) {
        std::vector<int> perm(n);
        std::iota(perm.begin(), perm.end(), 0);
        std::shuffle(perm.begin(), perm.end(), std::mt19937{std::random_device{}()});

        for (int i = 1; i < n; ++i) {
            int u = perm[i - 1];
            int v = perm[i];
            int w = GetRandomValue(1, 999);
            edgeMap[{std::min(u, v), std::max(u, v)}] = w;
        }

        int extraEdges = GetRandomValue(n / 2, n * 2);
        for (int i = 0; i < extraEdges; ++i) {
            int u = GetRandomValue(0, n - 1);
            int v = GetRandomValue(0, n - 1);
            if (u != v) {
                int w = GetRandomValue(1, 999);
                auto key = std::make_pair(std::min(u, v), std::max(u, v));
                if (edgeMap.find(key) == edgeMap.end() || edgeMap[key] > w) {
                    edgeMap[key] = w;
                }
            }
        }
    } else {
        int components = GetRandomValue(2, n / 3 + 1);
        std::vector<std::vector<int>> groups(components);
        for (int i = 0; i < n; ++i)
            groups[GetRandomValue(0, components - 1)].push_back(i);

        for (const auto& group : groups) {
            if (group.size() < 2) continue;

            for (size_t i = 1; i < group.size(); ++i) {
                int u = group[i - 1];
                int v = group[i];
                int w = GetRandomValue(1, 999);
                edgeMap[{std::min(u, v), std::max(u, v)}] = w;
            }

            int extraEdges = GetRandomValue(1, (int)group.size());
            for (int i = 0; i < extraEdges; ++i) {
                int u = group[GetRandomValue(0, group.size() - 1)];
                int v = group[GetRandomValue(0, group.size() - 1)];
                if (u != v) {
                    int w = GetRandomValue(1, 999);
                    auto key = std::make_pair(std::min(u, v), std::max(u, v));
                    if (edgeMap.find(key) == edgeMap.end() || edgeMap[key] > w) {
                        edgeMap[key] = w;
                    }
                }
            }
        }
    }

    for (std::map<std::pair<int, int>, int>::iterator it = edgeMap.begin(); it != edgeMap.end(); ++it) {
        int u = it->first.first;
        int v = it->first.second;
        int w = it->second;
        edges.push_back({u, v, w});
    }

    UpdateGraph();
}



void Graph::InputDataFromFile()
{
    ClearAllData();

    FilePathList droppedFiles = LoadDroppedFiles();
    char *filePath = new char[2048];

    TextCopy(filePath, droppedFiles.paths[0]);
    UnloadDroppedFiles(droppedFiles);
    std::ifstream fin(filePath);
    std::string s;
    values.clear() ; 
    while(getline(fin, s))
    {
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
    }

    fin.close();
    InputDataFromKeyboard(values) ; 
    // BuildBasicStructure();
}

SetOfAnimation Graph::BasicStructure(const std::vector<Node> &nodes)
{
    SetOfAnimation basicStructure;
    { 
        basicStructure.CreateNewSet();
        for (Node curNode: nodes) 
        {
            basicStructure.InsertAnimationToSet(-1, NewAnimation(0, 0, BLACK, {curNode}));
        }
        for (Edge &e: edges)
        {
            basicStructure.InsertAnimationToSet(-1, NewAnimation(9, 0, BLACK, {nodes[e.u], nodes[e.v]}, e.w));
        }
    }
    return basicStructure;
}

void Graph::DrawToolBar()
{
    toolBarButtons[0][0].DrawButton(0.3, 0.1, LIME, true); 
    DrawTextureEx(flagToolBarButtons[0][0] == false ? toolBarRightArrow : toolBarLeftArrow, Vector2{2.5, 570 + 183.0 / 2 - 16.0 / 2 - 50}, 0, 1, WHITE);
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
        if (flagToolBarButtons[3][0] == true) // Source
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 - 50}, 40, 35, (char *)"s ="};
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[4][0] == true) // Path
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 40, 35, (char *)"u ="};
            Button i = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 - 50}, 40, 35, (char *)"v ="};
            i.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            v.DrawButtonAndText(0, 0.8, LIME, true, fontRoboto, 20, RAYWHITE);
            insertI.DrawTextBox();
            insertV.DrawTextBox();
        }
        if (flagToolBarButtons[5][0] == true) // MST
        {
            Button v = {Vector2{17 + 160 + 2, 607 + 35 + 2 + 35 + 2 + 35 + 2 - 50}, 40, 35, (char *)"s ="};
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

void Graph::FixGraph()
{
    isFixed = true;
    UpdateGraph();
}

void Graph::UnFixGraph()
{
    isFixed = false;
    UpdateGraph();
}

void Graph::MinimumSpanningTree(int source)
{
    FixGraph();
    myPresentation = MinimumSpanningTreeAnimation(source);
    myPresentation.InitBoardText(MSTList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

void Graph::ShortestPath(int start, int _end)
{
    FixGraph();
    myPresentation = ShortestPathAnimation(start, _end);
    myPresentation.InitBoardText(PathList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

Presentation Graph::ShortestPathFromSourceAnimation(int source)
{
    Presentation shortestPath;
    shortestPath.present = {BasicStructure(nodes)};
    for (SetOfAnimation &curSet: shortestPath.present)
    {
        for (std::vector<NewAnimation> &vec: curSet.setOfAnimation)
        {
            for (NewAnimation &curAnimation: vec)
            {
                if (curAnimation.type == 9)
                {
                    curAnimation.color = BLACK;
                    curAnimation.listHighlights = {0};
                }
            }
        }
    }

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    int INF = 1e9 + 7;
    std::vector<int> dist(int(nodes.size()), INF);
    std::vector<int> lastWeight(int(nodes.size()), -1);
    std::vector<int> lastUpdate(int(nodes.size()), -1);

    shortestPath.CopySetToLast(-1);
    shortestPath.SetStartAnimation(-1, 1);
    shortestPath.CreateNewSet(-1);
    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));

    --source;
    dist[source] = 0;
    pq.emplace(0, source);

    while (pq.size()) 
    {
        shortestPath.CopySetToLast(-1);
        shortestPath.CreateNewSet(-1);
        shortestPath.SetStartAnimation(-1, 1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));

        int u = pq.top().second, du = pq.top().first; pq.pop();
        shortestPath.CreateNewSet(-1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {3}));
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, ORANGE, {nodes[u]}, 0, {3}));
        
        shortestPath.CreateNewSet(-1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {4}));

        if (dist[u] != du) {
            shortestPath.EraseAnimation(-1, -1, NewAnimation(2, 0, ORANGE, {nodes[u]}));
            continue;
        }

        shortestPath.CopySetToLast(-1);
        shortestPath.CreateNewSet(-1);
        shortestPath.SetStartAnimation(-1, 1);
        if (lastUpdate[u] != -1)
        {
            shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(11, 0, ORANGE, {nodes[lastUpdate[u]], nodes[u]}, lastWeight[u], {5}));
        }
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, ORANGE, {nodes[u]}, 0, {5}));

        shortestPath.CreateNewSet(-1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {6, 7}));


        shortestPath.CopySetToLast(-1);
        shortestPath.SetStartAnimation(-1, 1);
        shortestPath.CreateNewSet(-1);
        for (Edge &e: edges)
        {
            int from = e.u, to = e.v, weight = e.w;
            if (from == u)
            {
                if (dist[to] > dist[from] + weight)
                {
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(11, 0, PINK, {nodes[from], nodes[to]}, weight, {6, 7}));
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, PINK, {nodes[to]}, 0, {6, 7}));
                    dist[to] = dist[from] + weight;
                    if (lastUpdate[to] != -1)
                    {
                        shortestPath.EraseAnimation(-1, NewAnimation(11, 0, PINK, {nodes[lastUpdate[to]], nodes[to]}, lastWeight[to]));
                    }
                    lastUpdate[to] = from;
                    lastWeight[to] = weight;
                    pq.emplace(dist[to], to);
                }
            }
            std::swap(from, to);
            if (from == u)
            {
                if (dist[to] > dist[from] + weight)
                {
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(11, 0, PINK, {nodes[from], nodes[to]}, weight, {6, 7}));
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, PINK, {nodes[to]}, 0, {6, 7}));
                    dist[to] = dist[from] + weight;
                    if (lastUpdate[to] != -1)
                    {
                        shortestPath.EraseAnimation(-1, NewAnimation(11, 0, PINK, {nodes[lastUpdate[to]], nodes[to]}, lastWeight[to]));
                    }
                    lastUpdate[to] = from;
                    lastWeight[to] = weight;
                    pq.emplace(dist[to], to);
                }
            }
            std::swap(from, to);
        }
    }

    return shortestPath;
}

Presentation Graph::ShortestPathAnimation(int start, int _end)
{
    Presentation shortestPath;
    shortestPath.present = {BasicStructure(nodes)};
    for (SetOfAnimation &curSet: shortestPath.present)
    {
        for (std::vector<NewAnimation> &vec: curSet.setOfAnimation)
        {
            for (NewAnimation &curAnimation: vec)
            {
                if (curAnimation.type == 9)
                {
                    curAnimation.color = BLACK;
                    curAnimation.listHighlights = {0};
                }
            }
        }
    }

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    int INF = 1e9 + 7;
    std::vector<int> dist(int(nodes.size()), INF);
    std::vector<int> lastWeight(int(nodes.size()), -1);
    std::vector<int> lastUpdate(int(nodes.size()), -1);
    
    shortestPath.CopySetToLast(-1);
    shortestPath.SetStartAnimation(-1, 1);
    shortestPath.CreateNewSet(-1);
    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
    --start;
    --_end;
    dist[start] = 0;
    pq.emplace(0, start);
    while (pq.size()) 
    {
        shortestPath.CopySetToLast(-1);
        shortestPath.CreateNewSet(-1);
        shortestPath.SetStartAnimation(-1, 1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));
        int u = pq.top().second, du = pq.top().first; pq.pop();
        
        shortestPath.CreateNewSet(-1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {3}));
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, ORANGE, {nodes[u]}, 0, {3}));
        
        shortestPath.CreateNewSet(-1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {4}));

        if (dist[u] != du) 
        {
            shortestPath.EraseAnimation(-1, -1, NewAnimation(2, 0, ORANGE, {nodes[u]}));
            continue;
        }
        
        shortestPath.CopySetToLast(-1);
        shortestPath.CreateNewSet(-1);
        shortestPath.SetStartAnimation(-1, 1);
        if (lastUpdate[u] != -1)
        {
            shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(11, 0, ORANGE, {nodes[lastUpdate[u]], nodes[u]}, lastWeight[u], {5}));
        }
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, ORANGE, {nodes[u]}, 0, {5}));

        if (u == _end)
        {
            shortestPath.CopySetToLast(-1);
            shortestPath.SetStartAnimation(-1, 1);
            shortestPath.CreateNewSet(-1);
            while (u != start)
            {
                shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(11, 0, BLUE, {nodes[lastUpdate[u]], nodes[u]}, lastWeight[u], {6}));
                shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, BLUE, {nodes[u]}, 0, {6}));
                u = lastUpdate[u];
            }
            shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, BLUE, {nodes[u]}, 0, {6}));
            return shortestPath;
        }
        
        shortestPath.CreateNewSet(-1);
        shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {7, 8}));


        shortestPath.CopySetToLast(-1);
        shortestPath.SetStartAnimation(-1, 1);
        shortestPath.CreateNewSet(-1);
        for (Edge &e: edges)
        {
            int from = e.u, to = e.v, weight = e.w;
            if (from == u)
            {
                if (dist[to] > dist[from] + weight)
                {
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(11, 0, PINK, {nodes[from], nodes[to]}, weight, {7, 8}));
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, PINK, {nodes[to]}, 0, {7, 8}));
                    dist[to] = dist[from] + weight;
                    if (lastUpdate[to] != -1)
                    {
                        shortestPath.EraseAnimation(-1, NewAnimation(11, 0, PINK, {nodes[lastUpdate[to]], nodes[to]}, lastWeight[to]));
                    }
                    lastUpdate[to] = from;
                    lastWeight[to] = weight;
                    pq.emplace(dist[to], to);
                }
            }
            std::swap(from, to);
            if (from == u)
            {
                if (dist[to] > dist[from] + weight)
                {
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(11, 0, PINK, {nodes[from], nodes[to]}, weight, {7, 8}));
                    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, PINK, {nodes[to]}, 0, {7, 8}));
                    dist[to] = dist[from] + weight;
                    if (lastUpdate[to] != -1)
                    {
                        shortestPath.EraseAnimation(-1, NewAnimation(11, 0, PINK, {nodes[lastUpdate[to]], nodes[to]}, lastWeight[to]));
                    }
                    lastUpdate[to] = from;
                    lastWeight[to] = weight;
                    pq.emplace(dist[to], to);
                }
            }
            std::swap(from, to);
        }
    }

    shortestPath.CreateNewSet(-1);
    shortestPath.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {9}));

    return shortestPath;
}

Presentation Graph::MinimumSpanningTreeAnimation(int source)
{
    Presentation mst;
    mst.present = {BasicStructure(nodes)};
    for (SetOfAnimation &curSet: mst.present)
    {
        for (std::vector<NewAnimation> &vec: curSet.setOfAnimation)
        {
            for (NewAnimation &curAnimation: vec)
            {
                if (curAnimation.type == 9)
                {
                    curAnimation.color = Fade(isLightMode ? BLACK : WHITE, 0.2);
                    curAnimation.listHighlights = {0};
                }
            }
        }
    }

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    std::vector<int> lastUpdate(int(nodes.size()), -1);
    std::vector<int> lastWeight(int(nodes.size()), -1);
    int INF = 1e9 + 7;
    std::vector<int> dist(int(nodes.size()), INF);


    mst.CopySetToLast(-1);
    mst.CreateNewSet(-1);
    mst.SetStartAnimation(-1, 1);
    mst.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {1}));
    --source;
    dist[source] = 0;
    pq.emplace(0, source);

    while (pq.size())
    {
        mst.CopySetToLast(-1);
        mst.CreateNewSet(-1);
        mst.SetStartAnimation(-1, 1);
        mst.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));

        int u = pq.top().second, du = pq.top().first; pq.pop();
        mst.CreateNewSet(-1);
        mst.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {3}));

        if (du != dist[u]) continue;
        dist[u] = -INF; 
        mst.CopySetToLast(-1);
        mst.CreateNewSet(-1);
        mst.SetStartAnimation(-1, 1);
        if (lastUpdate[u] != -1)
        {
            mst.InsertAnimationToSet(-1, -1, NewAnimation(10, 0, ORANGE, {nodes[lastUpdate[u]], nodes[u]}, lastWeight[u], {4}));
        }
        mst.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, ORANGE, {nodes[u]}, 0, {4}));
        
        mst.CopySetToLast(-1);
        mst.SetStartAnimation(-1, 1);
        mst.CreateNewSet(-1);
        for (Edge &e: edges)
        {
            int from = e.u, to = e.v, weight = e.w;
            if (from == u)
            {
                if (dist[to] > -INF) 
                {
                    mst.InsertAnimationToSet(-1, -1, NewAnimation(9, 0, BLACK, {nodes[from], nodes[to]}, weight, {4}));
                    int f = from, t = to;
                    if (f > t) std::swap(f, t);
                    mst.EraseAnimation(-1, NewAnimation(9, 0, Fade(isLightMode ? BLACK : WHITE, 0.2), {nodes[f], nodes[t]}));
                }
                if (dist[to] >  weight)
                {
                    dist[to] = weight;
                    lastUpdate[to] = from;
                    lastWeight[to] = weight;
                    pq.emplace(dist[to], to);
                }
            }
            std::swap(from, to);
            if (from == u)
            {
                if (dist[to] > -INF) 
                {
                    mst.InsertAnimationToSet(-1, -1, NewAnimation(9, 0, BLACK, {nodes[from], nodes[to]}, weight, {4}));
                    int f = from, t = to;
                    if (f > t) std::swap(f, t);
                    mst.EraseAnimation(-1, NewAnimation(9, 0, Fade(isLightMode ? BLACK : WHITE, 0.2), {nodes[f], nodes[t]}));
                }
                if (dist[to] > weight)
                {
                    dist[to] = weight;
                    lastUpdate[to] = from;
                    lastWeight[to] = weight;
                    pq.emplace(dist[to], to);
                }
            }
            std::swap(from, to);
        }
    }

    mst.CopySetToLast(-1);
    mst.SetStartAnimation(-1, 1);

    mst.CreateNewSet(-1);
    mst.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {2}));

    mst.CreateNewSet(-1);
    mst.InsertAnimationToSet(-1, -1, NewAnimation(2, 0, Fade(BLACK, 0), {Node()}, 0, {5}));

    return mst;
}

void Graph::ShortestPathFromSource(int source)
{
    FixGraph();
    myPresentation = ShortestPathFromSourceAnimation(source);
    myPresentation.InitBoardText(sourceList, positionBoard, width, height);
    myPresentation.numberOfPresentation = 0;
    myPresentation.currentPresentation = 0;
    curRemoteState = 0;
}

std::vector<int> Graph::StringToVector(std::string listChar)
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
void Graph::InputDataFromKeyboard(std::vector<int> values )
{
    ClearAllData();
    UnFixGraph();  
    while(values.size()%3!=0)
    {
        values.pop_back() ; 
    }
    std::vector<int>dinh ; 
    for(int i= 0;i<values.size();i++)
    {
        if(i%3!=2)
        {
            dinh.push_back(values[i]) ; 
        }   
    }
    sort(dinh.begin(),dinh.end()) ; 
    dinh.resize(unique(dinh.begin(),dinh.end())-dinh.begin()) ; 
    std::cout<<dinh.size()<<std::endl; 
    for(auto x : dinh)std::cout<<x<<" "; 
    std::cout<<std::endl; 
    for(auto x : values)std::cout<<x<<" " ;
    std::cout<<std::endl; 
    // return ; 
    // return ; 
    int n = dinh.size() ; 
    nodes.reserve(n); 
    for(auto u : dinh)
    {
        float x = GetRandomValue(100, GetScreenWidth() - 100);
        float y = GetRandomValue(100, GetScreenHeight() - 100);
        nodes.emplace_back(Vector2{x, y}, NODE_RADIUS, std::to_string(u)); 
    }
    std::map<std::pair<int, int>,int> edgeMap;
    edges.clear();
    for (int i = 0; i+2 < values.size(); i+=3)
    {
        auto key = std::make_pair(std::min(values[i], values[i+1]), std::max(values[i], values[i+1]));
        edgeMap[key]=values[i+2];
    }
    for (std::pair<std::pair<int, int>, int> tmp: edgeMap) {
        edges.push_back({tmp.first.first-1, tmp.first.second-1, tmp.second});
    }
    UpdateGraph();
}
void Graph::HandleToolBar()
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
                        insertI.textBox = {Vector2{17 + 160 + 2 + 35, 607 - 50}, 60, 35};
                        insertI.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};

                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 - 50}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};
                    }
                    if (i == 3 && j == 0)
                    {
                        insertV.oldWidth = 60;
                        insertV.textBox = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 - 50}, 60, 35};
                        insertV.confirm = {Vector2{17 + 160 + 2 + 35, 607 + 35 + 2 + 35 + 2 - 50}, 60, 35, (char *)"Confirm"};
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
                values = StringToVector(listChar);
                InputDataFromKeyboard(values);
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
            // Insert(iValue, vValue);
    
            return;
        }
        if (flagToolBarButtons[3][0] == true) // Source
        {
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[3][0] = false;
            int vValue = StringToVector(v)[0];
            ShortestPathFromSource(vValue);
    
            return;
        }
        if (flagToolBarButtons[4][0] == true) // Path
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
            ShortestPath(iValue, vValue);

    
            return;
        }
        if (flagToolBarButtons[5][0] == true) // MST
        {
            std::string v = insertV.HandleTextBox();
            if (v.empty() == true) 
            {
                return;
            }
            flagToolBarButtons[5][0] = false;
            int vValue = StringToVector(v)[0];
            MinimumSpanningTree(vValue);
    
            return;
        }
    }
}

void Graph::Draw() 
{  
    if (isFixed == false)
    {
        ApplyForces();
        UpdateGraph();
    }
    myPresentation.DrawPresentation(hollowCircle, solidCircle, arrowEdge, fontNumber, robotoFont, isLightMode, speed, curRemoteState);
}

Vector2 Graph::f_rep(const Node& u, const Node& v) {
    Vector2 diff = Vector2Subtract(u.position, v.position);
    float dist = Vector2Length(diff);
    if (dist < NODE_RADIUS) dist = NODE_RADIUS;

    float forceMagnitude;
    if (dist > 2.5f * DESIRED_DISTANCE) {
        forceMagnitude = 0.0f; 
    } else {
        forceMagnitude = REPULSION_STRENGTH / (dist * dist);
    }

    return Vector2Scale(Vector2Normalize(diff), forceMagnitude);
}

Vector2 Graph::f_attr(const Node& u, const Node& v) {
    Vector2 diff = Vector2Subtract(v.position, u.position);
    float dist = Vector2Length(diff);

    float forceMagnitude = 0.0f;
    if (dist > DESIRED_DISTANCE) {
        forceMagnitude = ATTRACTION_STRENGTH * (dist - DESIRED_DISTANCE);
    }

    return Vector2Scale(Vector2Normalize(diff), forceMagnitude);
}

Vector2 Graph::f_center(const Node& node) {
    Vector2 center = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    Vector2 diff = Vector2Subtract(center, node.position);
    return Vector2Scale(diff, CENTER_PULL);
}

Vector2 Graph::LimitForce(Vector2 force, float maxForce) {
    float length = Vector2Length(force);
    if (length > maxForce) {
        return Vector2Scale(force, maxForce / length);
    }
    return force;
}

void Graph::ApplyForces() {
    std::vector<Vector2> forces(nodes.size(), {0, 0});

    for (size_t i = 0; i < nodes.size(); i++) {
        for (size_t j = i + 1; j < nodes.size(); j++) {
            Vector2 repulsion = f_rep(nodes[i], nodes[j]);
            forces[i] = Vector2Add(forces[i], repulsion);
            forces[j] = Vector2Subtract(forces[j], repulsion);
        }
    }

    for (const Edge& edge : edges) {
        Vector2 attraction = f_attr(nodes[edge.u], nodes[edge.v]);
        forces[edge.u] = Vector2Add(forces[edge.u], attraction);
        forces[edge.v] = Vector2Subtract(forces[edge.v], attraction);
    }

    for (size_t i = 0; i < nodes.size(); i++) {
        forces[i] = Vector2Add(forces[i], f_center(nodes[i]));
        forces[i] = LimitForce(forces[i], MAX_FORCE);
        nodes[i].velocity = Vector2Scale(Vector2Add(nodes[i].velocity, forces[i]), DAMPING);
        nodes[i].position = Vector2Add(nodes[i].position, nodes[i].velocity);

        // Clamp vị trí node trong màn hình
        float radius = nodes[i].radius;
        nodes[i].position.x = Clamp(nodes[i].position.x, radius, (float)1600 - radius);
        nodes[i].position.y = Clamp(nodes[i].position.y, radius, (float)800 - radius);
    }
}


void Graph::HandleDragging()
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        for (auto& node : nodes) {
            if (CheckCollisionPointCircle(mouse, {node.position.x, node.position.y}, NODE_RADIUS)) {
                node.dragging = true;
                node.position.x = mouse.x;
                node.position.y = mouse.y;
                node.velocity.x = 0;
                node.velocity.y = 0;
            }
        }
    } else {
        for (auto& node : nodes) {
            node.dragging = false;
        }
    }
}

void Graph::HandleRemote()
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

void Graph::DrawRemote()
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
