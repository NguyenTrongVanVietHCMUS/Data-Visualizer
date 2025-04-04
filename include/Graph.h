#pragma once
#include "Node.h"
#include "DataStructure.h"

class Graph : public DataStructure
{
    public:
        const float NODE_RADIUS = 20.0f;     
        const float DESIRED_DISTANCE = 180.0f;
        const float REPULSION_STRENGTH = 30000.0f; 
        const float ATTRACTION_STRENGTH = 0.005f; 
        const float CENTER_PULL = 0.002f;      
        const float MAX_FORCE = 10.0f;        
        const float DAMPING = 0.85f;          

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
        TextBox enterList;
        TextBox insertI;
        TextBox insertV;

        std::vector<int> values;
      
        void ClearAllData();
        void RandomNewData();
        void InputDataFromFile();
        void UpdateGraph();
        void Insert(int pos, int val);
        void Delete(int val);
        void Search(int val);
        void Update(int pos, int val);
        std::vector<int> StringToVector(std::string listChar);

        Presentation BasicStructure(const std::vector<Node> &nodes);
        Presentation InsertAnimation(int pos, int val);
        Presentation UpdateAnimation(int pos, int val);
        Presentation DeleteAnimation(int val);
        Presentation SearchAnimation(int val, Color color);

        void Run();
        void Init();
        void Clear();

        Vector2 f_rep(const Node& u, const Node& v);
        Vector2 f_attr(const Node& u, const Node& v);
        Vector2 f_center(const Node& node);
        Vector2 LimitForce(Vector2 force, float maxForce);

        void ApplyForces();

        void HandleDragging();
                
        
        void DrawToolBar();
        void HandleToolBar();
        
        
        
        

        void Draw();
};