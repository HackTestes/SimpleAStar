// Esse arquivo contém funções de configuração
#include <unordered_set>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include"json/single_include/nlohmann/json.hpp"
#include"AStarHeader.h"

void SetStart(long start_x, long start_y)
{
    long start_index = Node<long>::GetIndex(start_x, start_y);

    if ( Node<long>::VerifyCoordinate(start_x, start_y) && Node<long>::VerifyIndex(start_index) )
    {
        START = start_index;
    }
    else
    {
        std::cout << "The start is out of bounds!!!\n";
        std::exit(0);
    }
}

void SetGoal(long goal_x, long goal_y)
{
    long goal_index = Node<long>::GetIndex(goal_x, goal_y);

    if ( Node<long>::VerifyCoordinate(goal_x, goal_y) && Node<long>::VerifyIndex(goal_index) )
    {
        GOAL = goal_index;
    }
    else
    {
        std::cout << "The goal is out of bounds!!!\n";
        std::exit(0);
    }
}

void SetGirdSizeX(long size_x)
{
    if (size_x < 0)
    {
        std::cout << "WARNING: Gird size X is negative. Using absolute value insted\n";
    }

    grid_size_x = std::abs(size_x);
}

void SetGirdSizeY(long size_y)
{
    if (size_y < 0)
    {
        std::cout << "WARNING: Gird size Y is negative. Using absolute value insted\n";
    }

    grid_size_y = std::abs(size_y);
}

void ConfigNodeMap(nlohmann::json parsed_json_configuration)
{
    // !todo! Adicionar suporte para SlidingPuzzle
    // - Criar um fluxo de execução alternativo
    // - Desmembrar cada uma das configurações específicas em funções

    SetGirdSizeX(parsed_json_configuration["Grid size X"]);
    SetGirdSizeY(parsed_json_configuration["Grid size Y"]);

    SetStart(parsed_json_configuration["Start"][0], parsed_json_configuration["Start"][1]);
    SetGoal(parsed_json_configuration["Goal"][0], parsed_json_configuration["Goal"][1]);

    for (long i = 0; i < parsed_json_configuration["Barrier coordinates"].size(); ++i)
    {
        long barrier_x = parsed_json_configuration["Barrier coordinates"][i][0];
        long barrier_y = parsed_json_configuration["Barrier coordinates"][i][1];
        long barrier_index = Node<long>::GetIndex(barrier_x, barrier_y);

        // Vefifica se é uma barreira válida
        if ( Node<long>::VerifyCoordinate(barrier_x, barrier_y) && Node<long>::VerifyIndex(barrier_index) )
        {
            barrier.insert(Node<long>::GetIndex(barrier_x, barrier_y));
        }
        else if (warning_enabled)
        {
            // A barreira simplesmente não será inserida, não é necessário encerrar o programa
            // Um aviso será envido apenas para se saber do problema
            std::cout << "WARNING: Invalid barrier - " << parsed_json_configuration["Barrier coordinates"][i] << "\n";
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
    std::cout << "Grid size X " << grid_size_x << "\n";
    std::cout << "Grid size Y " << grid_size_y << "\n";
    std::cout << "Start [" << parsed_json_configuration["Start"][0] << "] [" << parsed_json_configuration["Start"][1] << "]\n";
    std::cout << "Goal [" << parsed_json_configuration["Goal"][0] << "] [" << parsed_json_configuration["Goal"][1] << "]\n";
    std::cout << "Barrier coordinates " << parsed_json_configuration["Barrier coordinates"] << "\n";
    */
}

void SetSlidingPuzzleStartGoal(std::vector<long> json_sliding_puzzle_start, std::vector<long> json_sliding_puzzle_goal)
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

    SetGirdSizeX(parsed_json_configuration["Grid size X"]);
    SetGirdSizeY(parsed_json_configuration["Grid size Y"]);

    SetSlidingPuzzleStartGoal(parsed_json_configuration["Start"], parsed_json_configuration["Goal"]);

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
    if (configuration["Problem type"] == "Node Map")
    {
        ConfigNodeMap(configuration);
    }
    else if (configuration["Problem type"] == "Sliding Puzzle")
    {
        ConfigSlidingPuzzle(configuration);
    }
    else
    {
        std::cout << "Invalid problem type\n";
        std::exit(1);
    }
}
