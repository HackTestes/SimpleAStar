#ifndef ASTAR_HEADER_H
#define ASTAR_HEADER_H

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

    //Json
    #include "JsonInputStringFormat.hpp"
    extern JsonInputStringFormat json_input_string_values;

    // problem type
    extern bool node_map_enabled;
    extern bool sliding_puzzle_enabled;

    // grid_size
    extern long long grid_size_x;
    extern long long grid_size_y;

    extern long long START;
    extern long long GOAL;

    extern std::unordered_set<long long> barrier;

    // boleanos e variáveis para argumentos
    extern bool debug;
    extern bool best_path_index;
    extern bool debug_all;
    extern bool show_priority_queue;
    extern bool show_visited_neighbors;

    extern bool snapshot;
    extern long long snapshot_start_node_index;
    extern long long snapshot_end_node_index;

    //extern bool snapshot_XY;
    extern long long snapshot_start_node_x;
    extern long long snapshot_end_node_x;
    extern long long snapshot_start_node_y;
    extern long long snapshot_end_node_y;

    extern bool interactive;
    extern bool show_map;
    extern bool show_barrier;
    extern bool warning_enabled;
    extern long long heuristic_weight;
    extern long long cost_weight;

    // map padding
    extern long long padding_cell_size;

    extern std::string json_config_file_path;
    extern bool json_config_enabled;

    // SlidingPuzzle
    extern std::vector<long long> sliding_puzzle_goal;
    extern std::vector<long long> sliding_puzzle_start;

    //Output
    extern bool json_output_enabled;

    std::string StringPadding(long string_length);

    #include "Node.h"

    #include "PriorityQueue.h"

    // FunctionsConfig.cpp
    void ArgsOptions(int argc, char* argv[]);
    void JsonConfig();
    void SetStart(long long start_x, long long start_y);
    void SetGoal(long long goal_x, long long goal_y);
    void SetGirdSizeX(long long size_x);
    void SetGirdSizeY(long long size_y);

    int StartNodeMapImplementations();
    int StartSlidingPuzzleImplementations();

#endif
