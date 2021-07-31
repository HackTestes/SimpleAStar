
// Devo manter????
// #include <queue>
// #include <vector>
// #include <unordered_map>
// #include <iostream>
// #include <stdlib.h>


#include"AStarHeader.h"

// o mapa é uma tabela
// movimentos: cima, baixo, esquerda, direta (4 movimentos possíveis)

/*

#ERROS:
- OPEN e CLOSED apontam para os nós do map (um vetor!)
    .aparentemente o OPEN e CLOSED apontando (com ponteiros) para um vetor (que não garante linearidade e pode invalidar ponteiros mudando
    os itens de lugar ao fazer um resize) pode causar comportamento indefinido!!!
    .pode estar csando outros problemas.
    #talvez dê para somente usar uma array ao invés de um vetor

- Mapas com lados de diferentes tamanhos (x!=y) geram erros (loops infinitos ou segmentation fault)
    .exemplo: ./SimpleAStarExecutabele 9999 10 0 5 --SnapshotXY 0-10 0-10 --BestPathIndex --ShowPriorityQueue --ShowClosedList

*/

// Variáveis globais
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

int main (int argc, char* argv[])
{
    ArgsOptions(argc, argv);

    // defino as variáveis principais : tamanho X e Y, começo (START), fim (GOAL)

    long grid_size_x = std::stoi(argv[1]);

    long grid_size_y = std::stoi(argv[2]);

    std::vector <Node, std::allocator<Node>> my_map;
    CreateNode (&my_map, grid_size_x, grid_size_y);

    long GOAL;
    if(std::stoi(argv[4]) >= 0 && std::stoi(argv[4]) < (grid_size_x * grid_size_y))
    {
        GOAL = std::stoi(argv[4]);
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
        my_map[START].appearance = "S";
    }
    else
    {
        std::cout << "The start is out of bounds!!!\n";
        return 0;
    }

    // usa o f do nó para ordenar
    std::priority_queue < Node, std::vector<Node>, CustomComparator > OPEN;

    my_map[START].f = g(my_map[START], my_map[START]) + h(my_map[START], my_map[GOAL]);

    OPEN.push(my_map[START]);

    std::cout << "PONTO DE CONTROLE  " << 1 << "\n";

    // inicio o loop principal
    while (OPEN.top().node_index != my_map[GOAL].node_index)
    {
        long current_node_index = OPEN.top().node_index;//(OPEN.top()->x * grid_size_x) + OPEN.top()->y; // !todo! substituir por top()->index
        OPEN.pop();

        my_map[current_node_index].visited = true;

        std::vector <long> my_neighbors_list;
        ExpandNeighbors(&my_map[current_node_index], &my_neighbors_list, grid_size_x, grid_size_y);

        std::string visited_neighbors = "";
        for (long neighbor_index : my_neighbors_list)
        {
            long cost_so_far = my_map[current_node_index].g + g(my_map[current_node_index], my_map[neighbor_index]);//1; // +1 ou g(current_node_index, current_neighbor)

            std::string path_executed = "didn't execute any if";// !todo! melhorar para demonstrar multiplos caminhos executados
            if (CheckOpenList(OPEN, my_map[neighbor_index]) && (cost_so_far < my_map[neighbor_index].g))
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
                // !todo! adicionar o restante da lógica
                my_map[neighbor_index].visited = false;
                path_executed = "path 2, neighbor_in_closed with better path";
            }

            if (!CheckOpenList(OPEN, my_map[neighbor_index]) && !my_map[neighbor_index].visited)
            {
                my_map[neighbor_index].f = cost_so_far + h(my_map[neighbor_index], my_map[GOAL]);
                my_map[neighbor_index].g = cost_so_far;
                my_map[neighbor_index].came_from = current_node_index;
                OPEN.push(my_map[neighbor_index]);
                path_executed = "path 3, wasn't in any list";
            }

            if (my_map[neighbor_index].appearance != "S" && my_map[neighbor_index].appearance != "G") {my_map[neighbor_index].appearance = "X";} // visited
            
            if (show_visited_neighbors)
            {
                visited_neighbors += "visited neighbor  : index  " + std::to_string(neighbor_index) +
                "  |  path_executed " + path_executed + "\n";
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
                std::cout << "CURRENT NODE  : " << " x " << my_map[current_node_index].x << "  y " << my_map[current_node_index].y << "  |  f " << my_map[current_node_index].f  << "  |  node_index " << my_map[current_node_index].node_index << "\n\n";

                if (show_priority_queue)
                {
                    ShowPriorityQueue(OPEN);
                    std::cout << "\n";
                }

                if (show_visited_neighbors)
                {
                    std::cout << visited_neighbors << "\n";
                }

                if (show_map)
                {
                    PrintMap(my_map, grid_size_x, grid_size_y);
                }

                if (interactive)
                {
                    std::cin.ignore();
                }

                std::cout << "<<------------------------------------------------------------------------->>\n";
            }
        }
    }

    std::cout << "ACHEI UM CAMINHO!!!" << "\n" << " ... " << "\n\n";
    std::cout << "PONTO DE CONTROLE  " << 2 << "\n";

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

    std::cout << "PONTO DE CONTROLE  " << 3 << "\n";

    PrintMap(my_map, grid_size_x, grid_size_y);

    return 0;
}
