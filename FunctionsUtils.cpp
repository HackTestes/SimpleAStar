// Esse arquivo contém funções "utilitárias" para o funcionamento do código principal

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
class SmallNode
{
    public:
        long x;
        long y;
        long node_index;

        SmallNode(long x, long y, long node_index)
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
std::vector<long> ExpandNeighbors (long current_node_x, long current_node_y)
{
    std::vector <long> my_neighbors_coord;

    SmallNode neighbors_nodes[4];
    neighbors_nodes[0] = SmallNode(current_node_x, current_node_y - 1, ((current_node_x * grid_size_y) + current_node_y - 1));
    neighbors_nodes[1] = SmallNode(current_node_x + 1, current_node_y, ((current_node_x + 1) * grid_size_y + current_node_y));
    neighbors_nodes[2] = SmallNode(current_node_x, current_node_y + 1, ((current_node_x * grid_size_y) + current_node_y + 1));
    neighbors_nodes[3] = SmallNode(current_node_x - 1, current_node_y, ((current_node_x - 1) * grid_size_y + current_node_y));

    // verificar os vizinhos no sentido horário
    for (long i = 0; i < 4; ++i)
    {
        bool valid_coord = Node::VerifyCoordinate(neighbors_nodes[i].x, neighbors_nodes[i].y);
        bool valid_index = Node::VerifyIndex(neighbors_nodes[i].node_index);

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

// gera os espaços necessário para fazer um padding 
std::string StringPadding(long string_length)
{
    long padding = padding_cell_size - string_length;
    std::string padding_text;

    for (long i = 0; i < padding; ++i)
    {
        padding_text += " ";
    }

    return padding_text;
}

// imprimo meu mapa de nós usando uma cópia (parecido com um snapshot do momento)
// será construída uma string de baixo para cima em cada linha (cima para baixo, esquerda para direita)
void PrintMap (std::unordered_map <long, Node> map, std::unordered_set<long> barrier_map)
{
    std::string map_string;
    std::string line;

    // construa a primeira linha com os índices de X
    line = StringPadding(1) + "|";
    for (long x = 0; x < grid_size_x; ++x)
    {
        std::string coordinate_x = std::to_string(x);
        line += StringPadding(coordinate_x.length()) + coordinate_x;
    }
    line += "\n\n\n";
    map_string = line;

    // separador
    line = StringPadding(1) + "|";
    for (long x = 0; x < grid_size_x; ++x)
    {
        line += StringPadding(1) + "_";
    }
    line += "\n\n\n";
    map_string = line + map_string; // faz em append no começo, ao invés de no final

    // constrói todo o restante do mapa, cada linha sendo um Y
    for (long y = 0; y < grid_size_y; ++y)
    {
        std::string coordinate_y = std::to_string(y);
        line = StringPadding( (coordinate_y.length() + 1) ) + coordinate_y + "|";
        for (long x = 0; x < grid_size_x; ++x)
        {
            long index = (x * grid_size_y) + y;


            if (barrier_map.find(index) != barrier_map.end())
            {
                line += StringPadding(1) + " ";
            }
            else if (map.find(index) != map.end())
            {
                std::string node_appearance = map[index].appearance;
                line += StringPadding(node_appearance.length()) + node_appearance;
            }
            else
            {
                line += StringPadding(1) + "o";
            }

        }
        line += "\n\n\n";
        map_string = line + map_string;
    }

    std::cout << map_string;
}

void ShowBarrier(std::unordered_set<long> my_barrier)
{
    std::string barrier_indexes = "Barrier indexes :  ";

    for (long barrier_index : my_barrier)
    {
        barrier_indexes += std::to_string(barrier_index) + ", ";
    }

    barrier_indexes = barrier_indexes.substr(0, barrier_indexes.size()-2);
    barrier_indexes += "\n";

    std::cout << barrier_indexes;
}

std::pair<long, long> CoordinateParser(std::string string_coordinate, std::string separator)
{
    long first, second;
    long separator_pos = string_coordinate.find(separator);
    long end_line_pos = string_coordinate.size();

    first = std::stoi(string_coordinate.substr(0, separator_pos));
    second = std::stoi(string_coordinate.substr(separator_pos + 1, end_line_pos - separator_pos));

    return std::make_pair(first, second);
}

// !done! ParsedNode => ParsedNode
ParsedNode::ParsedNode(long x, long y)
{
    this->index = Node::GetIndex(x, y);
    this->x = x;
    this->y = y;
}

ParsedNode ParserXY(std::string string_coordinate, std::string separator)
{
    std::pair<long, long>coordinate_pair = CoordinateParser(string_coordinate, separator);

    return ParsedNode(coordinate_pair.first, coordinate_pair.second);
}