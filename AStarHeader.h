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

    // PriorityQueueTemplates
    template<typename T>
    struct PriorityQueueContainer
    {
        long f; // f(n) = cost_g(n) + heuristic_h(n)
        T reference_key; //chave única que faz referência a um item: nó, Sliding Puzzle...

        PriorityQueueContainer(long f, T reference_key)
        {
            this->f = f;
            this->reference_key = reference_key;
        }
    };

    template<typename T>
    class SortPriorityQueue
    {
        public:
            bool operator() (PriorityQueueContainer<T> n1, PriorityQueueContainer<T> n2)
            {
                if (n1.f > n2.f)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
    };

    // FnFunctions.cpp
    long cost_g (long current_node_index, long neighbor_node_index);
    long heuristic_h (long current_node_index, long goal_node_index);

    long SlidingPuzzleHeuristic_h (std::vector<long> current_sliding_puzzle_obj_position, std::vector<long> goal_sliding_puzzle_obj_position);

    // FunctionsUtils.cpp
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
        std::vector<long> came_from;
        bool visited;
        bool in_priority_queue; // otimização de velocidade em busca

        SlidingPuzzle(std::vector<long> sliding_puzzle);
        SlidingPuzzle();

        bool operator==(const std::vector<long>& v) const;
        long CalculateEmptyCell(std::vector<long> &current_map);
        std::vector<long> CreateObjPositionList();
        std::string PrintCurrentSlidingPuzzle(long cell_length, long cell_height);
    };

    std::vector< std::vector<long> > CreateSlidingPuzzleFromNeighbors(std::vector<long> current_sliding_puzzle, std::vector<long> neighbors_indexes, long empty_position_index);
    std::vector<long> CreateVectorFromNeighbor(std::vector<long> current_sliding_puzzle, long neighbor_index, long empty_position_index);

    /*// PriorityQueue.cpp
    struct PriorityQueueContainer
    {
        long f; // f(n) = cost_g(n) + heuristic_h(n)
        std::vector<long> reference_key; //chave única que faz referência a um item: nó, Sliding Puzzle...

        PriorityQueueContainer(long f, std::vector<long> reference_key);
    };


    class SortPriorityQueue
    {
        public:
            bool operator() (PriorityQueueContainer n1, PriorityQueueContainer n2);
    };*/

    void ShowPriorityQueue (std::priority_queue < PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long> > priority_queue);

    std::priority_queue< PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long> > CopyPriorityQueueExcept (std::priority_queue <PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long>>priority_queue, long except_key);

    //CustomHashes.cpp
    struct HashVector
    {
        size_t operator ()(const std::vector<long>& v) const;
    };

    // Main functions
    int mainSP(std::vector<long> local_start, std::vector<long> local_goal);
    int MainNodeMap(long local_start, long local_goal);
#endif
