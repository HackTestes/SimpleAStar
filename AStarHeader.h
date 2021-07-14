#ifndef Astar_H
#define AStar_H

#include <queue>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>

    // Node.cpp
    class Node
    {
    public:
        long x;
        long y;
        long node_index;

        long f; // f(n) = g(n) + h(n)
        long g;

        Node *came_from;

        std::string appearance;

        Node(long x, long y, long node_index);
    };

    // FnFunctions.cpp
    long g (Node current, Node start);
    long h (Node current, Node goal);

    // FunctionsUtils.cpp
    class CustomComparator
    {
    public:
        bool operator() (Node *n1, Node *n2);
    };

    void ShowPriorityQueue (std::priority_queue < Node*, std::vector<Node*>, CustomComparator > priority_queue);

    bool CheckClosedList (Node *item);

    bool CheckOpenList (std::priority_queue < Node*, std::vector<Node*>, CustomComparator > priority_queue, Node *item);

    void ExpandNeighbors2 (Node *current_node, std::vector <long> *my_neighbors_coord, long grid_size_x, long grid_size_y);

    void PrintMap (std::vector <Node, std::allocator<Node>> map, long grid_size_x, long grid_size_y);

    void CreateNode (std::vector <Node, std::allocator<Node>> *map, long grid_size_x, long grid_size_y);

#endif
