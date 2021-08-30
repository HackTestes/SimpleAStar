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
// !todo! usar como argumento o index do nó ao invés de o nó em si (para usar GetX e GetY)
long g (Node current, Node neighbor)
{
    long dx = abs(current.x - neighbor.x);
    long dy = abs(current.y - neighbor.y);

    return cost_weight * (dx + dy);
}

long h (Node current, Node goal)
{
    long dx = abs(current.x - goal.x);
    long dy = abs(current.y - goal.y);

    return heuristic_weight * (dx + dy);
}
