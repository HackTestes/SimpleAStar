// Esse arquivo trata de implementações específicas para resolver um Nó (Node)

#include"AStarHeader.h"

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

    this->f = 0;
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

    this->f = 0;
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
