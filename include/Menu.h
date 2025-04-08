#pragma once
#include "Node.h"
#include "NewAnimation.h"
#include "Button.h"
#include "raylib.h"
#include "SinglyLinkedList.h"
#include "AVL.h" 
#include "Heap.h"
#include "Trie.h"
#include "Graph.h"
class Menu
{
    private:
        Texture2D logo; 
        Texture2D homeIcon;
        Texture2D toDarkMode;
        Texture2D toLightMode; 

        Button *buttons;
        
        SinglyLinkedList myList;
        Graph myGraph;
        Heap myHeap;
        Trie myTrie;
        AVL myAVL;
        Color GetColorBackground();
        void DrawGradient(const bool isLightMode);
    public:
        int dataStructure;
        int isLightMode;
        
        void Run();
        void Init();
        void Clear();
        void DrawMenu();
        void HandleMenu();
};



