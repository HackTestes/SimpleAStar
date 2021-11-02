#ifndef MAIN_NODE_MAP_H
#define MAIN_NODE_MAP_H

    #include <queue>
    #include <vector>
    #include <unordered_map>
    #include <unordered_set>
    #include <iostream>
    #include <fstream>
    #include <stdlib.h>

    #include "AStarHeader.h"
    #include "FnFunctions.h"
    #include "Neighbors.h"

    template<typename IndexType>
    int MainNodeMap(IndexType local_start, IndexType local_goal)
    {
        std::unordered_map<IndexType, Node<IndexType>> my_map;

        // Verificação feita em outro lugar, criação do nó aqui
        my_map[local_start] = Node<IndexType>(local_start); 
        my_map[local_start].appearance = "S";

        my_map[local_goal] = Node<IndexType>(local_goal);
        my_map[local_goal].appearance = "G";

        IndexType previous_map_size = my_map.size();

        // usa o f do nó para ordenar
        std::priority_queue < PriorityQueueContainer<IndexType, IndexType>, std::vector<PriorityQueueContainer<IndexType, IndexType>>, SortPriorityQueue<IndexType, IndexType> > OPEN;

        // inicializo todos os atributos necessários para começar
        my_map[local_start].in_priority_queue = true;
        OPEN.push( PriorityQueueContainer<IndexType, IndexType>(0, local_start) );


        // inicio o loop principal
        while (OPEN.top().reference_key != local_goal)
        {
            IndexType current_node_index = OPEN.top().reference_key;
            my_map[current_node_index].in_priority_queue = false;
            OPEN.pop();

            my_map[current_node_index].visited = true;

            std::vector <IndexType> my_neighbors_list;
            my_neighbors_list = ExpandNeighbors<IndexType>(Node<IndexType>::GetX(current_node_index), Node<IndexType>::GetY(current_node_index));

            std::string visited_neighbors = "";
            IndexType neighbor_f = 0;
            for (IndexType neighbor_index : my_neighbors_list)
            {
                neighbor_f = 0;
                bool path_executed_bool[4];
                path_executed_bool[0] = true;
                path_executed_bool[1] = false;
                path_executed_bool[2] = false;
                path_executed_bool[3] = false;

                // se não for uma barreira
                if (!(barrier.find(neighbor_index) != barrier.end()))
                {

                    // se não existir no meu mapa ainda, crie
                    if (!(my_map.find(neighbor_index) != my_map.end()))
                    {
                        my_map[neighbor_index] = Node<IndexType>(neighbor_index);
                    }

                    IndexType cost_so_far = my_map[current_node_index].g + cost_g<IndexType>(current_node_index, neighbor_index);

                    // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                    // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                    // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                    // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
                    if (my_map[neighbor_index].in_priority_queue && (cost_so_far < my_map[neighbor_index].g))
                    {
                        neighbor_f = cost_so_far + heuristic_h<IndexType>(neighbor_index, local_goal);
                        my_map[neighbor_index].g = cost_so_far;
                        my_map[neighbor_index].came_from = current_node_index;
                        OPEN = CopyPriorityQueueExcept<IndexType>(OPEN, neighbor_index); // removo o nó com valor antigo
                        OPEN.push( PriorityQueueContainer<IndexType, IndexType>(neighbor_f, neighbor_index) ); // coloco de volta com o valor atualizado

                        path_executed_bool[0] = false;
                        path_executed_bool[1] = true;
                    }

                    if (my_map[neighbor_index].visited && (cost_so_far < my_map[neighbor_index].g))
                    {
                        my_map[neighbor_index].visited = false;

                        path_executed_bool[0] = false;
                        path_executed_bool[2] = true;
                    }

                    if (!my_map[neighbor_index].in_priority_queue && !my_map[neighbor_index].visited)
                    {
                        neighbor_f = cost_so_far + heuristic_h<IndexType>(neighbor_index, local_goal);
                        my_map[neighbor_index].g = cost_so_far;
                        my_map[neighbor_index].came_from = current_node_index;
                        my_map[neighbor_index].in_priority_queue = true;

                        OPEN.push( PriorityQueueContainer<IndexType, IndexType>(neighbor_f, neighbor_index) );

                        path_executed_bool[0] = false;
                        path_executed_bool[3] = true;
                    }

                    if (my_map[neighbor_index].appearance != "S" && my_map[neighbor_index].appearance != "G") {my_map[neighbor_index].appearance = "X";} // visited but not expanded
                    
                    if (show_visited_neighbors)
                    {
                        visited_neighbors += "Visited neighbor:\n"
                                            "  -  node_index " + std::to_string(my_map[neighbor_index].node_index) + "\n" +
                                            "  -  x " + std::to_string( Node<IndexType>::GetX(neighbor_index) ) +
                                            "  y " + std::to_string( Node<IndexType>::GetY(neighbor_index) ) +  "\n" +
                                            "  -  f " + std::to_string(neighbor_f) + "\n" +
                                            "  -  g " + std::to_string(my_map[neighbor_index].g) + "\n" +
                                            "  -  visited " + std::to_string(my_map[neighbor_index].visited) + "\n" +
                                            "  -  in_priority_queue " + std::to_string(my_map[neighbor_index].in_priority_queue) + "\n" +
                                            "  -  path[0] = " + std::to_string(path_executed_bool[0]) +":  didn't execute any alternative path\n" +
                                            "  -  path[1] = " + std::to_string(path_executed_bool[1]) +":  neighbor_in_open with better path\n" +
                                            "  -  path[2] = " + std::to_string(path_executed_bool[2]) +":  neighbor_in_closed with better path\n" +
                                            "  -  path[3] = " + std::to_string(path_executed_bool[3]) +":  added to the open queue, bacause it wasn't in any list\n\n";

                    }
                }
            }

            //Options - mostra uma série de informações sobre o loop principal
            if (snapshot)
            {
                if ( (current_node_index >= snapshot_start_node_index && current_node_index <= snapshot_end_node_index) || //usando index
                    (Node<IndexType>::GetX(current_node_index) >= snapshot_start_node_x  && Node<IndexType>::GetX(current_node_index) <= snapshot_end_node_x && //usando as coordenadas
                    Node<IndexType>::GetY(current_node_index) >= snapshot_start_node_y  && Node<IndexType>::GetY(current_node_index) <= snapshot_end_node_y) )
                {
                    std::cout << "<<-------------------------End of loop iformation:------------------------->>\n\n";
                    std::cout << "CURRENT NODE  : " << " node_index " << my_map[current_node_index].node_index
                                                    << "  |  x " << Node<IndexType>::GetX(current_node_index)
                                                    << "  y " << Node<IndexType>::GetY(current_node_index)
                                                    << "  |  f " << neighbor_f
                                                    << "  |  g " << my_map[current_node_index].g
                                                    << "  |  visited " << my_map[current_node_index].visited
                                                    << "  |  in_priority_queue " << my_map[current_node_index].in_priority_queue << "\n\n";

                    if (show_priority_queue)
                    {
                        ShowPriorityQueue<IndexType>(OPEN);
                        std::cout << "\n";
                    }

                    if (show_visited_neighbors)
                    {
                        std::cout << visited_neighbors << "\n";
                    }

                    if (show_barrier)
                    {
                        ShowBarrier<long long>(barrier);
                        std::cout << "\n\n";
                    }

                    if (show_map)
                    {
                        PrintMap<IndexType>(my_map, barrier);
                    }

                    if (interactive)
                    {
                        std::cin.ignore();
                    }

                    std::cout << "<<------------------------------------------------------------------------->>\n";
                }
            }

            // (mapa não aumentou e PQ vazia) - condição de parada com barreiras
            if (previous_map_size == my_map.size() && OPEN.size() == 0)
            {
                std::cout << "The best path doesn't exist\n\n";

                PrintMap<IndexType>(my_map, barrier);
                return 0;
            }

            previous_map_size = my_map.size(); // atualiza o valor de tamanho
        }

        std::cout << " BEST PATH\n\n\n";

        // mudo a "aparência" dos nós que pertencem ao melhor caminho
        // fazendo o caminho reverso do local_goal para o local_start
        IndexType index = local_goal;
        while (true)
        {
            if (index == local_start)
            {
                break;
            }
            else
            {
                if (index != local_goal)
                {
                    if (best_path_index)
                    {
                        my_map[index].appearance =  std::to_string(index);
                    }
                    else
                    {
                        my_map[index].appearance = "@";
                    }
                }
                index = my_map[index].came_from;
            }
        }

        //PrintMap<IndexType>(my_map, barrier);

        return 0;
    }

#endif










//////////////////////////////////////////////////////////////////////////////
/*
int MainNodeMap(long local_start, long local_goal)
{
    std::unordered_map<long, Node> my_map;

    // Verificação feita em outro lugar, criação do nó aqui
    my_map[local_start] = Node(local_start); 
    my_map[local_start].appearance = "S";

    my_map[local_goal] = Node(local_goal);
    my_map[local_goal].appearance = "G";

    long previous_map_size = my_map.size();

    // usa o f do nó para ordenar
    std::priority_queue < PriorityQueueContainer<long>, std::vector<PriorityQueueContainer<long>>, SortPriorityQueue<long> > OPEN;

    // inicializo todos os atributos necessários para começar
    my_map[local_start].in_priority_queue = true;
    OPEN.push( PriorityQueueContainer<long>(0, local_start) );


    // inicio o loop principal
    while (OPEN.top().reference_key != local_goal)
    {
        long current_node_index = OPEN.top().reference_key;
        my_map[current_node_index].in_priority_queue = false;
        OPEN.pop();

        my_map[current_node_index].visited = true;

        std::vector <long> my_neighbors_list;
        my_neighbors_list = ExpandNeighbors(Node::GetX(current_node_index), Node::GetY(current_node_index));

        std::string visited_neighbors = "";
        long neighbor_f = 0;
        for (long neighbor_index : my_neighbors_list)
        {
            neighbor_f = 0;
            bool path_executed_bool[4];
            path_executed_bool[0] = true;
            path_executed_bool[1] = false;
            path_executed_bool[2] = false;
            path_executed_bool[3] = false;

            // se não for uma barreira
            if (!(barrier.find(neighbor_index) != barrier.end()))
            {

                // se não existir no meu mapa ainda, crie
                if (!(my_map.find(neighbor_index) != my_map.end()))
                {
                    my_map[neighbor_index] = Node(neighbor_index);
                }

                long cost_so_far = my_map[current_node_index].g + cost_g(current_node_index, neighbor_index);

                // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
                if (my_map[neighbor_index].in_priority_queue && (cost_so_far < my_map[neighbor_index].g))
                {
                    neighbor_f = cost_so_far + heuristic_h(neighbor_index, local_goal);
                    my_map[neighbor_index].g = cost_so_far;
                    my_map[neighbor_index].came_from = current_node_index;
                    OPEN = CopyPriorityQueueExcept(OPEN, neighbor_index); // removo o nó com valor antigo
                    OPEN.push( PriorityQueueContainer<long>(neighbor_f, neighbor_index) ); // coloco de volta com o valor atualizado

                    path_executed_bool[0] = false;
                    path_executed_bool[1] = true;
                }

                if (my_map[neighbor_index].visited && (cost_so_far < my_map[neighbor_index].g))
                {
                    my_map[neighbor_index].visited = false;

                    path_executed_bool[0] = false;
                    path_executed_bool[2] = true;
                }

                if (!my_map[neighbor_index].in_priority_queue && !my_map[neighbor_index].visited)
                {
                    neighbor_f = cost_so_far + heuristic_h(neighbor_index, local_goal);
                    my_map[neighbor_index].g = cost_so_far;
                    my_map[neighbor_index].came_from = current_node_index;
                    my_map[neighbor_index].in_priority_queue = true;

                    OPEN.push( PriorityQueueContainer<long>(neighbor_f, neighbor_index) );

                    path_executed_bool[0] = false;
                    path_executed_bool[3] = true;
                }

                if (my_map[neighbor_index].appearance != "S" && my_map[neighbor_index].appearance != "G") {my_map[neighbor_index].appearance = "X";} // visited but not expanded
                
                if (show_visited_neighbors)
                {
                    visited_neighbors += "Visited neighbor:\n"
                                         "  -  node_index " + std::to_string(my_map[neighbor_index].node_index) + "\n" +
                                         "  -  x " + std::to_string( Node::GetX(neighbor_index) ) +
                                         "  y " + std::to_string( Node::GetY(neighbor_index) ) +  "\n" +
                                         "  -  f " + std::to_string(neighbor_f) + "\n" +
                                         "  -  g " + std::to_string(my_map[neighbor_index].g) + "\n" +
                                         "  -  visited " + std::to_string(my_map[neighbor_index].visited) + "\n" +
                                         "  -  in_priority_queue " + std::to_string(my_map[neighbor_index].in_priority_queue) + "\n" +
                                         "  -  path[0] = " + std::to_string(path_executed_bool[0]) +":  didn't execute any alternative path\n" +
                                         "  -  path[1] = " + std::to_string(path_executed_bool[1]) +":  neighbor_in_open with better path\n" +
                                         "  -  path[2] = " + std::to_string(path_executed_bool[2]) +":  neighbor_in_closed with better path\n" +
                                         "  -  path[3] = " + std::to_string(path_executed_bool[3]) +":  added to the open queue, bacause it wasn't in any list\n\n";

                }
            }
        }

        //Options - mostra uma série de informações sobre o loop principal
        if (snapshot)
        {
            if ( (current_node_index >= snapshot_start_node_index && current_node_index <= snapshot_end_node_index) || //usando index
                (Node::GetX(current_node_index) >= snapshot_start_node_x  && Node::GetX(current_node_index) <= snapshot_end_node_x && //usando as coordenadas
                Node::GetY(current_node_index) >= snapshot_start_node_y  && Node::GetY(current_node_index) <= snapshot_end_node_y) )
            {
                std::cout << "<<-------------------------End of loop iformation:------------------------->>\n\n";
                std::cout << "CURRENT NODE  : " << " node_index " << my_map[current_node_index].node_index
                                                << "  |  x " << Node::GetX(current_node_index)
                                                << "  y " << Node::GetY(current_node_index)
                                                << "  |  f " << neighbor_f
                                                << "  |  g " << my_map[current_node_index].g
                                                << "  |  visited " << my_map[current_node_index].visited
                                                << "  |  in_priority_queue " << my_map[current_node_index].in_priority_queue << "\n\n";

                if (show_priority_queue)
                {
                    ShowPriorityQueue(OPEN);
                    std::cout << "\n";
                }

                if (show_visited_neighbors)
                {
                    std::cout << visited_neighbors << "\n";
                }

                if (show_barrier)
                {
                    ShowBarrier(barrier);
                    std::cout << "\n\n";
                }

                if (show_map)
                {
                    PrintMap(my_map, barrier);
                }

                if (interactive)
                {
                    std::cin.ignore();
                }

                std::cout << "<<------------------------------------------------------------------------->>\n";
            }
        }

        // (mapa não aumentou e PQ vazia) - condição de parada com barreiras
        if (previous_map_size == my_map.size() && OPEN.size() == 0)
        {
            std::cout << "The best path doesn't exist\n\n";

            PrintMap(my_map, barrier);
            return 0;
        }

        previous_map_size = my_map.size(); // atualiza o valor de tamanho
    }

    std::cout << " BEST PATH\n\n\n";

    // mudo a "aparência" dos nós que pertencem ao melhor caminho
    // fazendo o caminho reverso do local_goal para o local_start
    long index = local_goal;
    while (true)
    {
        if (index == local_start)
        {
            break;
        }
        else
        {
            if (index != local_goal)
            {
                if (best_path_index)
                {
                    my_map[index].appearance =  std::to_string(index);
                }
                else
                {
                    my_map[index].appearance = "@";
                }
            }
            index = my_map[index].came_from;
        }
    }

    PrintMap(my_map, barrier);

    return 0;
}
*/