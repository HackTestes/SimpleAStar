// esse arquivo contém funções relacionadas ao f : f(n) = g(n) + h(n)
// custo e heurística


// http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#manhattan-distance

/*
function heuristic(node) =
    dx = abs(node.x - goal.x)
    dy = abs(node.y - goal.y)
    return D * (dx + dy)
*/

#include <cmath>

// por cópia
// além disso, adicionei esse parâmetro extra, já que esses nós (START, GOAL) estão fora do escopo da função
long g (Node current, Node start)
{
    long D = 4;
    long dx = abs(current.x - start.x);
    long dy = abs(current.y - start.y);

    return D * (dx + dy);
}

long h (Node current, Node goal)
{
    long D = 4;
    long dx = abs(current.x - goal.x);
    long dy = abs(current.y - goal.y);

    return D * (dx + dy);
}