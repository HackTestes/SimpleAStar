

#include"AStarHeader.h"

Node::Node(long node_index)
{
    this->node_index = node_index;

    this->x = node_index / (grid_size_y);
    this->y = node_index - (this->x * grid_size_y);

    this->f = 0;
    this->g = 0;

    this->came_from = 0;

    this->visited = false;
    this->in_priority_queue = false;

    this->appearance = "o"; // nó normal
    //this->appearance = std::to_string(node_index);
}

Node::Node()
{
    this->x = 0;
    this->y = 0;
    this->node_index = 0;

    this->f = 0;
    this->g = 0;

    this->came_from = 0;

    this->visited = false;
    this->in_priority_queue = false;

    this->appearance = "o"; // nó normal
}

/* Talvez usar no futuro - !todo! clacular x e y ao invés de armazenar (otimiza espaço na memória)
long Node::GetX() const
{
    return this->node_index / (grid_size_y); // floor division
}

long Node::GetY() const
{
    return this->node_index - (GetX() * grid_size_y);
}
*/



