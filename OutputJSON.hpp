#ifndef OUTPUT_JSON_HPP
#define OUTPUT_JSON_HPP

    template<typename IndexType>
    std::string ShowJsonStatisticsNodeMap(long long map_size,
                                          long long travel_distance,
                                          std::vector<IndexType> compressed_best_path, // !todo! Template vector
                                          long long expanded_nodes,
                                          std::string problem_type)
    {
        // Final Statistics
        std::string final_result_json;

        final_result_json += "{\n";

        final_result_json += "\t\"" + problem_type + " visited\": " + std::to_string(map_size) + ",\n" +
                            "\t\"Travel distance\": " + std::to_string(travel_distance) + ",\n" +
                            "\t\"Best path (compressed format)\": \n\t[\n";

        for (IndexType best_path_index : compressed_best_path)
        {
            final_result_json += "\t\t[" + std::to_string(Node<IndexType>::GetX(best_path_index)) + ", "
                            + std::to_string(Node<IndexType>::GetY(best_path_index)) + "],\n";
        }
        final_result_json += "\t], \n\t\"Expanded " + problem_type + "\": " + std::to_string(expanded_nodes) + "\n";

        final_result_json += "}\n";

        return final_result_json;
    }

#endif