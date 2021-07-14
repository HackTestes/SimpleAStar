// esse arquivo contém funções "utilitárias" para o funcionamento do código principal



// por ponteiros
// essa classe é a que organiza a lista de prioridades
// baseado no valor f de cada nó

#include"AStarHeader.h"

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
        std::cout << "priority_queue_copy : " << " x " << priority_queue.top()->x << " y " << priority_queue.top()->y << " | f " << priority_queue.top()->f  << " | node_index " << priority_queue.top()->node_index << "\n";
        priority_queue.pop();
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
        // é mais seguro olhar para as coordenadas ao em vez do endereço de momória,
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
void ExpandNeighbors2 (Node *current_node, std::vector <long> *my_neighbors_coord, long grid_size_x, long grid_size_y)
{
    // armazeno os possíveis vizinhos
    long index[4];

    // guardo os valores que seriam as coordenadas "modificadas" dos vizinhos
    long modified_index_x[2];
    long modified_index_y[2];

    modified_index_x[0] = current_node->x - 1;
    modified_index_x[1] = current_node->x + 1;

    modified_index_y[0] = current_node->y - 1;
    modified_index_y[1] = current_node->y + 1;

    // verifico se estão dentro dos limite de tamanho X( e Y???)
    for (long i = 0; i < 2; ++i)
    {
        if (modified_index_x[i] >= 0 && modified_index_x[i] < grid_size_x)
        {
            index[i] = ((modified_index_x[i]) * grid_size_x) + current_node->y;
        }
        else
        {
            index[i] = -1;
        }
    }

    // verifico se estão dentro dos limite de tamanho Y( e X???)
    for (long i = 0; i < 2; ++i)
    {
        if (modified_index_y[i] >= 0 && modified_index_y[i] < grid_size_x)
        {
            index[i + 2] = ((current_node->x) * grid_size_x) + modified_index_y[i];
        }
        else
        {
            index[i + 2] = -1;
        }
    }

    //limite é a quantidade de vizinhos
    //verifico se possuem um index válido ( x * y)
    for (long i = 0; i < 4; ++i)
    {
        if ( (index[i] >= 0) && (index[i] < (grid_size_x * grid_size_y)) )
        {
            my_neighbors_coord->push_back(index[i]);
        }
    }
}

// imprimo meu mapa de nós usando uma cópia (parecido com um snapshot do momento)
void PrintMap (std::vector <Node, std::allocator<Node>> map, long grid_size_x, long grid_size_y)
{
    for (long x = 0; x < grid_size_x; ++x)
    {
        for (long y = 0; y < grid_size_y; ++y)
        {
            std::cout << " " << map[(x * grid_size_x) + y].appearance << " ";
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
