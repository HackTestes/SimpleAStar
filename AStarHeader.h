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

    //template<typename Type>
    //long SlidingPuzzleHeuristic_h (Type current_sliding_puzzle_obj_position, Type goal_sliding_puzzle_obj_position);

    template<typename Type>
    long SlidingPuzzleHeuristic_h (Type current_sliding_puzzle_obj_position, Type goal_sliding_puzzle_obj_position)
    {
        long heuristic_total = 0;

        for (long i = 0; i < current_sliding_puzzle_obj_position.size(); ++i)
        {
            long unit_result_h = heuristic_h(current_sliding_puzzle_obj_position[i], goal_sliding_puzzle_obj_position[i]);

            heuristic_total =  heuristic_total + unit_result_h;
        }

        return heuristic_total;
    }


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

    /*//SlidingPuzzle.cpp
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
    std::vector<long> CreateVectorFromNeighbor(std::vector<long> current_sliding_puzzle, long neighbor_index, long empty_position_index);*/

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
    /*
    struct HashVector
    {
        size_t operator ()(const std::vector<long>& v) const;
    };*/

    template<typename Type>
    struct HashVector
    {
        size_t operator ()(const Type& v) const
        {
            return boost::hash_range(v.begin(), v.end());
        }
    };


    // Main functions
    //int mainSP(std::vector<long> local_start, std::vector<long> local_goal);
    int MainNodeMap(long local_start, long local_goal);












    template<typename Type>
    struct SlidingPuzzle
    {
        long map_key; // !todo! Itens de tamanhos variados
        Type sliding_puzzle;
        long empty_index; // empty cell index
        long g;
        Type came_from;
        bool visited;
        bool in_priority_queue; // otimização de velocidade em busca

        SlidingPuzzle(Type sliding_puzzle)
        {
            this->map_key = 0;
            this->sliding_puzzle = sliding_puzzle;
            this->empty_index = SlidingPuzzle::CalculateEmptyCell(sliding_puzzle);

            this->g = 0;
            this->visited = false;
            this->in_priority_queue = false;
            this->came_from = {0};
        }

        SlidingPuzzle()
        {
            this->map_key = 0;
            this->sliding_puzzle = sliding_puzzle;
            this->empty_index = SlidingPuzzle::CalculateEmptyCell(sliding_puzzle);

            this->g = 0;
            this->visited = false;
            this->in_priority_queue = false;
            this->came_from = {0};
        }

        bool operator==(const Type& v) const
        {
            return sliding_puzzle == v;
        }

        long CalculateEmptyCell(Type &current_map)
        {
            for(long position = 0; position < current_map.size(); ++position)
            {
                long cell_value = current_map[position];

                if(cell_value == 0)
                {
                    return position;
                }
            }
        }

        std::vector<long> CreateObjPositionList()
        {
            std::vector<long> obj_position_mapping (sliding_puzzle.size());

            for (long position = 0; position < sliding_puzzle.size(); ++position)
            {
                long obj = sliding_puzzle[position];
                obj_position_mapping[obj] = position;
            }

            return obj_position_mapping;
        }

        std::string PrintCurrentSlidingPuzzle(long cell_length, long cell_height)
        {
            // #WARNING# Perigoso! Variável global sendo modificada
            padding_cell_size = cell_length;

            long empty_cell_coordinate_x = Node::GetX(empty_index);
            long empty_cell_coordinate_y = Node::GetY(empty_index);

            std::string empty_symbol = " ";

            std::string sliding_puzzle_map;

            for(long y = 0; y < grid_size_y; ++y)
            {
                std::string grid_line = "";

                std::string cell_top_line = "";
                std::string cell_top_edge_symbol = ".";
                std::string line_symbol = "_";
                // Processo para construir uma linha de células


                // Linha superior ".________."
                for(long x = 0; x < grid_size_x; ++x)
                {
                    if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) )
                    {
                        cell_top_line += cell_top_edge_symbol;
                        for(long j = 1; j <= cell_length; ++j)
                        {
                            cell_top_line += line_symbol;
                        }
                        cell_top_line += cell_top_edge_symbol;
                    }

                    else
                    {
                        cell_top_line += empty_symbol;
                        for(long j = 1; j <= cell_length; ++j)
                        {
                            cell_top_line += empty_symbol;
                        }
                        cell_top_line += empty_symbol;
                    }
                }
                cell_top_line += "\n";
                grid_line += cell_top_line;


                // Meio "|    |"
                long middle = (cell_height/2) + 1;
                std::string left_right_symbol = "|";
                std::string cell_body_line = "";

                // (cell_height - 2) : subentendido o topo e o fim
                for (long j = 2; j <= (cell_height - 1); ++j)
                {
                    for(long x = 0; x < grid_size_x; ++x)
                    {
                        if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) ) {cell_body_line += left_right_symbol;}
                        else {cell_body_line += empty_symbol;}

                        if(j == middle)
                        {
                            std::string item;

                            //show_sliding_puzlle_coordinate
                            if(false)
                            {
                                item = "x:" + std::to_string(x) + " y:" + std::to_string(y);
                            }
                            else
                            {
                                long item_index = Node::GetIndex(x, y);
                                item = std::to_string( this->sliding_puzzle[item_index] );
                            }
                            std::string center = std::string((cell_length - item.length()) / 2, ' ') + item;

                            //cell_body_line += StringPadding( item.length() ) + item;
                            cell_body_line += center + StringPadding( center.length() );
                        }
                        else
                        {
                            cell_body_line += StringPadding(0);
                        }
                        if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) ) {cell_body_line += left_right_symbol;}
                        else {cell_body_line += empty_symbol;}
                    }
                    cell_body_line += "\n";
                    grid_line += cell_body_line;

                    cell_body_line = ""; //Reset
                }


                // Linha inferior "|_____________|"
                std::string bottom_line = "";

                for(long x = 0; x < grid_size_x; ++x)
                {
                    if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) )
                    {
                        bottom_line += left_right_symbol;
                        for(long j = 1; j <= cell_length; ++j)
                        {
                            bottom_line += line_symbol;
                        }
                        bottom_line += left_right_symbol;
                    }

                    else
                    {
                        bottom_line += empty_symbol;
                        for(long j = 1; j <= cell_length; ++j)
                        {
                            bottom_line += empty_symbol;
                        }
                        bottom_line += empty_symbol;
                    }
                }
                bottom_line += "\n";
                grid_line += bottom_line;

                sliding_puzzle_map = grid_line + sliding_puzzle_map;
            }

            //std::cout << sliding_puzzle_map;

            return sliding_puzzle_map;
        }

    };

    template<typename Type>
    Type CreateSlidingPuzzleFromNeighbors(Type current_sliding_puzzle, std::vector<long> neighbors_indexes, long empty_position_index)
    {
        std::vector< std::vector<long> > neighbors_keys ( neighbors_indexes.size() );

        for(long i = 0; i < neighbors_indexes.size(); ++i)
        {
            // copy the original vector
            neighbors_keys[i] = current_sliding_puzzle;

            long current_neighbor_index = neighbors_indexes[i];
            long neighbor_value = current_sliding_puzzle[current_neighbor_index];

            //Swap places with the neighbor
            neighbors_keys[i][empty_position_index] = neighbor_value;
            neighbors_keys[i][current_neighbor_index] = 0;
        }

        return neighbors_keys;
    }

    template<typename Type>
    Type CreateVectorFromNeighbor(Type current_sliding_puzzle, long neighbor_index, long empty_position_index)
    {
        // copy the original vector
        Type neighbor_sliding_puzzle = current_sliding_puzzle;
        long neighbor_value = current_sliding_puzzle[neighbor_index];

        //Swap places with the neighbor
        neighbor_sliding_puzzle[empty_position_index] = neighbor_value;
        neighbor_sliding_puzzle[neighbor_index] = 0;

        return neighbor_sliding_puzzle;
    }




    template<typename Type, typename ConvertToThisType>
    Type ConvertListItemType(std::vector<long> original_list)
    {
        Type new_converted_list;

        for(long i = 0; i < original_list.size(); ++i)
        {
            new_converted_list[i] = (ConvertToThisType)original_list[i];
        }

        return new_converted_list;
    }

    template<typename Type, typename ConvertToThisType>
    int mainSP(std::vector<long> global_start, std::vector<long> global_goal)
    {

        Type local_start;
        Type local_goal;

        local_start = ConvertListItemType<Type, ConvertToThisType>(global_start);

        local_goal = ConvertListItemType<Type, ConvertToThisType>(global_goal);

        std::unordered_map <Type, SlidingPuzzle<Type>, HashVector<Type>> my_map;

        // Verificação feita em outro lugar, criação do nó aqui
        my_map[local_start] = SlidingPuzzle<Type>(local_start); 

        my_map[local_goal] = SlidingPuzzle<Type>(local_goal);

        long previous_map_size = my_map.size();

        // usa o f do nó para ordenar
        std::priority_queue < PriorityQueueContainer<Type>, std::vector< PriorityQueueContainer<Type> >, SortPriorityQueue<Type> > OPEN;

        // inicializo todos os atributos necessários para começar
        my_map[local_start].in_priority_queue = true;
        OPEN.push( PriorityQueueContainer<Type>(0, local_start) );


        // inicio o loop principal
        while (OPEN.top().reference_key != local_goal)
        {
            Type current_sliding_puzzle = OPEN.top().reference_key;
            my_map[current_sliding_puzzle].in_priority_queue = false;
            OPEN.pop();

            my_map[current_sliding_puzzle].visited = true;

            long empty_pos_x = Node::GetX( my_map[current_sliding_puzzle].empty_index );
            long empty_pos_y = Node::GetY( my_map[current_sliding_puzzle].empty_index );

            std::vector<long> empty_cell_neighbors_list = ExpandNeighbors(empty_pos_x, empty_pos_y);

            //std::vector< std::vector<long> > sliding_puzzle_neighbors_list = CreateSlidingPuzzleFromNeighbors(current_sliding_puzzle, empty_cell_neighbors_list, my_map[current_sliding_puzzle].empty_index);

            std::string visited_neighbors = "";
            for (long neighbor_index : empty_cell_neighbors_list)
            {
                long neighbor_f = 0;
                bool path_executed_bool[4];
                path_executed_bool[0] = true;
                path_executed_bool[1] = false;
                path_executed_bool[2] = false;
                path_executed_bool[3] = false;

                Type neighbor_key = CreateVectorFromNeighbor(current_sliding_puzzle, neighbor_index, my_map[current_sliding_puzzle].empty_index);

                // se não for uma barreira
                if (true)
                {

                    // se não existir no meu mapa ainda, crie
                    if (!(my_map.find(neighbor_key) != my_map.end()))
                    {
                        my_map[neighbor_key] = SlidingPuzzle<Type>(neighbor_key);
                    }

                    long cost_so_far = my_map[current_sliding_puzzle].g + cost_g(my_map[current_sliding_puzzle].empty_index, neighbor_index);

                    // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                    // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                    // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                    // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
                    if (my_map[neighbor_key].in_priority_queue && (cost_so_far < my_map[neighbor_key].g))
                    {
                        neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h<Type>(neighbor_key, local_goal);
                        my_map[neighbor_key].g = cost_so_far;
                        my_map[neighbor_key].came_from = current_sliding_puzzle;
                        //OPEN = CopyPriorityQueueExcept(OPEN, neighbor_key); // removo o nó com valor antigo
                        OPEN.push( PriorityQueueContainer<Type>(neighbor_f, neighbor_key) ); // coloco de volta com o valor atualizado

                        path_executed_bool[0] = false;
                        path_executed_bool[1] = true;
                    }

                    if (my_map[neighbor_key].visited && (cost_so_far < my_map[neighbor_key].g))
                    {
                        my_map[neighbor_key].visited = false;

                        path_executed_bool[0] = false;
                        path_executed_bool[2] = true;
                    }

                    if (!my_map[neighbor_key].in_priority_queue && !my_map[neighbor_key].visited)
                    {
                        neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h<Type>(neighbor_key, local_goal);
                        my_map[neighbor_key].g = cost_so_far;
                        my_map[neighbor_key].came_from = current_sliding_puzzle;
                        my_map[neighbor_key].in_priority_queue = true;

                        OPEN.push( PriorityQueueContainer<Type>(neighbor_f, neighbor_key) );

                        path_executed_bool[0] = false;
                        path_executed_bool[3] = true;
                    }

                }
            }

            // (mapa não aumentou e PQ vazia) - condição de parada com barreiras
            if (previous_map_size == my_map.size() && OPEN.size() == 0)
            {
                std::cout << "The best path doesn't exist\n\n";
                //PrintMap(my_map, barrier);
                return 0;
            }

            previous_map_size = my_map.size(); // atualiza o valor de tamanho
        }

        std::cout << " BEST PATH\n\n\n";

        // mudo a "aparência" dos nós que pertencem ao melhor caminho
        // fazendo o caminho reverso do local_goal para o local_start
        Type index = local_goal;

        std::string best_path_output;

        while (true)
        {
            if (index == local_start)
            {
                best_path_output = "\n" + my_map[index].PrintCurrentSlidingPuzzle(10, 15) + "\n" + best_path_output;

                break;
            }
            else
            {

                best_path_output = "\n" + my_map[index].PrintCurrentSlidingPuzzle(10, 15) + "\n" + best_path_output;

                index = my_map[index].came_from;
            }
        }

        std::cout << best_path_output;

        return 0;
    }
#endif
