// esse arquivo contém funções "utilitárias" para o funcionamento do código principal



// por ponteiros
// essa classe é a que organiza a lista de prioridades
// baseado no valor f de cada nó

#include <unordered_map>
#include <vector>
#include <map>
#include"AStarHeader.h"

void ArgsOptions(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        if ((std::string)argv[i] == "-Debug")
        {
            debug = true;
            std::cout << "Debug mode enabled!\n";
        }

        if ((std::string)argv[i] == "-DebugAll")
        {
            debug_all = true;
            std::cout << "Debug all mode enabled!\n";
        }

        if ((std::string)argv[i] == "-BestPathIndex")
        {
            best_path_index = true;
        }

        if ((std::string)argv[i] == "-ShowPriorityQueue")
        {
            show_priority_queue = true;
        }

        if ((std::string)argv[i] == "-ShowClosedList")
        {
            show_closed_list = true;
        }

        if ((std::string)argv[i] == "-ShowVisitedNeighbors")
        {
            show_visited_neighbors = true;
        }

        if ((std::string)argv[i] == "-Snapshot")
        {
            snapshot = true;

            snapshot_start_node_index = std::stoi(argv[i + 1]);
            snapshot_end_node_index = std::stoi(argv[i + 2]);
        }

        if ((std::string)argv[i] == "-SnapshotXY")
        {
            snapshot = true;

            // X-X : start-end
            snapshot_start_node_x = std::stoi(&argv[i + 1][0]);
            snapshot_end_node_x = std::stoi((&argv[i + 1][2]));

            // Y-Y : start-end
            snapshot_start_node_y = std::stoi(&argv[i + 2][0]);
            snapshot_end_node_y = std::stoi((&argv[i + 2][2]));
        }

        if ((std::string)argv[i] == "-Interactive")
        {
            interactive = true;
        }

        if ((std::string)argv[i] == "-ShowMap")
        {
            show_map = true;
        }

    }
}

bool CustomComparator::operator() (Node *n1, Node *n2)
{
    if (n1->f > n2->f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// usa uma cópia da minha lista de prioridade
// imprime a lista de prioridades a partir de uma cópia
void ShowPriorityQueue (std::priority_queue < Node*, std::vector<Node*>, CustomComparator > priority_queue)
{
    while (!priority_queue.empty())
    {
        std::cout << "priority_queue_copy : " << " x " << priority_queue.top()->x << "  y " << priority_queue.top()->y << "  |  f " << priority_queue.top()->f  << "  |  node_index " << priority_queue.top()->node_index << "\n";
        priority_queue.pop();
    }
}

void ShowClosedList (std::unordered_map< long, Node* > closed_list)
{
    for (auto &node_index : closed_list)
    {
        std::cout << "CLOSED_copy : " << " node_index " << node_index.first << "\n";
    }
}

// talvez seja melhor rever como fazer a comparação
// Checa se um nó existe na CLOSED list
bool CheckClosedList (Node *item)
{
    if (item == NULL)
    {
        //std::cout << "NADA" << "\n";
        return false;
    }
    else
    {
        //std::cout << "EXISTE   " << "x : " << item->x << " | y : " << item->y <<  "\n";
        return true;
    }
}

// checa se um nó existe na OPEN list, baseado no index do nó
bool CheckOpenList (std::priority_queue < Node*, std::vector<Node*>, CustomComparator > priority_queue, Node *item)
{
    while (!priority_queue.empty())
    {
        // é mais seguro olhar para as coordenadas ao em vez do endereço de memória,
        // ainda mais porque o priority_queue é uma cópia!
        if ((item->x == priority_queue.top()->x) && (item->y == priority_queue.top()->y))
        {
            //std::cout << "EXISTE NO OPEN!" << "\n";
            return true;
        }
        priority_queue.pop();
    }
    return false;
}

// essa função encontra os nós vizinhos e devolve um vetor do tamanho adequando (== a quantidade de vizinhos)
void ExpandNeighbors (Node *current_node, std::vector <long> *my_neighbors_coord, long grid_size_x, long grid_size_y)
{
    Node neighbors_nodes[4];
    neighbors_nodes[0] = Node(current_node->x, current_node->y - 1, ((current_node->x * grid_size_x) + current_node->y - 1));
    neighbors_nodes[1] = Node(current_node->x + 1, current_node->y, ((current_node->x + 1) * grid_size_x + current_node->y));
    neighbors_nodes[2] = Node(current_node->x, current_node->y + 1, ((current_node->x * grid_size_x) + current_node->y + 1));
    neighbors_nodes[3] = Node(current_node->x - 1, current_node->y, ((current_node->x - 1) * grid_size_x + current_node->y));

    // verificar os vizinhos no sentido horário
    for (long i = 0; i < 4; ++i)
    {
        bool valid_x = false;
        bool valid_y = false;
        bool valid_index = false;

        // avalio se o a posição em X é válida
        if (neighbors_nodes[i].x >= 0 && neighbors_nodes[i].x < grid_size_x)
        {
            valid_x = true;
        }

        if (neighbors_nodes[i].y >= 0 && neighbors_nodes[i].y < grid_size_y)
        {
            valid_y = true;
        }

        if (neighbors_nodes[i].node_index >= 0 && neighbors_nodes[i].node_index < (grid_size_x * grid_size_y))
        {
            valid_index = true;
        }

        if (valid_x && valid_y && valid_index)
        {
            my_neighbors_coord->push_back(neighbors_nodes[i].node_index);
        }

        if(debug == true)
        {
            std::cout << "neighbors_nodes[" << i << "] : x " << neighbors_nodes[i].x << " | y " << neighbors_nodes[i].y << " | node_index " << neighbors_nodes[i].node_index << "\n";
        }
    }
    if(debug == true)
    {
        std::cout << "\n\n";
    }
}

// imprimo meu mapa de nós usando uma cópia (parecido com um snapshot do momento)
void PrintMap (std::vector <Node, std::allocator<Node>> map, long grid_size_x, long grid_size_y)
{
    for (long x = 0; x < grid_size_x; ++x)
    {
        for (long y = 0; y < grid_size_y; ++y)
        {
            long cell_size = 4;

            std::cout << " ";
            long i = 0;
            for (auto letter : map[(x * grid_size_x) + y].appearance)
            {
                std::cout << letter;
                ++i;
            }
            if (i < (cell_size))
            {
                while (i != (cell_size))
                {
                    std::cout << " ";
                    ++i;
                }
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}


// usa ponteiro para preencher meu vetor mapa
void CreateNode (std::vector <Node, std::allocator<Node>> *map, long grid_size_x, long grid_size_y)
{
    for (long x = 0; x < grid_size_x; ++x)
    {
        for (long y = 0; y < grid_size_y; ++y)
        {
            map->insert(map->end(),Node(x, y, ((x * grid_size_x) + y)));

            //std::cout << "map : " << ((x * grid_size_x) + y) << " x "<< (*map)[(x * grid_size_x) + y].x << " y " << (*map)[(x * grid_size_x) + y].y << "\n";

            //std::cout << "x:" << x << "   y:" << y << "\n";
        }
    }
}
