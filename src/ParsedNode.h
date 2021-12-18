#ifndef PARSED_NODE_H
#define PARSED_NODE_H

    template<typename IntType>
    struct ParsedNode
    {
        IntType x;
        IntType y;
        IntType index;

        ParsedNode(IntType x, IntType y)
        {
            this->index = Node<IntType>::GetIndex(x, y);
            this->x = x;
            this->y = y;
        }
    };

    template<typename IntType>
    std::pair<IntType, IntType> CoordinateParser(std::string string_coordinate, std::string separator)
    {
        IntType first, second;
        IntType separator_pos = string_coordinate.find(separator);
        IntType end_line_pos = string_coordinate.size();

        first = std::stoi(string_coordinate.substr(0, separator_pos));
        second = std::stoi(string_coordinate.substr(separator_pos + 1, end_line_pos - separator_pos));

        return std::make_pair(first, second);
    }

    template<typename IntType>
    ParsedNode<IntType> ParserXY(std::string string_coordinate, std::string separator)
    {
        std::pair<IntType, IntType>coordinate_pair = CoordinateParser<IntType>(string_coordinate, separator);

        return ParsedNode<IntType>(coordinate_pair.first, coordinate_pair.second);
    }

#endif