// Esse arquivo trata de implementações específicas para resolver um Sliding Puzzle

#include"AStarHeader.h"

/*
// Key proposal
int_64 key[]; // Armazena partes da chave de acordo com o tamaho máximo de bits

// Key build process proposal
    - Cada parte da chave ocupa uma posição na lista "key[]"
    - Cada parte da chave tem a maior quantidade de bits possíveis (key[0] == 64 bits)
    - Cada parte da chave é composta por várias posições da tabela, ou seja, 1 posição de "key[]" guarda várias posições da tabela
    (Avaliar números quebrados ou não pares, podem desperdiçar bits)
    - Cada fração da chave recebe uma quantidade de bits mínima para representar todas as posições
    - A fração de cada posição da "key[]" (quantas posições da tabela uma posição da "key[]" representa): key_fraction = ??????
        loge(n) / loge(2) //--// 10/log(2)
        https://stackoverflow.com/questions/7150035/calculating-bits-required-to-store-decimal-number 
    - Quantidade de níveis: key.length

// Search key proposal
vector< vector<int_64> > map; // A busca é feita em níveis, cada nível representando um pedaço da chave (ex.: 64 bits -> 64 -> 64 ...)
*/


SlidingPuzzle::SlidingPuzzle (std::vector<long> sliding_puzzle)
{
    this->map_key = 0;
    this->sliding_puzzle = sliding_puzzle;
    this->empty_index = SlidingPuzzle::CalculateEmptyCell(sliding_puzzle);

    this->g = 0;
    this->visited = false;
    this->in_priority_queue = false;
    this->came_from = 0;
}

SlidingPuzzle::SlidingPuzzle ()
{
    this->map_key = 0;
    this->sliding_puzzle = sliding_puzzle;
    this->empty_index = SlidingPuzzle::CalculateEmptyCell(sliding_puzzle);

    this->g = 0;
    this->visited = false;
    this->in_priority_queue = false;
    this->came_from = 0;
}

long SlidingPuzzle::CalculateEmptyCell(std::vector<long> &current_map)
{
    for(long position = 0; position < current_map.size(); ++position)
    {
        long cell_value = current_map[position];

        if(cell_value == 0)
        {
            return position;
        }
    }
}

bool SlidingPuzzle::operator==(const std::vector<long>& v) const
{
    return sliding_puzzle == v;
}

std::vector<long> SlidingPuzzle::CreateObjPositionList()
{
    std::vector<long> obj_position_mapping (sliding_puzzle.size());

    for (long position = 0; position < sliding_puzzle.size(); ++position)
    {
        long obj = sliding_puzzle[position];
        obj_position_mapping[obj] = position;
    }

    return obj_position_mapping;
}


// !todo! Aumentar a reutilização de código
// cell_length: tamanho horzontal e refere-se ao espaços dentro da célula
// cell_height: tamanho vertical e refere-se a quantidade de linhas que a célula ocupa no total, ou seja, considera mais que apenas os espaçoes brancos
std::string SlidingPuzzle::PrintCurrentSlidingPuzzle (long cell_length, long cell_height)
{
    // #WARNING# Perigoso!
    padding_cell_size = cell_length;

    long empty_cell_coordinate_x = Node::GetX(empty_index);
    long empty_cell_coordinate_y = Node::GetY(empty_index);

    std::string empty_symbol = " ";

    std::string sliding_puzzle_map;

    for(long y = 0; y < grid_size_y; ++y)
    {
        std::string grid_line = "";

        std::string cell_top_line = "";
        std::string cell_top_edge_symbol = ".";
        std::string line_symbol = "_";
        // Processo para construir uma linha de células


        // Linha superior ".________."
        for(long x = 0; x < grid_size_x; ++x)
        {
            if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) )
            {
                cell_top_line += cell_top_edge_symbol;
                for(long j = 1; j <= cell_length; ++j)
                {
                    cell_top_line += line_symbol;
                }
                cell_top_line += cell_top_edge_symbol;
            }

            else
            {
                cell_top_line += empty_symbol;
                for(long j = 1; j <= cell_length; ++j)
                {
                    cell_top_line += empty_symbol;
                }
                cell_top_line += empty_symbol;
            }
        }
        cell_top_line += "\n";
        grid_line += cell_top_line;


        // Meio "|    |"
        long middle = (cell_height/2) + 1;
        std::string left_right_symbol = "|";
        std::string cell_body_line = "";

        // (cell_height - 2) : subentendido o topo e o fim
        for (long j = 2; j <= (cell_height - 1); ++j)
        {
            for(long x = 0; x < grid_size_x; ++x)
            {
                if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) ) {cell_body_line += left_right_symbol;}
                else {cell_body_line += empty_symbol;}

                if(j == middle)
                {
                    std::string item = "x:" + std::to_string(x) + " y:" + std::to_string(y);
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

        for(long x = 0; x < grid_size_x; ++x)
        {
            if ( !(x == empty_cell_coordinate_x && y == empty_cell_coordinate_y) )
            {
                bottom_line += left_right_symbol;
                for(long j = 1; j <= cell_length; ++j)
                {
                    bottom_line += line_symbol;
                }
                bottom_line += left_right_symbol;
            }

            else
            {
                bottom_line += empty_symbol;
                for(long j = 1; j <= cell_length; ++j)
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

    std::cout << sliding_puzzle_map;

    return sliding_puzzle_map;
}
