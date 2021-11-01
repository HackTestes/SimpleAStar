#ifndef FN_FUNCTIONS_H
#define FN_FUNCTIONS_H

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
    template<typename Type>
    Type cost_g (Type current_node_index, Type neighbor_node_index)
    {
        Type dx = abs( Node::GetX(current_node_index) - Node::GetX(neighbor_node_index) );
        Type dy = abs( Node::GetY(current_node_index) - Node::GetY(neighbor_node_index) );

        return cost_weight * (dx + dy);
    }


    template<typename Type>
    Type heuristic_h (Type current_node_index, Type goal_node_index)
    {
        Type dx = abs(Node::GetX(current_node_index) - Node::GetX(goal_node_index));
        Type dy = abs(Node::GetY(current_node_index) - Node::GetY(goal_node_index));

        return heuristic_weight * (dx + dy);
    }

    //SlidingPuzzle

    // std::vector <position_index> = [item]; : otimiza o acesso para a posição
    // std::vector <item> = [position_index]; : otimiza o acesso para o item ESCOLHIDO
    template<typename Type>
    long SlidingPuzzleHeuristic_h (Type current_sliding_puzzle_obj_position, Type goal_sliding_puzzle_obj_position)
    {
        long heuristic_total = 0;

        for (long i = 0; i < current_sliding_puzzle_obj_position.size(); ++i)
        {
            long unit_result_h = heuristic_h(current_sliding_puzzle_obj_position[i], goal_sliding_puzzle_obj_position[i]);

            heuristic_total =  heuristic_total + unit_result_h;
        }

        return heuristic_total;
    }

#endif

/////////////////////////////////////////////////////
/*
// por cópia
// além disso, adicionei esse parâmetro extra, já que alguns nós estão fora do escopo da função
long cost_g (long current_node_index, long neighbor_node_index)
{
    long dx = abs( Node::GetX(current_node_index) - Node::GetX(neighbor_node_index) );
    long dy = abs( Node::GetY(current_node_index) - Node::GetY(neighbor_node_index) );

    return cost_weight * (dx + dy);
}

long heuristic_h (long current_node_index, long goal_node_index)
{
    long dx = abs(Node::GetX(current_node_index) - Node::GetX(goal_node_index));
    long dy = abs(Node::GetY(current_node_index) - Node::GetY(goal_node_index));

    return heuristic_weight * (dx + dy);
}

//SlidingPuzzle

// std::vector <position_index> = [item]; : otimiza o acesso para a posição
// std::vector <item> = [position_index]; : otimiza o acesso para o item ESCOLHIDO

long SlidingPuzzleHeuristic_h (std::vector<long> current_sliding_puzzle_obj_position, std::vector<long> goal_sliding_puzzle_obj_position)
{
    long heuristic_total = 0;

    for (long i = 0; i < current_sliding_puzzle_obj_position.size(); ++i)
    {
        long unit_result_h = heuristic_h(current_sliding_puzzle_obj_position[i], goal_sliding_puzzle_obj_position[i]);

        heuristic_total =  heuristic_total + unit_result_h;
    }

    return heuristic_total;
}
*/
