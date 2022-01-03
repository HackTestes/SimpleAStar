#ifndef SlidingPuzzle_H
#define SlidingPuzzle_H

    // Esse arquivo trata de implementações específicas para resolver um Sliding Puzzle
    #include <vector>

    template<typename ListType, typename IntType>
    struct SlidingPuzzle
    {
        ListType sliding_puzzle;
        IntType empty_index; // empty cell index
        IntType g;
        ListType came_from;
        bool visited;
        bool in_priority_queue; // otimização de velocidade em busca

        SlidingPuzzle(ListType sliding_puzzle)
        {
            this->sliding_puzzle = sliding_puzzle;
            this->empty_index = SlidingPuzzle::CalculateEmptyCell(sliding_puzzle);

            this->g = 0;
            this->visited = false;
            this->in_priority_queue = false;
            this->came_from = {0};
        }

        SlidingPuzzle()
        {
            this->sliding_puzzle = sliding_puzzle;
            this->empty_index = SlidingPuzzle::CalculateEmptyCell(sliding_puzzle);

            this->g = 0;
            this->visited = false;
            this->in_priority_queue = false;
            this->came_from = {0};
        }

        bool operator==(const ListType& v) const
        {
            return sliding_puzzle == v;
        }

        IntType CalculateEmptyCell(ListType &current_map)
        {
            for(IntType position = 0; position < current_map.size(); ++position)
            {
                IntType cell_value = current_map[position];

                if(cell_value == 0)
                {
                    return position;
                }
            }

            return -1;
        }

        // !todo! Mudar nome para Obj TO position
        std::vector<IntType> CreateObjPositionList()
        {
            std::vector<IntType> obj_position_mapping (sliding_puzzle.size());

            for (IntType position = 0; position < sliding_puzzle.size(); ++position)
            {
                IntType obj = sliding_puzzle[position];
                obj_position_mapping[obj] = position;
            }

            return obj_position_mapping;
        }

        // !todo! Aumentar a reutilização de código
        // cell_length: tamanho horzontal e refere-se ao espaços dentro da célula
        // cell_height: tamanho vertical e refere-se a quantidade de linhas que a célula ocupa no total, ou seja, considera mais que apenas os espaçoes brancos
        std::string PrintCurrentSlidingPuzzle(IntType cell_length, IntType cell_height)
        {
            // #WARNING# Perigoso! Variável global sendo modificada
            padding_cell_size = cell_length;

            IntType empty_cell_coordinate_x = Node<IntType>::GetX(empty_index);
            IntType empty_cell_coordinate_y = Node<IntType>::GetY(empty_index);

            std::string empty_symbol = " ";

            std::string sliding_puzzle_map;

            for(IntType y = 0; y < grid_size_y; ++y)
            {
                std::string grid_line = "";

                std::string cell_top_line = "";
                std::string cell_top_edge_symbol = ".";
                std::string line_symbol = "_";
                // Processo para construir uma linha de células


                // Linha superior ".________."
                for(IntType x = 0; x < grid_size_x; ++x)
                {
                    if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) )
                    {
                        cell_top_line += cell_top_edge_symbol;
                        for(IntType j = 1; j <= cell_length; ++j)
                        {
                            cell_top_line += line_symbol;
                        }
                        cell_top_line += cell_top_edge_symbol;
                    }

                    else
                    {
                        cell_top_line += empty_symbol;
                        for(IntType j = 1; j <= cell_length; ++j)
                        {
                            cell_top_line += empty_symbol;
                        }
                        cell_top_line += empty_symbol;
                    }
                }
                cell_top_line += "\n";
                grid_line += cell_top_line;


                // Meio "|    |"
                IntType middle = (cell_height/2) + 1;
                std::string left_right_symbol = "|";
                std::string cell_body_line = "";

                // (cell_height - 2) : subentendido o topo e o fim
                for (IntType j = 2; j <= (cell_height - 1); ++j)
                {
                    for(IntType x = 0; x < grid_size_x; ++x)
                    {
                        if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) ) {cell_body_line += left_right_symbol;}
                        else {cell_body_line += empty_symbol;}

                        if(j == middle)
                        {
                            std::string item;

                            //show_sliding_puzlle_coordinate
                            if(false)
                            {
                                item = "x:" + std::to_string(x) + " y:" + std::to_string(y);
                            }
                            else
                            {
                                IntType item_index = Node<IntType>::GetIndex(x, y);
                                item = std::to_string( this->sliding_puzzle[item_index] );
                            }
                            std::string center = std::string((cell_length - item.length()) / 2, ' ') + item;

                            //cell_body_line += StringPadding( item.length() ) + item;
                            cell_body_line += center + StringPadding( center.length() );
                        }
                        else
                        {
                            cell_body_line += StringPadding(0);
                        }
                        if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) ) {cell_body_line += left_right_symbol;}
                        else {cell_body_line += empty_symbol;}
                    }
                    cell_body_line += "\n";
                    grid_line += cell_body_line;

                    cell_body_line = ""; //Reset
                }


                // Linha inferior "|_____________|"
                std::string bottom_line = "";

                for(IntType x = 0; x < grid_size_x; ++x)
                {
                    if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) )
                    {
                        bottom_line += left_right_symbol;
                        for(IntType j = 1; j <= cell_length; ++j)
                        {
                            bottom_line += line_symbol;
                        }
                        bottom_line += left_right_symbol;
                    }

                    else
                    {
                        bottom_line += empty_symbol;
                        for(IntType j = 1; j <= cell_length; ++j)
                        {
                            bottom_line += empty_symbol;
                        }
                        bottom_line += empty_symbol;
                    }
                }
                bottom_line += "\n";
                grid_line += bottom_line;

                sliding_puzzle_map = grid_line + sliding_puzzle_map;
            }

            //std::cout << sliding_puzzle_map;

            return sliding_puzzle_map;
        }

    };

    /*template<typename Type, typename IntType>
    Type CreateSlidingPuzzleFromNeighbors(Type current_sliding_puzzle, std::vector<IntType> neighbors_indexes, IntType empty_position_index)
    {
        std::vector< std::vector<IntType> > neighbors_keys ( neighbors_indexes.size() );

        for(IntType i = 0; i < neighbors_indexes.size(); ++i)
        {
            // copy the original vector
            neighbors_keys[i] = current_sliding_puzzle;

            IntType current_neighbor_index = neighbors_indexes[i];
            IntType neighbor_value = current_sliding_puzzle[current_neighbor_index];

            //Swap places with the neighbor
            neighbors_keys[i][empty_position_index] = neighbor_value;
            neighbors_keys[i][current_neighbor_index] = 0;
        }

        return neighbors_keys;
    }*/

    template<typename ListType, typename IntType>
    ListType CreateVectorFromNeighbor(ListType current_sliding_puzzle, IntType neighbor_index, IntType empty_position_index)
    {
        // copy the original vector
        ListType neighbor_sliding_puzzle = current_sliding_puzzle;
        IntType neighbor_value = current_sliding_puzzle[neighbor_index];

        //Swap places with the neighbor
        neighbor_sliding_puzzle[empty_position_index] = neighbor_value;
        neighbor_sliding_puzzle[neighbor_index] = 0;

        return neighbor_sliding_puzzle;
    }
#endif