#ifndef Astar_H
#define AStar_H

    #include <queue>
    #include <vector>
    #include <unordered_map>
    #include <unordered_set>
    #include <iostream>
    #include <fstream>
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
            bool in_priority_queue;

            std::string appearance;

            Node(long node_index);
            Node();

            static long GetX(long node_index);
            static long GetY(long node_index);
            static long GetIndex(long x, long y);

            static bool VerifyCoordinate(long x, long y);
            static bool VerifyIndex(long node_index);
    };

    // grid_size
    extern long grid_size_x;
    extern long grid_size_y;

    extern long START;
    extern long GOAL;

    extern std::unordered_set<long> barrier;

    // boleanos e variáveis para argumentos
    extern bool debug;
    extern bool best_path_index;
    extern bool debug_all;
    extern bool show_priority_queue;
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
    extern bool show_barrier;
    extern bool warning_enabled;
    extern long heuristic_weight;
    extern long cost_weight;

    // map padding
    extern long padding_cell_size;

    extern std::string json_config_file_path;
    extern bool json_config_enabled;

    // FnFunctions.cpp
    long g (Node current, Node start);
    long h (Node current, Node goal);

    // FunctionsUtils.cpp
    //long ArgsOptions(int argc, char* argv[]);

    class CustomComparator
    {
        public:
            bool operator() (Node n1, Node n2);
    };

    void ShowPriorityQueue (std::priority_queue < Node, std::vector<Node>, CustomComparator > priority_queue);

    std::priority_queue < Node, std::vector<Node>, CustomComparator > CopyPriorityQueueExcept (std::priority_queue < Node, std::vector<Node>, CustomComparator > priority_queue,long except_index);

    void ExpandNeighbors (Node current_node, std::vector <long> *my_neighbors_coord);

    void PrintMap (std::unordered_map <long, Node> map, std::unordered_set<long> barrier_map);

    long ReadBarrier(std::unordered_set<long> *my_barrier);

    void ShowBarrier(std::unordered_set<long> my_barrier);

    class NodeParsed
    {
        public:
            long x;
            long y;
            long index;

        NodeParsed(long x, long y);
    };

    NodeParsed ParserXY(std::string string_coordinate, std::string separator);

    // FunctionsConfig.cpp
    void ArgsOptions(int argc, char* argv[]);
    void JsonConfig();
    void SetStart(long start_x, long start_y);
    void SetGoal(long goal_x, long goal_y);
    void SetGirdSizeX(long size_x);
    void SetGirdSizeY(long size_y);

#endif
