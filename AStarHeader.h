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

    //boost
    #include <boost/range/iterator_range_core.hpp>
    #include <boost/functional/hash.hpp>

    // Node.cpp
    class Node
    {
        public:
            long node_index;

            long f; // f(n) = cost_g(n) + heuristic_h(n)
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

    // problem type
    extern bool node_map_enabled;
    extern bool sliding_puzzle_enabled;

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

    // SlidingPuzzle
    extern std::vector<long> sliding_puzzle_goal;
    extern std::vector<long> sliding_puzzle_start;

    // FnFunctions.cpp
    long cost_g (long current_node_index, long neighbor_node_index);
    long heuristic_h (long current_node_index, long goal_node_index);

    long SlidingPuzzleHeuristic_h (std::vector<long> current_sliding_puzzle_obj_position, std::vector<long> goal_sliding_puzzle_obj_position);

    /*
    // FunctionsUtils.cpp
    class SortPriorityQueue
    {
        public:
            bool operator() (Node n1, Node n2);
    };

    void ShowPriorityQueue (std::priority_queue < Node, std::vector<Node>, SortPriorityQueue > priority_queue);

    std::priority_queue < Node, std::vector<Node>, SortPriorityQueue > CopyPriorityQueueExcept (std::priority_queue < Node, std::vector<Node>, SortPriorityQueue > priority_queue,long except_index);

    */

    std::vector<long> ExpandNeighbors (long current_node_x, long current_node_y);

    std::string StringPadding(long string_length);

    void PrintMap (std::unordered_map <long, Node> map, std::unordered_set<long> barrier_map);

    long ReadBarrier(std::unordered_set<long> *my_barrier);

    void ShowBarrier(std::unordered_set<long> my_barrier);

    std::pair<long, long> CoordinateParser(std::string string_coordinate, std::string separator);

    class ParsedNode
    {
        public:
            long x;
            long y;
            long index;

        ParsedNode(long x, long y);
    };

    ParsedNode ParserXY(std::string string_coordinate, std::string separator);

    // FunctionsConfig.cpp
    void ArgsOptions(int argc, char* argv[]);
    void JsonConfig();
    void SetStart(long start_x, long start_y);
    void SetGoal(long goal_x, long goal_y);
    void SetGirdSizeX(long size_x);
    void SetGirdSizeY(long size_y);

    //SlidingPuzzle.cpp
    struct SlidingPuzzle
    {
        long map_key; // !todo! Itens de tamanhos variados
        std::vector<long> sliding_puzzle;
        long empty_index; // empty cell index
        long g;
        long came_from;
        bool visited;
        bool in_priority_queue; // otimização de velocidade em busca

        SlidingPuzzle(std::vector<long> sliding_puzzle);
        SlidingPuzzle();

        bool operator==(const std::vector<long>& v) const;
        long CalculateEmptyCell(std::vector<long> &current_map);
        std::vector<long> CreateObjPositionList();
        std::string PrintCurrentSlidingPuzzle(long cell_length, long cell_height);
    };

    // PriorityQueue.cpp
    struct PriorityQueueContainer
    {
        long f; // f(n) = cost_g(n) + heuristic_h(n)
        long reference_key; //chave única que faz referência a um item: nó, Sliding Puzzle...

        PriorityQueueContainer(long f, long reference_key);
    };


    class SortPriorityQueue
    {
        public:
            bool operator() (PriorityQueueContainer n1, PriorityQueueContainer n2);
    };

    void ShowPriorityQueue (std::priority_queue < PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue > priority_queue);

    std::priority_queue< PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue > CopyPriorityQueueExcept (std::priority_queue <PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue>priority_queue, long except_key);

    // MainFunctions.cpp
    void ReversePath(std::unordered_map<long, Node> &map_reverse);

    void SnapshotOptions(long current_key,
                        std::priority_queue < PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue > current_priority_queue,
                        std::string neighbors,
                        std::unordered_map<long, Node> &current_map);

    long MainLoop(std::unordered_map<long, Node> &my_map, std::priority_queue<PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue> &OPEN, long previous_map_size);

    long StartMain();

    //CustomHashes.cpp
    struct HashVector
    {
        size_t operator ()(const std::vector<long>& v) const;// noexcept
    };

#endif
