#ifndef MainSlidingPuzzle_H
#define MainSlidingPuzzle_H

#include "CustomHashes.h"
#include "SlidingPuzzle.h"

/*
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <stdlib.h>
*/

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
