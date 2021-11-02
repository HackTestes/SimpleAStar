#ifndef NODE_H
#define NODE_H

    // Esse arquivo trata de implementações específicas para resolver um Nó (Node)
    #include"AStarHeader.h"

    template<typename IntType>
    struct Node
    {
        IntType node_index;

        IntType g;

        IntType came_from;

        bool visited;
        bool in_priority_queue;

        std::string appearance;

        Node(IntType node_index)
        {
            this->node_index = node_index;

            this->g = 0;

            this->came_from = 0;

            this->visited = false;
            this->in_priority_queue = false; // otimização de velocidade em busca

            this->appearance = "o"; // nó normal
            //this->appearance = std::to_string(node_index);
        }

        Node()
        {
            this->node_index = 0;

            this->g = 0;

            this->came_from = 0;

            this->visited = false;
            this->in_priority_queue = false; // otimização de velocidade em busca

            this->appearance = "o"; // nó normal
        }

        static IntType GetX(IntType node_index)
        {
            return node_index / (grid_size_y); // floor division
        }

        static IntType GetY(IntType node_index)
        {
            return node_index - (GetX(node_index) * grid_size_y);
        }

        static IntType GetIndex(IntType x, IntType y)
        {
            return (x * grid_size_y) + y;
        }

        static bool VerifyCoordinate(IntType x, IntType y)
        {
            if ( !(x >= 0 && x < grid_size_x) )
            {
                return false;
            }

            if ( !(y >= 0 && y < grid_size_y) )
            {
                return false;
            }
            return true;
        }

        static bool VerifyIndex(IntType node_index)
        {
            if ( !(node_index >= 0 && node_index < (grid_size_x * grid_size_y)) )
            {
                return false;
            }
            return true;
        }
    };


    // imprimo meu mapa de nós usando uma cópia (parecido com um snapshot do momento)
    // será construída uma string de baixo para cima em cada linha (cima para baixo, esquerda para direita)
    template<typename IntType>
    void PrintMap (std::unordered_map <IntType, Node<IntType>> map, std::unordered_set<long> barrier_map)
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
                IntType index = (x * grid_size_y) + y;


                if (barrier_map.find(index) != barrier_map.end())
                {
                    line += StringPadding(1) + " ";
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

    template<typename IntType>
    void ShowBarrier(std::unordered_set<IntType> my_barrier)
    {
        std::string barrier_indexes = "Barrier indexes :  ";

        for (IntType barrier_index : my_barrier)
        {
            barrier_indexes += std::to_string(barrier_index) + ", ";
        }

        barrier_indexes = barrier_indexes.substr(0, barrier_indexes.size()-2);
        barrier_indexes += "\n";

        std::cout << barrier_indexes;
    }



#endif

/*
/////////////////////////////////////////////////////////////////////////////////////////
// !done! Criar um nó especial para a lista de prioridades
// Ideia -> o construtor desse nó deve:
//    - Receber um nó convencional como argumento
//    - Ter como atributos um valor "f" e o "node_index" apenas
// Se feito:
//    - Nó normal não precisa mais armazenar o "f"

// !todo! Retirar o index também???
// !done~! Usar GetX, GetY ou GetIndex
Node::Node(long node_index)
{
    this->node_index = node_index;

    this->g = 0;

    this->came_from = 0;

    this->visited = false;
    this->in_priority_queue = false; // otimização de velocidade em busca

    this->appearance = "o"; // nó normal
    //this->appearance = std::to_string(node_index);
}

Node::Node()
{
    this->node_index = 0;

    this->g = 0;

    this->came_from = 0;

    this->visited = false;
    this->in_priority_queue = false; // otimização de velocidade em busca

    this->appearance = "o"; // nó normal
}

// !done! adicionar restrições ao usar Get*(antes de retornar): o X ou Y estão corretos? Estão fora do tamanho do mapa? NÃO
// !done! Essas funções devem receber apenas valores válidos? SIM - criar aviso

// RESTRIÇÃO: Valores inválidos devem ser avaliados antes, portanto, use apenas valaores válidos com Node::GetX(), Node::GetY() e Node::GetIndex()
long Node::GetX(long node_index)
{
    return node_index / (grid_size_y); // floor division
}

long Node::GetY(long node_index)
{
    return node_index - (GetX(node_index) * grid_size_y);
}

long Node::GetIndex(long x, long y)
{
    return (x * grid_size_y) + y;
}

// verifica se um determinado parâmetro é válido
bool Node::VerifyCoordinate(long x, long y)
{
    if ( !(x >= 0 && x < grid_size_x) )
    {
        return false;
    }

    if ( !(y >= 0 && y < grid_size_y) )
    {
        return false;
    }
    return true;
}

bool Node::VerifyIndex(long node_index)
{
    if ( !(node_index >= 0 && node_index < (grid_size_x * grid_size_y)) )
    {
        return false;
    }
    return true;
}
*/