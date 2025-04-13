#pragma once
#include "Node.h"
#include <queue>
#include "DataStructure.h"

class Graph : public DataStructure
{
    public:
        const float NODE_RADIUS = 20.0f;     
        const float DESIRED_DISTANCE = 150.0f; 
        const float ATTRACTION_STRENGTH = 0.05f;
        const float REPULSION_STRENGTH = 5000.0f; 
        const float CENTER_PULL = 0.001f;      
        const float MAX_FORCE = 5.0f;        
        const float DAMPING = 0.9f;          

        struct Edge 
        {
            int u, v, w;
            Edge(int _u = 0, int _v = 0, int _w = 0)
            {
                u = _u;
                v = _v;
                w = _w;
            }
        };

        std::vector<Node> nodes;
        std::vector<Edge> edges;
        bool isDirected = false;
        bool isFixed = false;

        Font fontRoboto = LoadFont("../Fonts/Roboto_Condensed-SemiBold.ttf");
        ExtendedTextBox enterList;
        TextBox insertI;
        TextBox insertV;

        std::vector<int> values;
      
        void ClearAllData();
        Vector2 GenerateNodePositionSpread(int i, int n);
        void RandomNewData();
        void InputDataFromFile();
        void InputDataFromKeyboard(std::vector<int> values);
        void UpdateGraph();
        void FixGraph();
        void UnFixGraph();
        void MinimumSpanningTree(int source);
        void ShortestPath(int start, int _end);
        void ShortestPathFromSource(int source);
        std::vector<int> StringToVector(std::string listChar);

        SetOfAnimation BasicStructure(const std::vector<Node> &nodes);
        Presentation ShortestPathFromSourceAnimation(int source);
        Presentation ShortestPathAnimation(int start, int _end);
        Presentation MinimumSpanningTreeAnimation(int source);

        void Run();
        void Init();
        void Clear();

        Vector2 f_rep(const Node& u, const Node& v);
        Vector2 f_attr(const Node& u, const Node& v);
        Vector2 f_center(const Node& node);
        Vector2 LimitForce(Vector2 force, float maxForce);

        void ApplyForces();

        void HandleDragging();
                
        
        void Draw();
        void DrawRemote();
        void HandleRemote();
        void DrawToolBar();
        void HandleToolBar();


        std::vector<std::string> createList;
        std::vector<std::string> sourceList;
        std::vector<std::string> PathList;
        std::vector<std::string> MSTList;
        Vector2 positionBoard;
        float width, height;
};