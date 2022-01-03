#ifndef JSON_INPUT_STRING_FORMAT_HPP
#define JSON_INPUT_STRING_FORMAT_HPP

    struct JsonInputStringFormat
    {
        std::string problem_type;
        std::string grid_size_x;
        std::string gird_size_Y;
        std::string start;
        std::string goal;
        std::string barrier;

        JsonInputStringFormat()
        {
            this->problem_type = "";
            this->grid_size_x = "";
            this->gird_size_Y = "";
            this->start = "";
            this->goal = "";
            this->barrier = "[]";
        }

        JsonInputStringFormat(std::string problem_type,
                                std::string grid_size_x,
                                std::string gird_size_Y,
                                std::string start,
                                std::string goal,
                                std::string barrier)
        {
            this->problem_type = problem_type;
            this->grid_size_x = grid_size_x;
            this->gird_size_Y = gird_size_Y;
            this->start = start;
            this->goal = goal;
            this->barrier = barrier;
        }
    };



#endif