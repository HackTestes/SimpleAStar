
#include <queue>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>

//#include "Node.cpp"
//#include "FunctionsUtils.cpp"
//#include "FnFunctions.cpp"

#include"AStarHeader.h"

// o mapa é uma tabela
// movimentos: cima, baixo, esquerda, direta (4 movimentos possíveis)

// strtol(argv[1], NULL, 10);
// string to long(string, endpointer, base)

/*
#ERROS:
- listas grandes o programa não funciona com grades com mais de 100 nós, gera curvas
    .pode ser uma segmentação de caminhos (ex.:impeime dois caminhos pela metade)
        .pode ser pq deixer de fazer uma verificação no CLOSED no loop principal - em comparação ao do RedBlobGames
        .pode ser que eu usei f(n) ao invés de g(n) no loop principal - em comparação ao do RedBlobGames

        .pode ser uma "barreira" (fazer um mapa interativo para ver a curva)

*/

int main (int argc, char* argv[])
{
    // defino as variáveis principais : tamanho X e Y, começo (START), fim (GOAL)

    long grid_size_x = 10;
    //long grid_size_x = strtol(argv[1], NULL, 10);

    long grid_size_y = 10;
    //long grid_size_y = strtol(argv[2], NULL, 10);

    std::vector <Node, std::allocator<Node>> my_map;
    CreateNode (&my_map, grid_size_x, grid_size_y);

    Node *GOAL = &my_map[94];
    //Node *GOAL = &my_map[strtol(argv[4], NULL, 10)];
    (*GOAL).appearance = "G";

    Node *START = &my_map[0];
    //Node *START = &my_map[strtol(argv[3], NULL, 10)];
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
        ExpandNeighbors2(&my_map[current_node_index], &my_neighbors_list, grid_size_x, grid_size_y);

        for (long neighbor_index : my_neighbors_list)
        {
            long neighbor_f = g (my_map[neighbor_index], (*START)) + h (my_map[neighbor_index], (*GOAL));

            bool neighbor_in_open = CheckOpenList(OPEN, &my_map[neighbor_index]);
            bool neighbor_in_closed = CheckClosedList(CLOSED[neighbor_index]);

            if (neighbor_in_open && (neighbor_f < my_map[neighbor_index].f))
            {
                my_map[neighbor_index].f = neighbor_f;
                my_map[neighbor_index].came_from = &my_map[current_node_index];
            }

            if (!neighbor_in_open && !neighbor_in_closed)
            {
                my_map[neighbor_index].f = neighbor_f;
                my_map[neighbor_index].came_from = &my_map[current_node_index];
                OPEN.push(&my_map[neighbor_index]);
            }
            if (my_map[neighbor_index].appearance != "S" && my_map[neighbor_index].appearance != "G") {my_map[neighbor_index].appearance = "X";} // visited
            //std::cout << "neighbor_index  " << neighbor_index << "\n";
        }
        //std::cout << "current_node_index  " << current_node_index << "\n";
        //ShowPriorityQueue(OPEN);
        //PrintMap(my_map, grid_size_x, grid_size_y);
        //std::string stop_executing;
        //std::cin >> stop_executing;
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
            if (index != goal_index) {my_map[index].appearance = "-";}
            index = (my_map[index].came_from->x * grid_size_x) + my_map[index].came_from->y;
            //std::cout << "came_from index  " << index << "\n";
        }
    }

    std::cout << "PONTO DE CONTROLE  " << 3 << "\n";

    PrintMap(my_map, grid_size_x, grid_size_y);

    return 0;
}
