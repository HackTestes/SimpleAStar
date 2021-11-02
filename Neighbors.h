#ifndef NEIGHBORS_H
#define NEIGHBORS_H 

    #include <unordered_map>
    #include <unordered_set>
    #include <vector>
    #include <map>
    #include <iostream>
    #include <fstream>
    #include <stdlib.h>

    #include"AStarHeader.h"

    // feito especialmente para a expansão dos vizinhos
    // guarda apenas os valores necessários
    template<typename Type>
    class SmallNode
    {
        public:
            Type x;
            Type y;
            Type node_index;

            SmallNode(Type x, Type y, Type node_index)
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

    // !done! usar Node::Verify
    // essa função encontra os nós vizinhos e devolve um vetor do tamanho adequando (== a quantidade de vizinhos)
    template<typename Type>
    std::vector<Type> ExpandNeighbors (Type current_node_x, Type current_node_y)
    {
        std::vector <Type> my_neighbors_coord;

        SmallNode<Type> neighbors_nodes[4];
        neighbors_nodes[0] = SmallNode<Type>(current_node_x, current_node_y - 1, ((current_node_x * grid_size_y) + current_node_y - 1));
        neighbors_nodes[1] = SmallNode<Type>(current_node_x + 1, current_node_y, ((current_node_x + 1) * grid_size_y + current_node_y));
        neighbors_nodes[2] = SmallNode<Type>(current_node_x, current_node_y + 1, ((current_node_x * grid_size_y) + current_node_y + 1));
        neighbors_nodes[3] = SmallNode<Type>(current_node_x - 1, current_node_y, ((current_node_x - 1) * grid_size_y + current_node_y));

        // verificar os vizinhos no sentido horário
        for (Type i = 0; i < 4; ++i)
        {
            bool valid_coord = Node<Type>::VerifyCoordinate(neighbors_nodes[i].x, neighbors_nodes[i].y);
            bool valid_index = Node<Type>::VerifyIndex(neighbors_nodes[i].node_index);

            if (valid_coord && valid_index)
            {
                my_neighbors_coord.push_back(neighbors_nodes[i].node_index);
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

        return my_neighbors_coord;

        if(debug == true)
        {
            std::cout << "\n\n";
        }
    }

#endif