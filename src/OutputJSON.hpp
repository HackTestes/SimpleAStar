#ifndef OUTPUT_JSON_HPP
#define OUTPUT_JSON_HPP

// Helps to create apropriate templates for different problem types
#ifndef MainSlidingPuzzle_H
#define SELECT_TEMPLATE template<typename ProblemTypeKey>
#define SELECT_STRINGIFY_PROBLEM_KEY StringifyProblemKeyNodeMap<ProblemTypeKey>(compressed_best_path, problem_type);
#endif

#ifndef MAIN_NODE_MAP_H
#define SELECT_TEMPLATE template<typename ProblemTypeKey, typename ProblemItemType>
#define SELECT_STRINGIFY_PROBLEM_KEY StringifyProblemKeySlidingPuzzle<ProblemTypeKey,ProblemItemType>(compressed_best_path, problem_type);
#endif


    template<typename ProblemTypeKey>
    std::string StringifyProblemKeyNodeMap(std::vector<ProblemTypeKey> compressed_best_path, std::string problem_type)
    {
        std::string result_json = "";

        result_json += "\t\"Best_path\": \n\t[\n";

        for (ProblemTypeKey best_path_index : compressed_best_path)
        {
            result_json += "\t\t[" + std::to_string(Node<ProblemTypeKey>::GetX(best_path_index)) + ", "
                            + std::to_string(Node<ProblemTypeKey>::GetY(best_path_index)) + "],\n";
        }
        result_json.erase( result_json.size() - 2, 1);
        result_json += "\t] \n";


        return result_json;
    }



    template<typename ProblemTypeKey, typename ProblemItemType>
    std::string StringifyProblemKeySlidingPuzzle(std::vector<ProblemTypeKey> compressed_best_path, std::string problem_type)
    {
        std::string result_json = "";

        result_json += "\t\"Best_path\": \n\t[\n";

        for (ProblemTypeKey sliding_puzzle : compressed_best_path)
        {
            result_json += "\t\t[";

            for (int i = 0; i < sliding_puzzle.size(); ++i)
            {
                uint8_t sliding_puzzle_item = sliding_puzzle[i];
                result_json += std::to_string(sliding_puzzle_item) + ",";
            }

            result_json.pop_back();
            result_json += "], \n";
        }

        result_json.erase( result_json.size() - 3, 1);
        result_json += "\t] \n";

        return result_json;
    }


    template<typename ProblemTypeKey, typename ProblemItemType = ProblemTypeKey>
    static std::string GenerateHeader(long long map_size,
                                        long long travel_distance,
                                        long long expanded_nodes,
                                        std::string problem_type)
    {
        std::string result_json;

        result_json += "\t\"Problem_type\": \"" + problem_type + "\",\n" +
                        "\t\"Grid_size_X\": " + std::to_string(grid_size_x) + ",\n" +
                        "\t\"Grid_size_Y\": " + std::to_string(grid_size_y) + ",\n" +
                        "\t\"Start\": " + json_input_string_values.start + ",\n" +
                        "\t\"Goal\": " + json_input_string_values.goal + ",\n" +
                        "\t\"Barrier\": " + json_input_string_values.barrier + ",\n" +
                        "\t\"Travel_distance\": " + std::to_string(travel_distance) + ",\n" +
                        "\t\"Visited\": " + std::to_string(map_size) + ",\n" +
                        "\t\"Expanded\": " + std::to_string(expanded_nodes) + ",\n";

        return result_json;
    }


    SELECT_TEMPLATE
    std::string ShowJsonStatistics (long long map_size,
                                          long long travel_distance,
                                          std::vector<ProblemTypeKey> compressed_best_path,
                                          long long expanded_nodes,
                                          std::string problem_type)
    {
        // Final Statistics
        std::string final_result_json;

        final_result_json += "{\n";

        final_result_json += GenerateHeader<ProblemTypeKey>(map_size, travel_distance, expanded_nodes, problem_type);

        final_result_json += SELECT_STRINGIFY_PROBLEM_KEY

        final_result_json += "}\n";

        return final_result_json;
    }

#undef SELECT_TEMPLATE
#undef SELECT_STRINGIFY_PROBLEM_KEY
#endif