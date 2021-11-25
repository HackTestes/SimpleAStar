#ifndef OUTPUT_JSON_HPP
#define OUTPUT_JSON_HPP


    template<typename ProblemTypeKey>
    std::string StringfyProblemKeyNodeMap(std::vector<ProblemTypeKey> compressed_best_path, std::string problem_type)
    {
        std::string result_json = "";

        result_json += "\t\"Best path (compressed format)\": \n\t\t[\n";

        for (ProblemTypeKey best_path_index : compressed_best_path)
        {
            result_json += "\t\t[" + std::to_string(Node<ProblemTypeKey>::GetX(best_path_index)) + ", "
                            + std::to_string(Node<ProblemTypeKey>::GetY(best_path_index)) + "],\n";
        }
        result_json.erase( result_json.size() - 2, 1);
        result_json += "\t\t] \n";


        return result_json;
    }



    template<typename ProblemTypeKey, typename ProblemItemType>
    std::string StringfyProblemKeySlidingPuzzle(std::vector<ProblemTypeKey> compressed_best_path, std::string problem_type)
    {
        std::string result_json = "";

        result_json += "\t\"Best path (compressed format)\": \n\t\t[\n";

        for (ProblemTypeKey sliding_puzzle : compressed_best_path)
        {
            result_json += "\t\t\t[";

            for (int i = 0; i < sliding_puzzle.size(); ++i)
            {
                uint8_t sliding_puzzle_item = sliding_puzzle[i];
                result_json += " " + std::to_string(sliding_puzzle_item) + ", ";
            }

            result_json += "], \n";
        }

        result_json.erase( result_json.size() - 3, 1);
        result_json += "\t\t] \n";

        return result_json;
    }


    template<typename ProblemTypeKey, typename ProblemItemType = ProblemTypeKey>
    static std::string GenerateHeader(long long map_size,
                                        long long travel_distance,
                                        long long expanded_nodes,
                                        std::string problem_type)
    {
        std::string result_json;



        result_json += "\t\"" + problem_type + " visited\": " + std::to_string(map_size) + ",\n" +
                            "\t\"Travel distance\": " + std::to_string(travel_distance) + ",\n";

        result_json += "\n\t\"Expanded " + problem_type + "\": " + std::to_string(expanded_nodes) + "\n";

        return result_json;
    }


    template<typename ProblemTypeKey>
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

        final_result_json += StringfyProblemKeyNodeMap<ProblemTypeKey>(compressed_best_path, problem_type);

        final_result_json += "}\n";

        return final_result_json;
    }


    template<typename ProblemTypeKey, typename ProblemItemType>
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

        final_result_json += StringfyProblemKeySlidingPuzzle<ProblemTypeKey,ProblemItemType>(compressed_best_path, problem_type);

        final_result_json += "}\n";

        return final_result_json;
    }


#endif