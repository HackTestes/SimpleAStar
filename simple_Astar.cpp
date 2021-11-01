
// !todo! Devo manter????
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "AStarHeader.h"
#include "MainSlidingPuzzle.h"
#include "MainNodeMap.h"

// o mapa é uma tabela
// movimentos: cima, baixo, esquerda, direta (4 movimentos possíveis)

// !todo! GlobalParameters.cpp ????? Ou .h?
// Variáveis globais

// problem type
bool node_map_enabled = true;
bool sliding_puzzle_enabled = false;

// grid_size
long grid_size_x = 0;
long grid_size_y = 0;

long START = 0;
long GOAL = 0;

std::unordered_set<long> barrier = {};

bool debug = false;
bool best_path_index = false;
bool debug_all = false;
bool show_priority_queue = false;
bool show_visited_neighbors = false;

bool snapshot = false;
long snapshot_start_node_index = -1;
long snapshot_end_node_index = -1;

long snapshot_start_node_x = -1;
long snapshot_end_node_x = -1;
long snapshot_start_node_y = -1;
long snapshot_end_node_y = -1;

bool interactive = false;
bool show_map = false;
bool show_barrier = false;
bool warning_enabled = false;
long heuristic_weight = 1;
long cost_weight = 1;

// map padding
long padding_cell_size = 7;

std::string json_config_file_path = "";
bool json_config_enabled = false;

// SlidingPuzzle
std::vector<long> sliding_puzzle_goal = {};
std::vector<long> sliding_puzzle_start = {};

int main (int argc, char* argv[])
{

    ArgsOptions(argc, argv);

    if (node_map_enabled)
    {
        return MainNodeMap<long>(START, GOAL);
    }

    if(sliding_puzzle_enabled)
    {
        //Sliding Puzzle 3x3
        if(sliding_puzzle_goal.size() == 9)
        {
            return mainSP< std::array<uint8_t, 9>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);
        }

        //Sliding Puzzle 4x4
        if(sliding_puzzle_goal.size() == 16)
        {
            return mainSP< std::array<uint8_t, 16>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);
        }

        //Sliding Puzzle 5x5
        if(sliding_puzzle_goal.size() == 25)
        {
            return mainSP< std::array<uint8_t, 25>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);
        }

        //Sliding Puzzle 6x6
        if(sliding_puzzle_goal.size() == 36)
        {
            return mainSP< std::array<uint8_t, 36>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);
        }
    }
}


/*
    std::unordered_map <long, Node> my_map;

    // Verificação feita em outro lugar, criação do nó aqui
    my_map[START] = Node(START); 
    my_map[START].appearance = "S";

    my_map[GOAL] = Node(GOAL);
    my_map[GOAL].appearance = "G";

    long previous_map_size = my_map.size();

    // usa o f do nó para ordenar
    std::priority_queue < PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue > OPEN;

    // inicializo todos os atributos necessários para começar
    my_map[START].in_priority_queue = true;
    OPEN.push( PriorityQueueContainer(my_map[START].f, START) );

    MainLoop(my_map, OPEN, previous_map_size);

    // !todo! Retirar trecho
    // inicio o loop principal
    /*while (OPEN.top().reference_key != GOAL)
    {
        long current_node_index = OPEN.top().reference_key;
        my_map[current_node_index].in_priority_queue = false;
        OPEN.pop();

        my_map[current_node_index].visited = true;

        std::vector <long> my_neighbors_list;
        ExpandNeighbors(Node::GetX(current_node_index), Node::GetY(current_node_index), &my_neighbors_list);

        std::string visited_neighbors = "";
        for (long neighbor_index : my_neighbors_list)
        {
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
                    my_map[neighbor_index].f = cost_so_far + heuristic_h(neighbor_index, GOAL);
                    my_map[neighbor_index].g = cost_so_far;
                    my_map[neighbor_index].came_from = current_node_index;
                    OPEN = CopyPriorityQueueExcept(OPEN, neighbor_index); // removo o nó com valor antigo
                    OPEN.push( PriorityQueueContainer(my_map[neighbor_index].f, neighbor_index) ); // coloco de volta com o valor atualizado

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
                    my_map[neighbor_index].f = cost_so_far + heuristic_h(neighbor_index, GOAL);
                    my_map[neighbor_index].g = cost_so_far;
                    my_map[neighbor_index].came_from = current_node_index;
                    my_map[neighbor_index].in_priority_queue = true;

                    OPEN.push( PriorityQueueContainer(my_map[neighbor_index].f, neighbor_index) );

                    path_executed_bool[0] = false;
                    path_executed_bool[3] = true;
                }

                previous_map_size = my_map.size(); // atualiza o valor de tamanho para posterior avaliação

                if (my_map[neighbor_index].appearance != "S" && my_map[neighbor_index].appearance != "G") {my_map[neighbor_index].appearance = "X";} // visited but not expanded
                
                if (show_visited_neighbors)
                {
                    visited_neighbors += "Visited neighbor:\n"
                                         "  -  node_index " + std::to_string(my_map[neighbor_index].node_index) + "\n" +
                                         "  -  x " + std::to_string( Node::GetX(neighbor_index) ) +
                                         "  y " + std::to_string( Node::GetY(neighbor_index) ) +  "\n" +
                                         "  -  f " + std::to_string(my_map[neighbor_index].f) + "\n" +
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


        SnapshotOptions(current_node_index, OPEN, visited_neighbors, my_map);
        // !todo! Retirar trecho
        /* Options - mostra uma série de informações sobre o loop principal
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
                                                << "  |  f " << my_map[current_node_index].f
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
    }

    std::cout << " BEST PATH\n\n\n";


    // !todo! Retirar trecho
    // mudo a "aparência" dos nós que pertencem ao melhor caminho
    // fazendo o caminho reverso do GOAL para o START
    long index = GOAL;
    while (true)
    {
        if (index == START)
        {
            break;
        }
        else
        {
            if (index != GOAL)
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

    ReversePath(my_map);

    PrintMap(my_map, barrier);

    return 0;
}*/