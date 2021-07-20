
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
- gera curvas
    .pode ser uma segmentação de caminhos (ex.:impeime dois caminhos pela metade)
        .pode ser pq deixer de fazer uma verificação no CLOSED no loop principal - em comparação ao do RedBlobGames
        .pode ser que eu usei f(n) ao invés de g(n) no loop principal - em comparação ao do RedBlobGames
        .pode ser uma "barreira" (fazer um mapa interativo para ver a curva)

- OPEN e CLOSED apontam para os nós do map (um vetor!)
    .aparentemente o OPEN e CLOSED apontando (com ponteiros) para um vetor (que não garante linearidade e pode invalidar ponteiros mudando
    os itens de lugar ao fazer um resize) pode causar comportamento indefinido!!!
    .pode estar csando outros problemas.
    #talvez dê para somente usar uma array ao invés de um vetor

*/

// Variáveis globais
bool debug = false;
bool best_path_index = false;
bool debug_all = false;
bool show_priority_queue = false;
bool show_closed_list = false;
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

    //long grid_size_x = 10;
    long grid_size_x = std::stoi(argv[1]);

    //long grid_size_y = 10;
    long grid_size_y = std::stoi(argv[2]);

    std::vector <Node, std::allocator<Node>> my_map;
    CreateNode (&my_map, grid_size_x, grid_size_y);

    //Node *GOAL = &my_map[94];
    Node *GOAL = &my_map[std::stoi(argv[4])];
    (*GOAL).appearance = "G";

    //Node *START = &my_map[0];
    Node *START = &my_map[std::stoi(argv[3])];
    (*START).appearance = "S";

    // usar o f do nó para ordenar
    // com ponteiros
    std::priority_queue < Node*, std::vector<Node*>, CustomComparator > OPEN;

    // talvez seja possível usar o nó como chave
    // ou o endereço de memória do nó
    // devo mesmo usar um hash_map ou posso usar uma lista ordenada?
    // Já que é possível saber o index dos objetos
    std::unordered_map< long, Node* > CLOSED;

    (*START).f = g((*START), (*START)) + h((*START), (*GOAL));

    OPEN.push(&(*START));

    std::cout << "PONTO DE CONTROLE  " << 1 << "\n";

    // inicio o loop principal
    while ( (OPEN.top()->x != (*GOAL).x) || (OPEN.top()->y != (*GOAL).y))
    {
        long current_node_index = (OPEN.top()->x * grid_size_x) + OPEN.top()->y;
        OPEN.pop();

        CLOSED[current_node_index] = &my_map[current_node_index];

        std::vector <long> my_neighbors_list;
        ExpandNeighbors(&my_map[current_node_index], &my_neighbors_list, grid_size_x, grid_size_y);

        std::string visited_neighbors = "";
        for (long neighbor_index : my_neighbors_list)
        {
            long neighbor_f = g (my_map[neighbor_index], (*START)) + h (my_map[neighbor_index], (*GOAL));

            bool neighbor_in_open = CheckOpenList(OPEN, &my_map[neighbor_index]);
            bool neighbor_in_closed = CheckClosedList(CLOSED[neighbor_index]);
            std::string path_executed = "didn't execute any if";

            if (neighbor_in_open && (neighbor_f < my_map[neighbor_index].f))
            {
                my_map[neighbor_index].f = neighbor_f;
                my_map[neighbor_index].came_from = &my_map[current_node_index];
                path_executed = "if 1, neighbor_in_open with better path";
            }

            if (neighbor_in_closed && (neighbor_f < my_map[neighbor_index].f))
            {
                path_executed = "if 2, neighbor_in_closed with better path";
            }

            if (!neighbor_in_open && !neighbor_in_closed)
            {
                my_map[neighbor_index].f = neighbor_f;
                my_map[neighbor_index].came_from = &my_map[current_node_index];
                OPEN.push(&my_map[neighbor_index]);
                path_executed = "if 3, wasn't in any list";
            }

            if (my_map[neighbor_index].appearance != "S" && my_map[neighbor_index].appearance != "G") {my_map[neighbor_index].appearance = "X";} // visited
            
            if (show_visited_neighbors)
            {
                visited_neighbors += "visited neighbor  : index  " + std::to_string(neighbor_index) + "  |  neighbor_in_open " + std::to_string(neighbor_in_open) + "  |  neighbor_in_closed " + std::to_string(neighbor_in_closed) + "  |  path_executed " + path_executed + "\n";
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

                if (show_closed_list)
                {
                    ShowClosedList(CLOSED);
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
    long goal_index = ((*GOAL).x * grid_size_x) + (*GOAL).y;
    long start_index = ((*START).x * grid_size_x) + (*START).y;
    long index = goal_index;
    while (true)
    {
        if (index == start_index)
        {
            break;
        }
        else
        {
            if (index != goal_index)
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
            index = (my_map[index].came_from->x * grid_size_x) + my_map[index].came_from->y;
            //std::cout << "came_from index  " << index << "\n";
        }
    }

    std::cout << "PONTO DE CONTROLE  " << 3 << "\n";

    PrintMap(my_map, grid_size_x, grid_size_y);

    return 0;
}
