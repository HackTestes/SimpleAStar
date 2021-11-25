#ifndef MainSlidingPuzzle_H
#define MainSlidingPuzzle_H

    #include "CustomHashes.h"
    #include "SlidingPuzzle.h"
    #include "Neighbors.h"
    #include "FnFunctions.h"
    #include "OutputJSON.hpp"

/*
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <stdlib.h>
*/

    template<typename ListType, typename IntType>
    ListType ConvertListItemType(std::vector<long long> original_list)
    {
        ListType new_converted_list;

        for(long long i = 0; i < original_list.size(); ++i)
        {
            new_converted_list[i] = (IntType)original_list[i];
        }

        return new_converted_list;
    }

    template<typename ListType, typename IntType>
    int mainSP(std::vector<long long> global_start, std::vector<long long> global_goal)
    {
        //Stats
        long long expanded_nodes = 0;

        ListType local_start;
        ListType local_goal;

        local_start = ConvertListItemType<ListType, IntType>(global_start);

        local_goal = ConvertListItemType<ListType, IntType>(global_goal);

        std::unordered_map <ListType, SlidingPuzzle<ListType, IntType>, HashVector<ListType>> my_map;

        // Verificação feita em outro lugar, criação do nó aqui
        my_map[local_start] = SlidingPuzzle<ListType, IntType>(local_start); 

        my_map[local_goal] = SlidingPuzzle<ListType, IntType>(local_goal);

        IntType previous_map_size = my_map.size();

        // usa o f do nó para ordenar
        std::priority_queue < PriorityQueueContainer<ListType, IntType>, std::vector< PriorityQueueContainer<ListType, IntType> >, SortPriorityQueue<ListType, IntType> > OPEN;

        // inicializo todos os atributos necessários para começar
        my_map[local_start].in_priority_queue = true;
        OPEN.push( PriorityQueueContainer<ListType, IntType>(0, local_start) );


        // inicio o loop principal
        while (OPEN.top().reference_key != local_goal)
        {
            ListType current_sliding_puzzle = OPEN.top().reference_key;
            my_map[current_sliding_puzzle].in_priority_queue = false;
            OPEN.pop();
            ++expanded_nodes;

            my_map[current_sliding_puzzle].visited = true;

            IntType empty_pos_x = Node<IntType>::GetX( my_map[current_sliding_puzzle].empty_index );
            IntType empty_pos_y = Node<IntType>::GetY( my_map[current_sliding_puzzle].empty_index );

            std::vector<IntType> empty_cell_neighbors_list = ExpandNeighbors<IntType>(empty_pos_x, empty_pos_y);

            //std::vector< std::vector<long> > sliding_puzzle_neighbors_list = CreateSlidingPuzzleFromNeighbors(current_sliding_puzzle, empty_cell_neighbors_list, my_map[current_sliding_puzzle].empty_index);

            std::string visited_neighbors = "";
            IntType neighbor_f = 0;
            for (IntType neighbor_index : empty_cell_neighbors_list)
            {
                neighbor_f = 0;
                bool path_executed_bool[4];
                path_executed_bool[0] = true;
                path_executed_bool[1] = false;
                path_executed_bool[2] = false;
                path_executed_bool[3] = false;

                ListType neighbor_key = CreateVectorFromNeighbor<ListType, IntType>(current_sliding_puzzle, neighbor_index, my_map[current_sliding_puzzle].empty_index);

                // se não for uma barreira
                if (true)
                {

                    // se não existir no meu mapa ainda, crie
                    if (!(my_map.find(neighbor_key) != my_map.end()))
                    {
                        my_map[neighbor_key] = SlidingPuzzle<ListType, IntType>(neighbor_key);
                    }

                    uint64_t cost_so_far = my_map[current_sliding_puzzle].g + cost_g(my_map[current_sliding_puzzle].empty_index, neighbor_index);

                    // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                    // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                    // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                    // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
                    if (my_map[neighbor_key].in_priority_queue && (cost_so_far < my_map[neighbor_key].g))
                    {
                        neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h<ListType, IntType>(neighbor_key, local_goal);
                        my_map[neighbor_key].g = cost_so_far;
                        my_map[neighbor_key].came_from = current_sliding_puzzle;
                        //OPEN = CopyPriorityQueueExcept(OPEN, neighbor_key); // removo o nó com valor antigo
                        OPEN.push( PriorityQueueContainer<ListType, IntType>(neighbor_f, neighbor_key) ); // coloco de volta com o valor atualizado

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
                        neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h<ListType, IntType>(neighbor_key, local_goal);
                        my_map[neighbor_key].g = cost_so_far;
                        my_map[neighbor_key].came_from = current_sliding_puzzle;
                        my_map[neighbor_key].in_priority_queue = true;

                        OPEN.push( PriorityQueueContainer<ListType, IntType>(neighbor_f, neighbor_key) );

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

        // mudo a "aparência" dos nós que pertencem ao melhor caminho
        // fazendo o caminho reverso do local_goal para o local_start
        std::vector<ListType> compressed_best_path (my_map[local_goal].g);
        long long vec_pos = 0;

        ListType index = local_goal;

        std::string best_path_output;

        while (true)
        {
            if (index == local_start)
            {
                if(!json_output_enabled)
                {
                    best_path_output = "\n" + my_map[index].PrintCurrentSlidingPuzzle(10, 15) + "\n" + best_path_output;
                }

                break;
            }
            else
            {

                if(!json_output_enabled)
                {
                    best_path_output = "\n" + my_map[index].PrintCurrentSlidingPuzzle(10, 15) + "\n" + best_path_output;
                }

                compressed_best_path[vec_pos] = index;
                ++vec_pos;

                index = my_map[index].came_from;
            }
        }

        std::cout << best_path_output;

        if(json_output_enabled)
        {
            // Final Statistics
            std::cout << ShowJsonStatistics<ListType, IntType>(my_map.size(),
                                                    my_map[local_goal].g,
                                                    compressed_best_path,
                                                    expanded_nodes,
                                                    "Sliding Puzzles");
        }


        return 0;
    }

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
int mainSP(std::vector<long> local_start, std::vector<long> local_goal)
{
    std::unordered_map <std::vector<long>, SlidingPuzzle, HashVector> my_map;

    // Verificação feita em outro lugar, criação do nó aqui
    my_map[local_start] = SlidingPuzzle(local_start); 

    my_map[local_goal] = SlidingPuzzle(local_goal);

    long previous_map_size = my_map.size();

    // usa o f do nó para ordenar
    std::priority_queue < PriorityQueueContainer<std::vector<long>>, std::vector< PriorityQueueContainer<std::vector<long>> >, SortPriorityQueue<std::vector<long>> > OPEN;

    // inicializo todos os atributos necessários para começar
    my_map[local_start].in_priority_queue = true;
    OPEN.push( PriorityQueueContainer(0, local_start) );


    // inicio o loop principal
    while (OPEN.top().reference_key != local_goal)
    {
        std::vector<long> current_sliding_puzzle = OPEN.top().reference_key;
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

            std::vector<long> neighbor_key = CreateVectorFromNeighbor(current_sliding_puzzle, neighbor_index, my_map[current_sliding_puzzle].empty_index);

            // se não for uma barreira
            if (true)
            {

                // se não existir no meu mapa ainda, crie
                if (!(my_map.find(neighbor_key) != my_map.end()))
                {
                    my_map[neighbor_key] = SlidingPuzzle(neighbor_key);
                }

                long cost_so_far = my_map[current_sliding_puzzle].g + cost_g(my_map[current_sliding_puzzle].empty_index, neighbor_index);

                // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
                if (my_map[neighbor_key].in_priority_queue && (cost_so_far < my_map[neighbor_key].g))
                {
                    neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h(neighbor_key, local_goal);
                    my_map[neighbor_key].g = cost_so_far;
                    my_map[neighbor_key].came_from = current_sliding_puzzle;
                    //OPEN = CopyPriorityQueueExcept(OPEN, neighbor_key); // removo o nó com valor antigo
                    OPEN.push( PriorityQueueContainer(neighbor_f, neighbor_key) ); // coloco de volta com o valor atualizado

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
                    neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h(neighbor_key, local_goal);
                    my_map[neighbor_key].g = cost_so_far;
                    my_map[neighbor_key].came_from = current_sliding_puzzle;
                    my_map[neighbor_key].in_priority_queue = true;

                    OPEN.push( PriorityQueueContainer(neighbor_f, neighbor_key) );

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
    std::vector<long> index = local_goal;

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
*/
