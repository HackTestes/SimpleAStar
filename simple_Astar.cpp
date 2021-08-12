
// !todo! Devo manter????
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include"AStarHeader.h"

// o mapa é uma tabela
// movimentos: cima, baixo, esquerda, direta (4 movimentos possíveis)


// Variáveis globais

// grid_size
long grid_size_x = 0;
long grid_size_y = 0;

bool debug = false;
bool best_path_index = false;
bool debug_all = false;
bool show_priority_queue = false;
bool show_visited_neighbors = false;

bool snapshot = false;
long snapshot_start_node_index = -1;
long snapshot_end_node_index = -1;

//bool snapshot_XY = false;
long snapshot_start_node_x = -1;
long snapshot_end_node_x = -1;
long snapshot_start_node_y = -1;
long snapshot_end_node_y = -1;

bool interactive = false;
bool show_map = false;
bool show_barrier = false;
bool barrier_enabled = false;

// map padding
long padding_cell_size = 7;

std::string barrier_file_path = "";

int main (int argc, char* argv[])
{
    switch (ArgsOptions(argc, argv))
    {
        case (0):
            break;

        case (1):
            return 1;

        case (2): // --help
            return 0;
    }

    // defino as variáveis principais : tamanho X e Y, começo (START), fim (GOAL)
    grid_size_x = std::stoi(argv[1]);

    grid_size_y = std::stoi(argv[2]);

    std::unordered_map <long, Node> my_map;

    long GOAL;
    if(std::stoi(argv[4]) >= 0 && std::stoi(argv[4]) < (grid_size_x * grid_size_y))
    {
        GOAL = std::stoi(argv[4]);
        my_map[GOAL] = Node(GOAL);
        my_map[GOAL].appearance = "G";
    }
    else
    {
        std::cout << "The set goal is out of bounds!!!\n";
        return 0;
    }

    long START;
    if(std::stoi(argv[3]) >= 0 && std::stoi(argv[3]) < (grid_size_x * grid_size_y))
    {
        START = std::stoi(argv[3]);
        my_map[START] = Node(START);
        my_map[START].appearance = "S";
    }
    else
    {
        std::cout << "The start is out of bounds!!!\n";
        return 0;
    }

    long previous_map_size = my_map.size();

    std::unordered_set<long> barrier;
    if (barrier_enabled)
    {
        if (ReadBarrier(&barrier) != 0)
        {
            return 1; // se não conseguir ler o arquivi, indique o erro e finaliza o programa
        }
    }

    // usa o f do nó para ordenar
    std::priority_queue < Node, std::vector<Node>, CustomComparator > OPEN;

    // inicializo todos os atributos necessários para começar
    my_map[START].in_priority_queue = true;
    OPEN.push(my_map[START]);

    // inicio o loop principal
    while (OPEN.top().node_index != my_map[GOAL].node_index)
    {
        long current_node_index = OPEN.top().node_index;
        my_map[current_node_index].in_priority_queue = false;
        OPEN.pop();

        my_map[current_node_index].visited = true;

        std::vector <long> my_neighbors_list;
        ExpandNeighbors(my_map[current_node_index], &my_neighbors_list);

        std::string visited_neighbors = "";
        for (long neighbor_index : my_neighbors_list)
        {
            std::string path_executed = "didn't execute any alternative path";// !todo! melhorar para demonstrar multiplos caminhos executados

            // se não for uma barreira
            if (!(barrier.find(neighbor_index) != barrier.end()))
            {

                // se não existir, crie
                if (!(my_map.find(neighbor_index) != my_map.end()))
                {
                    my_map[neighbor_index] = Node(neighbor_index);
                }

                long cost_so_far = my_map[current_node_index].g + g(my_map[current_node_index], my_map[neighbor_index]);

                // !done! verificar se um nó está na fila OPEN, pode ser feito com uma flag no nó
                // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                // Fazer isso retira a necessidade da função CheckOpenList() e a verificação não precisa iterar sobre a PQ (otimização de velocidade?)
                if (my_map[neighbor_index].in_priority_queue && (cost_so_far < my_map[neighbor_index].g))
                {
                    my_map[neighbor_index].f = cost_so_far + h(my_map[neighbor_index], my_map[GOAL]);
                    my_map[neighbor_index].g = cost_so_far;
                    my_map[neighbor_index].came_from = current_node_index;
                    OPEN = CopyPriorityQueueExcept(OPEN, neighbor_index); // removo o nó com valor antigo
                    OPEN.push(my_map[neighbor_index]); // coloco de volta com o valor atualizado
                    path_executed = "path 1, neighbor_in_open with better path";
                }

                if (my_map[neighbor_index].visited && (cost_so_far < my_map[neighbor_index].g))
                {
                    my_map[neighbor_index].visited = false;
                    path_executed = "path 2, neighbor_in_closed with better path";
                }

                if (!my_map[neighbor_index].in_priority_queue && !my_map[neighbor_index].visited)
                {
                    my_map[neighbor_index].f = cost_so_far + h(my_map[neighbor_index], my_map[GOAL]);
                    my_map[neighbor_index].g = cost_so_far;
                    my_map[neighbor_index].came_from = current_node_index;
                    my_map[neighbor_index].in_priority_queue = true;

                    OPEN.push(my_map[neighbor_index]);
                    path_executed = "path 3, wasn't in any list";
                }

                previous_map_size = my_map.size(); // atualiza o valor de tamanho para posterior avaliação

                if (my_map[neighbor_index].appearance != "S" && my_map[neighbor_index].appearance != "G") {my_map[neighbor_index].appearance = "X";} // visited but not expanded
                
                if (show_visited_neighbors)
                {
                    visited_neighbors += "Visited neighbor:\n"
                                         "  -  node_index " + std::to_string(my_map[neighbor_index].node_index) + "\n" +
                                         "  -  x " + std::to_string(my_map[neighbor_index].x) +
                                         "  y " + std::to_string(my_map[neighbor_index].y) +  "\n" +
                                         "  -  f " + std::to_string(my_map[neighbor_index].f) + "\n" +
                                         "  -  g " + std::to_string(my_map[neighbor_index].g) + "\n" +
                                         "  -  visited " + std::to_string(my_map[neighbor_index].visited) + "\n" +
                                         "  -  in_priority_queue " + std::to_string(my_map[neighbor_index].in_priority_queue) + "\n" +
                                         "  -  path_executed " + path_executed + "\n\n";
                }
            }
        }

        // Options - mostra uma série de informações sobre o loop principal
        if (snapshot)
        {
            if ( (current_node_index >= snapshot_start_node_index && current_node_index <= snapshot_end_node_index) || //usando index
                (my_map[current_node_index].x >= snapshot_start_node_x  && my_map[current_node_index].x <= snapshot_end_node_x && //usando as coordenadas
                my_map[current_node_index].y >= snapshot_start_node_y  && my_map[current_node_index].y <= snapshot_end_node_y) )
            {
                std::cout << "<<-------------------------End of loop iformation:------------------------->>\n\n";
                std::cout << "CURRENT NODE  : " << " node_index " << my_map[current_node_index].node_index
                                                << "  |  x " << my_map[current_node_index].x
                                                << "  y " << my_map[current_node_index].y
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

    std::cout << " BEST PATH FOUND!!!" << "\n" << " ... " << "\n\n";

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
                    my_map[index].appearance = "-";
                }
            }
            index = my_map[index].came_from;
            //std::cout << "came_from index  " << index << "\n";
        }
    }

    PrintMap(my_map, barrier);

    return 0;
}
