#include"AStarHeader.h"


// !todo! fazer com que possa receber tipo diferentes de mapa
// Pode ser feito usando templates e compilando uma versão diferente para cada tipo específico
// Cada "função específica" deve/vai ser instânciada ou no header ou nas funcções espcíficas

Node CreateObj(long index)
{
    return Node(index);
}

SlidingPuzzle CreateObj(std::vector<long> sliding_puzzle_map)
{
    return SlidingPuzzle(sliding_puzzle_map);
}

/*std::vector<long> RetriveKeys(std::vector<long> neighbor_list)
{
    return {};
}*/

std::vector<long> RetriveKeys(std::vector< std::vector<long> > neighbor_list)
{
    return {};
}

long SelectIndex(std::unordered_map <long, Node> &map, long index)
{
    return map[index].node_index;
}

long SelectIndex(std::unordered_map <long, SlidingPuzzle> &map, long key)
{
    return map[key].empty_index;
}


long StartMain()
{
    std::unordered_map <long, Node> my_map;

    // Verificação feita em outro lugar, criação do nó aqui
    my_map[START] = Node(START); 
    my_map[START].appearance = "S";

    my_map[GOAL] = Node(GOAL);
    my_map[GOAL].appearance = "G";

    long previous_map_size = my_map.size();

    // usa o f do nó para ordenar
    std::priority_queue < PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue > OPEN;

    // inicializo todos os atributos necessários para começar
    my_map[START].in_priority_queue = true;
    OPEN.push( PriorityQueueContainer(my_map[START].f, START) );

    long main_loop_return = MainLoop(my_map, OPEN, previous_map_size);

    if (main_loop_return == 1)
    {return 0;}

    std::cout << " BEST PATH\n\n\n";

    ReversePath(my_map);

    PrintMap(my_map, barrier);

    return 0;
}

/*long SlidingPuzzle_StartMain()
{
    std::unordered_map <long, SlidingPuzzle> my_map;

    // Verificação feita em outro lugar, criação do nó aqui
    my_map[START] = SlidingPuzzle(sliding_puzzle_goal);

    my_map[GOAL] = SlidingPuzzle(sliding_puzzle_start);

    long previous_map_size = my_map.size();

    // usa o f do nó para ordenar
    std::priority_queue < PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue > OPEN;

    // inicializo todos os atributos necessários para começar
    my_map[START].in_priority_queue = true;
    long starting_f = 0;
    OPEN.push( PriorityQueueContainer(starting_f, START) );

    long main_loop_return = SlidingPuzzleMainLoop(my_map, OPEN, previous_map_size);

    if (main_loop_return == 1)
    {return 0;}

    std::cout << " BEST PATH\n\n\n";

    SlidingPuzzle_ReversePath(my_map);

    PrintMap(my_map, barrier);

    return 0;
}*/


long MainLoop(std::unordered_map<long, Node> &my_map, std::priority_queue<PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue> &OPEN, long previous_map_size)
{
    // inicio o loop principal
    while (OPEN.top().reference_key != GOAL)
    {

        // #STEP# GET THE KEY
        // #STEP# REMOVE KEY FROM OPEN
        // #STEP# LOOP TRGOUGH THE NEIGHBORS
            // #STEP# CREATE THE NEIGHBORS IF IT DOESN'T EXIST
            // #STEP# SELECT THE APPROPRIATE TREATMENT
        // #STEP# CHECK IF THE MAP DOESN'T GROW AND IF OPEN IS EMPTY


        long current_key = OPEN.top().reference_key;

        long current_node_index = SelectIndex(my_map, current_key);

        //long current_node_index = OPEN.top().reference_key;
        my_map[current_node_index].in_priority_queue = false;
        OPEN.pop();

        my_map[current_node_index].visited = true;

        // !todo! Template
        std::vector<long> my_neighbors_keys = ExpandNeighbors(Node::GetX(current_node_index), Node::GetY(current_node_index)); // !todo! Overload

        std::string visited_neighbors = "";
        //for (long neighbor_key : my_neighbors_keys)
        for (long current_neighbor = 0; current_neighbor < my_neighbors_keys.size(); ++current_neighbor)
        {
            long neighbor_key = my_neighbors_keys[current_neighbor];

            bool path_executed_bool[4];
            path_executed_bool[0] = true;
            path_executed_bool[1] = false;
            path_executed_bool[2] = false;
            path_executed_bool[3] = false;

            // se não for uma barreira
            if (!(barrier.find(neighbor_key) != barrier.end()))
            {

                // se não existir no meu mapa ainda, crie
                if (!(my_map.find(neighbor_key) != my_map.end()))
                {
                    my_map[neighbor_key] = CreateObj(my_neighbors_keys[current_neighbor]); // !todo! CreateObj
                }

                long cost_so_far = my_map[current_node_index].g + cost_g(current_node_index, neighbor_key);

                // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
                if (my_map[neighbor_key].in_priority_queue && (cost_so_far < my_map[neighbor_key].g))
                {
                    my_map[neighbor_key].f = cost_so_far + heuristic_h(neighbor_key, GOAL); // !todo! Overload
                    my_map[neighbor_key].g = cost_so_far;
                    my_map[neighbor_key].came_from = current_node_index;
                    OPEN = CopyPriorityQueueExcept(OPEN, neighbor_key); // removo o nó com valor antigo
                    OPEN.push( PriorityQueueContainer(my_map[neighbor_key].f, neighbor_key) ); // coloco de volta com o valor atualizado

                    path_executed_bool[0] = false;
                    path_executed_bool[1] = true;
                }


                if (my_map[neighbor_key].visited && (cost_so_far < my_map[neighbor_key].g))
                {
                    my_map[neighbor_key].visited = false;

                    path_executed_bool[0] = false;
                    path_executed_bool[2] = true;
                }


                if (!my_map[neighbor_key].in_priority_queue && !my_map[neighbor_key].visited)
                {
                    my_map[neighbor_key].f = cost_so_far + heuristic_h(neighbor_key, GOAL); // !todo! Overload
                    my_map[neighbor_key].g = cost_so_far;
                    my_map[neighbor_key].came_from = current_node_index;
                    my_map[neighbor_key].in_priority_queue = true;

                    OPEN.push( PriorityQueueContainer(my_map[neighbor_key].f, neighbor_key) );

                    path_executed_bool[0] = false;
                    path_executed_bool[3] = true;
                }

                previous_map_size = my_map.size(); // atualiza o valor de tamanho para posterior avaliação

                if (my_map[neighbor_key].appearance != "S" && my_map[neighbor_key].appearance != "G") {my_map[neighbor_key].appearance = "X";} // visited but not expanded
                
                if (show_visited_neighbors)
                {
                    visited_neighbors += "Visited neighbor:\n"
                                         "  -  node_index " + std::to_string(my_map[neighbor_key].node_index) + "\n" +
                                         "  -  x " + std::to_string( Node::GetX(neighbor_key) ) +
                                         "  y " + std::to_string( Node::GetY(neighbor_key) ) +  "\n" +
                                         "  -  f " + std::to_string(my_map[neighbor_key].f) + "\n" +
                                         "  -  g " + std::to_string(my_map[neighbor_key].g) + "\n" +
                                         "  -  visited " + std::to_string(my_map[neighbor_key].visited) + "\n" +
                                         "  -  in_priority_queue " + std::to_string(my_map[neighbor_key].in_priority_queue) + "\n" +
                                         "  -  path[0] = " + std::to_string(path_executed_bool[0]) +":  didn't execute any alternative path\n" +
                                         "  -  path[1] = " + std::to_string(path_executed_bool[1]) +":  neighbor_in_open with better path\n" +
                                         "  -  path[2] = " + std::to_string(path_executed_bool[2]) +":  neighbor_in_closed with better path\n" +
                                         "  -  path[3] = " + std::to_string(path_executed_bool[3]) +":  added to the open queue, bacause it wasn't in any list\n\n";

                }
            }
        }

        SnapshotOptions(current_node_index, OPEN, visited_neighbors, my_map);

        // (mapa não aumentou e PQ vazia) - condição de parada com barreiras
        if (previous_map_size == my_map.size() && OPEN.size() == 0)
        {
            std::cout << "The best path doesn't exist\n\n";
            PrintMap(my_map, barrier);
            return 1;
        }
    }

    return 0;
}

long SlidingPuzzleMainLoop(std::unordered_map<long, SlidingPuzzle> &my_map, std::priority_queue<PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue> &OPEN, long previous_map_size)
{
    // inicio o loop principal
    while (OPEN.top().reference_key != GOAL)
    {
        long current_key = OPEN.top().reference_key;

        long empty_index = SelectIndex(my_map, current_key);

        my_map[current_key].in_priority_queue = false;
        OPEN.pop();

        my_map[current_key].visited = true;

        // !todo! Template
        std::vector< std::vector<long> > my_neighbors_list = {{}}; //ExpandNeighbors(Node::GetX(empty_index), Node::GetY(empty_index)); // !todo! Overload

        std::vector<long> my_neighbors_keys = RetriveKeys(my_neighbors_list);

        std::string visited_neighbors = "";
        //for (long neighbor_key : my_neighbors_keys)
        for (long current_neighbor = 0; current_neighbor < my_neighbors_keys.size(); ++current_neighbor)
        {
            long neighbor_key = my_neighbors_keys[current_neighbor];

            bool path_executed_bool[4];
            path_executed_bool[0] = true;
            path_executed_bool[1] = false;
            path_executed_bool[2] = false;
            path_executed_bool[3] = false;

            // se não for uma barreira
            if (!(barrier.find(neighbor_key) != barrier.end()))
            {
                long neighbor_f;

                // se não existir no meu mapa ainda, crie
                if (!(my_map.find(neighbor_key) != my_map.end()))
                {
                    my_map[neighbor_key] = CreateObj(my_neighbors_list[current_neighbor]); // !todo! CreateObj
                }

                long cost_so_far = my_map[neighbor_key].g + cost_g(empty_index, my_map[neighbor_key].empty_index);

                // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
                // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
                // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
                // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
                if (my_map[neighbor_key].in_priority_queue && (cost_so_far < my_map[neighbor_key].g))
                {
                    neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h(my_map[neighbor_key].sliding_puzzle, my_map[GOAL].sliding_puzzle); // !todo! Overload
                    my_map[neighbor_key].g = cost_so_far;
                    my_map[neighbor_key].came_from = current_key;
                    OPEN = CopyPriorityQueueExcept(OPEN, neighbor_key); // removo o nó com valor antigo
                    OPEN.push( PriorityQueueContainer(neighbor_f, neighbor_key) ); // coloco de volta com o valor atualizado

                    path_executed_bool[0] = false;
                    path_executed_bool[1] = true;
                }


                if (my_map[neighbor_key].visited && (cost_so_far < my_map[neighbor_key].g))
                {
                    my_map[neighbor_key].visited = false;

                    path_executed_bool[0] = false;
                    path_executed_bool[2] = true;
                }


                if (!my_map[neighbor_key].in_priority_queue && !my_map[neighbor_key].visited)
                {
                    neighbor_f = cost_so_far + SlidingPuzzleHeuristic_h(my_map[neighbor_key].sliding_puzzle, my_map[GOAL].sliding_puzzle); // !todo! Overload
                    my_map[neighbor_key].g = cost_so_far;
                    my_map[neighbor_key].came_from = current_key;
                    my_map[neighbor_key].in_priority_queue = true;

                    OPEN.push( PriorityQueueContainer(neighbor_f, neighbor_key) );

                    path_executed_bool[0] = false;
                    path_executed_bool[3] = true;
                }

                previous_map_size = my_map.size(); // atualiza o valor de tamanho para posterior avaliação

                if (show_visited_neighbors)
                {
                    visited_neighbors += "Visited neighbor:\n"
                                         "  -  key " + std::to_string(neighbor_key) + "\n" +
                                         "  -  empty cell index " + std::to_string( my_map[neighbor_key].empty_index ) + "\n" +
                                         "  -  f " + std::to_string(neighbor_f) + "\n" +
                                         "  -  g " + std::to_string(my_map[neighbor_key].g) + "\n" +
                                         "  -  visited " + std::to_string(my_map[neighbor_key].visited) + "\n" +
                                         "  -  in_priority_queue " + std::to_string(my_map[neighbor_key].in_priority_queue) + "\n" +
                                         "  -  path[0] = " + std::to_string(path_executed_bool[0]) +":  didn't execute any alternative path\n" +
                                         "  -  path[1] = " + std::to_string(path_executed_bool[1]) +":  neighbor_in_open with better path\n" +
                                         "  -  path[2] = " + std::to_string(path_executed_bool[2]) +":  neighbor_in_closed with better path\n" +
                                         "  -  path[3] = " + std::to_string(path_executed_bool[3]) +":  added to the open queue, bacause it wasn't in any list\n\n";

                }
            }
        }

        //SnapshotOptions(current_node_index, OPEN, visited_neighbors, my_map);

        // (mapa não aumentou e PQ vazia) - condição de parada com barreiras
        if (previous_map_size == my_map.size() && OPEN.size() == 0)
        {
            std::cout << "The best path doesn't exist\n\n";
            return 1; // !todo! Avaliar não é 1 de erro
        }
    }

    return 0;
}




// ------------------------------------------- //

/*

//template<typename T>
struct Env
{
    // Common
    std::priority_queue<PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue> OPEN;
    long current_key;
    std::vector neighbor_keys_list;
    std::string visited_neighbors_info;

    // Node map
    std::unordered_map<long, Node> problem_node_map;

    // Sliding Puzzle
    std::unordered_map<long, Node> problem_sliding_puzzle;


}

void StartupLoop_NodeMap(Env &environment)
{
    environment.current_key = environment.OPEN.top().reference_key;

    long current_node_index = current_key;

    //long current_node_index = OPEN.top().reference_key;
    my_map[current_node_index].in_priority_queue = false;
    OPEN.pop();

    my_map[current_node_index].visited = true;

    environment.neighbor_keys_list = ExpandNeighbors(Node::GetX(current_node_index), Node::GetY(current_node_index)); // !todo! Overload

    environment.visited_neighbors = "";
}

void EnvLoopThroughNeighbors(Env &environment)
{
    for (long current_neighbor = 0; current_neighbor < environment.my_neighbors_keys.size(); ++current_neighbor)
    {
        long neighbor_key = my_neighbors_keys[current_neighbor];

        bool path_executed_bool[4];
        path_executed_bool[0] = true;
        path_executed_bool[1] = false;
        path_executed_bool[2] = false;
        path_executed_bool[3] = false;

        // se não for uma barreira
        if (!(barrier.find(neighbor_key) != barrier.end()))
        {

            // se não existir no meu mapa ainda, crie
            if (!(my_map.find(neighbor_key) != my_map.end()))
            {
                my_map[neighbor_key] = CreateObj(my_neighbors_list[current_neighbor]); // !todo! CreateObj
            }

            long cost_so_far = my_map[current_node_index].g + cost_g(current_node_index, neighbor_key);

            // in_priority_queue = true ---> se estiver na PQ (toda adição na PQ)
            // in_priority_queue = false ---> se não estiver na PQ (toda remoção da PQ)
            // no caso dele precisar ser atualizado na PQ (remove e adiciona), não é necessário mudar a flag (porque é apenas uma atualização de valor)
            // A verificação não precisa iterar sobre a PQ (otimização de velocidade talvez?)
            if (my_map[neighbor_key].in_priority_queue && (cost_so_far < my_map[neighbor_key].g))
            {
                my_map[neighbor_key].f = cost_so_far + heuristic_h(neighbor_key, GOAL); // !todo! Overload
                my_map[neighbor_key].g = cost_so_far;
                my_map[neighbor_key].came_from = current_node_index;
                OPEN = CopyPriorityQueueExcept(OPEN, neighbor_key); // removo o nó com valor antigo
                OPEN.push( PriorityQueueContainer(my_map[neighbor_key].f, neighbor_key) ); // coloco de volta com o valor atualizado

                path_executed_bool[0] = false;
                path_executed_bool[1] = true;
            }


            if (my_map[neighbor_key].visited && (cost_so_far < my_map[neighbor_key].g))
            {
                my_map[neighbor_key].visited = false;

                path_executed_bool[0] = false;
                path_executed_bool[2] = true;
            }


            if (!my_map[neighbor_key].in_priority_queue && !my_map[neighbor_key].visited)
            {
                my_map[neighbor_key].f = cost_so_far + heuristic_h(neighbor_key, GOAL); // !todo! Overload
                my_map[neighbor_key].g = cost_so_far;
                my_map[neighbor_key].came_from = current_node_index;
                my_map[neighbor_key].in_priority_queue = true;

                OPEN.push( PriorityQueueContainer(my_map[neighbor_key].f, neighbor_key) );

                path_executed_bool[0] = false;
                path_executed_bool[3] = true;
            }

            previous_map_size = my_map.size(); // atualiza o valor de tamanho para posterior avaliação
        }
    }
}


// Teste
long EnvMainLoop(Env &environment)
{
    // inicio o loop principal
    while (environment.OPEN.top().reference_key != GOAL)
    {
        StartupLoop[problem_code](environment);

        LoopThroughNeighbors[problem_code](environment);

        CheckIfProblemHasNoBestPath[problem_code](environment);

        ShowEndOfLoopInfo[problem_code](environment);
    }

    return 0;
}
*/

// ------------------------------------------- //



void SnapshotOptions(long current_key,
                    std::priority_queue < PriorityQueueContainer, std::vector<PriorityQueueContainer>, SortPriorityQueue > current_priority_queue,
                    std::string neighbors,
                    std::unordered_map<long, Node> &current_map)
{
    // Options - mostra uma série de informações sobre o loop principal
    if (snapshot)
    {
        if ( (current_key >= snapshot_start_node_index && current_key <= snapshot_end_node_index) || //usando index
            (Node::GetX(current_key) >= snapshot_start_node_x  && Node::GetX(current_key) <= snapshot_end_node_x && //usando as coordenadas
            Node::GetY(current_key) >= snapshot_start_node_y  && Node::GetY(current_key) <= snapshot_end_node_y) )
        {
            std::cout << "<<-------------------------End of loop iformation:------------------------->>\n\n";
            std::cout << "CURRENT NODE  : " << " node_index " << current_map[current_key].node_index
                                            << "  |  x " << Node::GetX(current_key)
                                            << "  y " << Node::GetY(current_key)
                                            << "  |  f " << current_map[current_key].f
                                            << "  |  g " << current_map[current_key].g
                                            << "  |  visited " << current_map[current_key].visited
                                            << "  |  in_priority_queue " << current_map[current_key].in_priority_queue << "\n\n";

            if (show_priority_queue)
            {
                ShowPriorityQueue(current_priority_queue);
                std::cout << "\n";
            }

            if (show_visited_neighbors)
            {
                std::cout << neighbors << "\n";
            }

            if (show_barrier)
            {
                ShowBarrier(barrier);
                std::cout << "\n\n";
            }

            if (show_map)
            {
                // !todo! PrintMap genérico
                //    - Sem argumentos
                //    - Overloading
                PrintMap(current_map, barrier);
            }

            if (interactive)
            {
                std::cin.ignore();
            }

            std::cout << "<<------------------------------------------------------------------------->>\n";
        }
    }
}


// template <typename T>
void ReversePath(std::unordered_map<long, Node> &map_reverse)
{
    // mudo a "aparência" dos nós que pertencem ao melhor caminho
    // fazendo o caminho reverso do GOAL para o START
    long index = GOAL;
    while (true)
    {
        if (index == START)
        {
            break;
        }
        else
        {
            if (index != GOAL)
            {
                // !todo! Funções específicas para o tipo de problema
                if (best_path_index)
                {
                    map_reverse[index].appearance =  std::to_string(index);
                }
                else
                {
                    map_reverse[index].appearance = "@";
                }
            }
            index = map_reverse[index].came_from;
        }
    }
}

void SlidingPuzzle_ReversePath(std::unordered_map<long, SlidingPuzzle> &map_reverse)
{
    // mudo a "aparência" dos nós que pertencem ao melhor caminho
    // fazendo o caminho reverso do GOAL para o START
    long index = GOAL;
    while (true)
    {
        if (index == START)
        {
            break;
        }
        else
        {
            if (index != GOAL)
            {
                map_reverse[index].PrintCurrentSlidingPuzzle(10, 10);
            }
            index = map_reverse[index].came_from;
        }
    }
}