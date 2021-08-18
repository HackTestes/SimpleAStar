// esse arquivo contém funções "utilitárias" para o funcionamento do código principal

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include"AStarHeader.h"

// --------------------------teste--------------------------------------- //
    NodeParsed::NodeParsed(long x, long y)
    {
        this->index = Node::GetIndex(x, y);
        this->x = x;
        this->y = y;
    }

// --------------------------teste--------------------------------------- //


// !todo! adicionar supporte para opções com uma única letra "-h"
// !todo! melhorar opções que recebem valores (Snapshot, SnapshotXY)
// - verificar a entrada de dados (números)
// !todo! adicionar um Snapshot/SnapshotXY all: imprimir para todos os nós
long ArgsOptions(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {

        if ((std::string)argv[i] == "--help")
        {
            std::cout << "./SimpleAStarExecutable [gid_size_x] [gid_size_y] [START_X-START_Y] [GOAL_X-GOAL_Y]\n\n"
                      << "Options:\n\n"
                      << "--help\n"
                      << "--Debug\n"
                      << "--DebugAll\n"
                      << "--BestPathIndex\n"
                      << "--ShowPriorityQueue\n"
                      << "--ShowVisitedNeighbors\n"
                      << "--Snapshot [snapshot_start_node_index] [snapshot_end_node_index]\n"
                      << "--SnapshotXY [snapshot_start_node_x]-[snapshot_end_node_x] [snapshot_start_node_y]-[snapshot_end_node_y]\n"
                      << "--Interactive\n"
                      << "--ShowMap\n"
                      << "--ShowBarrier\n"
                      << "--BarrierFilePath [barrier_file_path]\n"
                      << "--Padding [padding_cell_size]\n";
            return 2; // qual o código para --help?
        }

        else if ((std::string)argv[i] == "--Debug")
        {
            debug = true;
            std::cout << "Debug mode enabled!\n";
            continue;
        }

        else if ((std::string)argv[i] == "--DebugAll")
        {
            debug_all = true;
            std::cout << "Debug all mode enabled!\n";
            continue;
        }

        else if ((std::string)argv[i] == "--BestPathIndex")
        {
            best_path_index = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowPriorityQueue")
        {
            show_priority_queue = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowVisitedNeighbors")
        {
            show_visited_neighbors = true;
            continue;
        }

        else if ((std::string)argv[i] == "--Snapshot")
        {
            snapshot = true;

            snapshot_start_node_index = std::abs(std::stoi(argv[i + 1]));
            snapshot_end_node_index = std::abs(std::stoi(argv[i + 2]));

            i = i + 2;
            continue;
        }

        else if ((std::string)argv[i] == "--SnapshotXY")
        {
            snapshot = true;

            // X-X : start-end
            snapshot_start_node_x = std::abs(std::stoi(&argv[i + 1][0]));
            snapshot_end_node_x = std::abs(std::stoi((&argv[i + 1][2])));


            // Y-Y : start-end
            snapshot_start_node_y = std::abs(std::stoi(&argv[i + 2][0]));
            snapshot_end_node_y = std::abs(std::stoi((&argv[i + 2][2])));
            std::cout << "snapshot_start_node_x :  " << snapshot_start_node_x << " | snapshot_end_node_x:  " << snapshot_end_node_x << "\n"
            << "snapshot_start_node_y :  " << snapshot_start_node_y << " | snapshot_end_node_y:  " << snapshot_end_node_y<< "\n\n";

            i = i + 2;
            continue;
        }

        else if ((std::string)argv[i] == "--Interactive")
        {
            interactive = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowMap")
        {
            show_map = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowBarrier")
        {
            show_barrier = true;
            continue;
        }

        else if ((std::string)argv[i] == "--BarrierFilePath")
        {
            barrier_enabled = true;
            barrier_file_path = (std::string)argv[i + 1];
            i = i + 1;
            continue;
        }

        else if ((std::string)argv[i] == "--Padding")
        {
            padding_cell_size = std::stoi(argv[i + 1]);
            i = i + 1;
            continue;
        }

        else if (i == 0)
        {
            continue;
        }

        // os parâmetros iniciais (tamanho, início e fim) são estáticos
        // caso nada dê certo e essa opção faça parte das posições estáticas,
        // pule
        else if ( (i == 1 | i == 2 | i == 3 | i == 4) && std::isdigit(*(argv[i])) )
        {
            continue;
        }

        else
        {
            std::cout << "Invalid option - " << (std::string)argv[i] << "\n";
            return 1;
        }
    }

    return 0;
}

// essa classe é a que organiza a lista de prioridades
// baseado no valor f de cada nó

bool CustomComparator::operator() (Node n1, Node n2)
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

// usa uma cópia da minha lista de prioridade
// imprime a lista de prioridades a partir de uma cópia
void ShowPriorityQueue (std::priority_queue < Node, std::vector<Node>, CustomComparator > priority_queue)
{
    while (!priority_queue.empty())
    {
        std::cout << "priority_queue_copy : " 
        << " x " << priority_queue.top().x 
        << "  y " << priority_queue.top().y 
        << "  |  f " << priority_queue.top().f  
        << "  |  node_index " << priority_queue.top().node_index 
        << "\n";
        priority_queue.pop();
    }
}


std::priority_queue< Node, std::vector<Node>, CustomComparator > CopyPriorityQueueExcept (std::priority_queue <Node, std::vector<Node>, CustomComparator>priority_queue, long except_index)
{
    std::priority_queue < Node, std::vector<Node>, CustomComparator > new_priority_queue;
    while (!priority_queue.empty())
    {
        if(priority_queue.top().node_index != except_index)
        {
            new_priority_queue.push(priority_queue.top());
        }
        priority_queue.pop();
    }
    return new_priority_queue;
}

// feito especialmente para a expansão dos vizinhos
// guarda apenas os valores necessários
class SmallNode
{
    public:
        long x;
        long y;
        long node_index;

        SmallNode(long x, long y, long node_index)
        {
            this->node_index = node_index;
            this->x = x;
            this->y = y;
        }

        SmallNode()
        {
            this->node_index = 0;
            this->x = 0;
            this->y = 0;
        }
};

// essa função encontra os nós vizinhos e devolve um vetor do tamanho adequando (== a quantidade de vizinhos)
void ExpandNeighbors (Node current_node, std::vector <long> *my_neighbors_coord)
{
    SmallNode neighbors_nodes[4];
    neighbors_nodes[0] = SmallNode(current_node.x, current_node.y - 1, ((current_node.x * grid_size_y) + current_node.y - 1));
    neighbors_nodes[1] = SmallNode(current_node.x + 1, current_node.y, ((current_node.x + 1) * grid_size_y + current_node.y));
    neighbors_nodes[2] = SmallNode(current_node.x, current_node.y + 1, ((current_node.x * grid_size_y) + current_node.y + 1));
    neighbors_nodes[3] = SmallNode(current_node.x - 1, current_node.y, ((current_node.x - 1) * grid_size_y + current_node.y));

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
            std::cout << "neighbors_nodes[" << i << "] : " 
            << "x " << neighbors_nodes[i].x 
            << " | y " << neighbors_nodes[i].y 
            << " | node_index " << neighbors_nodes[i].node_index 
            << " | gird_size_x " << grid_size_x
            << " | gird_size_y " << grid_size_y
            << "\n";
        }
    }
    if(debug == true)
    {
        std::cout << "\n\n";
    }
}

// gera os espaços necessário para fazer um padding 
std::string StringPadding(long string_length)
{
    long padding = padding_cell_size - string_length;
    std::string padding_text;

    for (long i = 0; i < padding; ++i)
    {
        padding_text += " ";
    }

    return padding_text;
}

// imprimo meu mapa de nós usando uma cópia (parecido com um snapshot do momento)
// será construída uma string de baixo para cima em cada linha (cima para baixo, esquerda para direita)
void PrintMap (std::unordered_map <long, Node> map, std::unordered_set<long> barrier_map)
{
    std::string map_string;
    std::string line;

    // construa a primeira linha com os índices de X
    line = StringPadding(1) + "|";
    for (long x = 0; x < grid_size_x; ++x)
    {
        std::string coordinate_x = std::to_string(x);
        line += StringPadding(coordinate_x.length()) + coordinate_x;
    }
    line += "\n\n\n";
    map_string = line;

    // separador
    line = StringPadding(1) + "|";
    for (long x = 0; x < grid_size_x; ++x)
    {
        line += StringPadding(1) + "_";
    }
    line += "\n\n\n";
    map_string = line + map_string; // faz em append no começo, ao invés de no final

    // constrói todo o restante do mapa, cada linha sendo um Y
    for (long y = 0; y < grid_size_y; ++y)
    {
        std::string coordinate_y = std::to_string(y);
        line = StringPadding( (coordinate_y.length() + 1) ) + coordinate_y + "|";
        for (long x = 0; x < grid_size_x; ++x)
        {
            long index = (x * grid_size_y) + y;


            if (barrier_map.find(index) != barrier_map.end())
            {
                line += StringPadding(1) + ".";
            }
            else if (map.find(index) != map.end())
            {
                std::string node_appearance = map[index].appearance;
                line += StringPadding(node_appearance.length()) + node_appearance;
            }
            else
            {
                line += StringPadding(1) + "o";
            }

        }
        line += "\n\n\n";
        map_string = line + map_string;
    }

    std::cout << map_string;
}

void ShowBarrier(std::unordered_set<long> my_barrier)
{
    std::string barrier_indexes = "Barrier indexes :  ";

    for (long barrier_index : my_barrier)
    {
        barrier_indexes += std::to_string(barrier_index) + ", ";
    }

    barrier_indexes = barrier_indexes.substr(0, barrier_indexes.size()-2);
    barrier_indexes += "\n";

    std::cout << barrier_indexes;
}

// novo ReadBarrier
// padrão .tsv de arquivo
long ReadBarrier(std::unordered_set<long> *my_barrier)
{
    std::string barrier_line;
    std::ifstream barrier_file (barrier_file_path);

    if (barrier_file.is_open())
    {
        while (getline(barrier_file, barrier_line))
        {
            long barrier_index = ParserXY(barrier_line, "\t").index;

            my_barrier->insert(barrier_index);
        }
        barrier_file.close();
    }
    else
    {
        std::cout << "Cannot open file";
        return 1;
    }

    return 0;
}

/*long ParserXY(std::string string_coordinate, std::string separator)
{
    long x, y;
    long separator_pos = string_coordinate.find(separator);
    long end_line_pos = string_coordinate.size();

    x = std::stoi(string_coordinate.substr(0, separator_pos));
    y = std::stoi(string_coordinate.substr(separator_pos + 1, end_line_pos - separator_pos));

    return Node::GetIndex(x, y);
}*/

NodeParsed ParserXY(std::string string_coordinate, std::string separator)
{
    long x, y;
    long separator_pos = string_coordinate.find(separator);
    long end_line_pos = string_coordinate.size();

    x = std::stoi(string_coordinate.substr(0, separator_pos));
    y = std::stoi(string_coordinate.substr(separator_pos + 1, end_line_pos - separator_pos));

    return NodeParsed(x, y);
}