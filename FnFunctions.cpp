// esse arquivo contém funções relacionadas ao f : f(n) = g(n) + h(n)
// custo(g) e heurística(h)
// f é usando para ordenar os nós na lista de prioridades

// http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#manhattan-distance
/*
function heuristic(node) =
    dx = abs(node.x - goal.x)
    dy = abs(node.y - goal.y)
    return D * (dx + dy)
*/

#include <cmath>
#include"AStarHeader.h"

// por cópia
// além disso, adicionei esse parâmetro extra, já que alguns nós estão fora do escopo da função
// !done! usar como argumento o index do nó ao invés de o nó em si (para usar GetX e GetY)
// Exemplo:
// current_node_index, neighbor_node_index
// dx = Node::GetX(current_node_index) - Node::GetX(neighbor_node_index)

long cost_g (long current_node_index, long neighbor_node_index)
{
    long dx = abs(Node::GetX(current_node_index) - Node::GetX(neighbor_node_index));
    long dy = abs(Node::GetY(current_node_index) - Node::GetY(neighbor_node_index));

    return cost_weight * (dx + dy);
}

long heuristic_h (long current_node_index, long goal_node_index)
{
    long dx = abs(Node::GetX(current_node_index) - Node::GetX(goal_node_index));
    long dy = abs(Node::GetY(current_node_index) - Node::GetY(goal_node_index));

    return heuristic_weight * (dx + dy);
}


// !todo! Retirar trecho
/*
long cost_g (Node current, Node neighbor)
{
    long dx = abs(current.x - neighbor.x);
    long dy = abs(current.y - neighbor.y);

    return cost_weight * (dx + dy);
}
*/

// !todo! Retirar trecho
/*
long heuristic_h (Node current, Node goal)
{
    long dx = abs(current.x - goal.x);
    long dy = abs(current.y - goal.y);

    return heuristic_weight * (dx + dy);
}
*/