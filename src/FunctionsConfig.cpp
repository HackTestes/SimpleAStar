// Esse arquivo contém funções de configuração
#include <unordered_set>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include"json/single_include/nlohmann/json.hpp"
#include"AStarHeader.h"

void StoreJsonObjectStringValues(nlohmann::json parsed_json_configuration)
{
    json_input_string_values.problem_type = parsed_json_configuration["Problem_type"].dump();
    json_input_string_values.grid_size_x = parsed_json_configuration["Grid_size_X"].dump();
    json_input_string_values.gird_size_Y = parsed_json_configuration["Grid_size_Y"].dump();
    json_input_string_values.start = parsed_json_configuration["Start"].dump();
    json_input_string_values.goal = parsed_json_configuration["Goal"].dump();

    if(parsed_json_configuration["Problem_type"] == "Node Map")
    {
        json_input_string_values.barrier = parsed_json_configuration["Barrier"].dump();
    }
}

void SetStart(long long start_x, long long start_y)
{
    long long start_index = Node<long long>::GetIndex(start_x, start_y);

    if ( Node<long long>::VerifyCoordinate(start_x, start_y) && Node<long long>::VerifyIndex(start_index) )
    {
        START = start_index;
    }
    else
    {
        std::cout << "The start is out of bounds!!!\n";
        std::exit(0);
    }
}

void SetGoal(long long goal_x, long long goal_y)
{
    long long goal_index = Node<long long>::GetIndex(goal_x, goal_y);

    if ( Node<long long>::VerifyCoordinate(goal_x, goal_y) && Node<long long>::VerifyIndex(goal_index) )
    {
        GOAL = goal_index;
    }
    else
    {
        std::cout << "The goal is out of bounds!!!\n";
        std::exit(0);
    }
}

void SetGirdSizeX(long long size_x)
{
    if (size_x < 0)
    {
        std::cout << "WARNING: Gird size X is negative. Using absolute value insted\n";
    }

    grid_size_x = std::abs(size_x);
}

void SetGirdSizeY(long long size_y)
{
    if (size_y < 0)
    {
        std::cout << "WARNING: Gird size Y is negative. Using absolute value insted\n";
    }

    grid_size_y = std::abs(size_y);
}

void ConfigNodeMap(nlohmann::json parsed_json_configuration)
{
    SetGirdSizeX(parsed_json_configuration["Grid_size_X"]);
    SetGirdSizeY(parsed_json_configuration["Grid_size_Y"]);

    SetStart(parsed_json_configuration["Start"][0], parsed_json_configuration["Start"][1]);
    SetGoal(parsed_json_configuration["Goal"][0], parsed_json_configuration["Goal"][1]);

    for (long i = 0; i < parsed_json_configuration["Barrier"].size(); ++i)
    {
        long long barrier_x = parsed_json_configuration["Barrier"][i][0];
        long long barrier_y = parsed_json_configuration["Barrier"][i][1];
        long long barrier_index = Node<long long>::GetIndex(barrier_x, barrier_y);

        // Vefifica se é uma barreira válida
        if ( Node<long long>::VerifyCoordinate(barrier_x, barrier_y) && Node<long long>::VerifyIndex(barrier_index) )
        {
            barrier.insert(Node<long long>::GetIndex(barrier_x, barrier_y));
        }
        else if (warning_enabled)
        {
            // A barreira simplesmente não será inserida, não é necessário encerrar o programa
            // Um aviso será envido apenas para se saber do problema
            std::cout << "WARNING: Invalid barrier - " << parsed_json_configuration["Barrier"][i] << "\n";
        }
    }

    if (barrier.find(START) != barrier.end())
    {
        std::cout << "WARNING: Barrier overlaps the START node\n";
        std::exit(1);
    }

    if (barrier.find(GOAL) != barrier.end())
    {
        std::cout << "WARNING: Barrier overlaps the GOAL node\n";
        std::exit(1);
    }

    /*
    std::cout << "Grid_size_X " << grid_size_x << "\n";
    std::cout << "Grid_size_Y " << grid_size_y << "\n";
    std::cout << "Start [" << parsed_json_configuration["Start"][0] << "] [" << parsed_json_configuration["Start"][1] << "]\n";
    std::cout << "Goal [" << parsed_json_configuration["Goal"][0] << "] [" << parsed_json_configuration["Goal"][1] << "]\n";
    std::cout << "Barrier " << parsed_json_configuration["Barrier"] << "\n";
    */

    StoreJsonObjectStringValues(parsed_json_configuration);
}

void SetSlidingPuzzleStartGoal(std::vector<long long> json_sliding_puzzle_start, std::vector<long long> json_sliding_puzzle_goal)
{
    // Checar se os tamanhos são compatíveis
    if (json_sliding_puzzle_goal.size() != json_sliding_puzzle_start.size())
    {
        std::cout << "Start and goal have different sizes\n";
        std::exit(1);
    }

    sliding_puzzle_start = json_sliding_puzzle_start;
    sliding_puzzle_goal = json_sliding_puzzle_goal;

    /*for (long i = 0; i < sliding_puzzle_start.size(); ++i)
    {
        std::cout << "sliding_puzzle_start [" << i << "] =  " << sliding_puzzle_start[i] << "\n";
    }

    for (long i = 0; i < sliding_puzzle_goal.size(); ++i)
    {
        std::cout << "sliding_puzzle_goal [" << i << "] =  " << sliding_puzzle_goal[i] << "\n";
    }*/
}


void ConfigSlidingPuzzle(nlohmann::json parsed_json_configuration)
{
    // !todo! Adicionar suporte para SlidingPuzzle
    // - Criar um fluxo de execução alternativo
    // - Desmembrar cada uma das configurações específicas em funções

    sliding_puzzle_enabled = true;
    node_map_enabled = false;

    SetGirdSizeX(parsed_json_configuration["Grid_size_X"]);
    SetGirdSizeY(parsed_json_configuration["Grid_size_Y"]);

    SetSlidingPuzzleStartGoal(parsed_json_configuration["Start"], parsed_json_configuration["Goal"]);

    StoreJsonObjectStringValues(parsed_json_configuration);

}

void JsonConfig()
{
    std::string json_config;
    std::string json_line;
    std::ifstream json_config_file (json_config_file_path);

    if (json_config_file.is_open())
    {
        while (getline(json_config_file, json_line))
        {
            json_config += json_line;
        }
        json_config_file.close();
    }
    else
    {
        std::cout << "Cannot open JSON file\n";
        std::exit(1);
    }

    nlohmann::json configuration = nlohmann::json::parse(json_config);

    // Problem
    if (configuration["Problem_type"] == "Node Map")
    {
        ConfigNodeMap(configuration);
    }
    else if (configuration["Problem_type"] == "Sliding Puzzle")
    {
        ConfigSlidingPuzzle(configuration);
    }
    else
    {
        std::cout << "Invalid problem type\n";
        std::exit(1);
    }
}
