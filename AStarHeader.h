#ifndef Astar_H
#define AStar_H

#include <queue>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iomanip>
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

            long came_from;

            bool visited;

            std::string appearance;

            Node(long x, long y, long node_index);
            Node();
    };

    // boleanos e variáveis para argumentos
    extern bool debug;
    extern bool best_path_index;
    extern bool debug_all;
    extern bool show_priority_queue;
    extern bool show_closed_list;
    extern bool show_visited_neighbors;

    extern bool snapshot;
    extern long snapshot_start_node_index;
    extern long snapshot_end_node_index;

    //extern bool snapshot_XY;
    extern long snapshot_start_node_x;
    extern long snapshot_end_node_x;
    extern long snapshot_start_node_y;
    extern long snapshot_end_node_y;

    extern bool interactive;
    extern bool show_map;

    // FnFunctions.cpp
    long g (Node current, Node start);
    long h (Node current, Node goal);

    // FunctionsUtils.cpp

    void ArgsOptions(int argc, char* argv[]);

    class CustomComparator
    {
    public:
        bool operator() (Node *n1, Node *n2);
    };

    void ShowPriorityQueue (std::priority_queue < Node*, std::vector<Node*>, CustomComparator > priority_queue);

    void ShowClosedList (std::unordered_map< long, Node* > closed_list);

    bool CheckClosedList (Node *item);

    bool CheckOpenList (std::priority_queue < Node*, std::vector<Node*>, CustomComparator > priority_queue, Node *item);

    void ExpandNeighbors (Node *current_node, std::vector <long> *my_neighbors_coord, long grid_size_x, long grid_size_y);

    void PrintMap (std::vector <Node, std::allocator<Node>> map, long grid_size_x, long grid_size_y);

    void CreateNode (std::vector <Node, std::allocator<Node>> *map, long grid_size_x, long grid_size_y);

#endif
