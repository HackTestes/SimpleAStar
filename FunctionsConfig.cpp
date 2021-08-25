// Esse arquivo contém funções de configuração

#include <unordered_set>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include"json/single_include/nlohmann/json.hpp"
#include"AStarHeader.h"

// !todo! adicionar supporte para opções com uma única letra "-h"
// !todo! melhorar opções que recebem valores (Snapshot, SnapshotXY)
// - verificar a entrada de dados (números)
// !todo! adicionar um Snapshot/SnapshotXY all: imprimir para todos os nós
// !todo! warning flag?
// !todo! show JSON config info (ex.: file path)
void ArgsOptions(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {

        if ((std::string)argv[i] == "--help")
        {
            std::cout << "./SimpleAStarExecutable [gid_size_x] [gid_size_y] [START_X-START_Y] [GOAL_X-GOAL_Y]\n\n"
                      << "Options:\n\n"
                      << "--help\n"
                      << "--Debug\n"
                      << "--DebugAll\n"
                      << "--BestPathIndex\n"
                      << "--ShowPriorityQueue\n"
                      << "--ShowVisitedNeighbors\n"
                      << "--Snapshot [snapshot_start_node_index] [snapshot_end_node_index]\n"
                      << "--SnapshotXY [snapshot_start_node_x]-[snapshot_end_node_x] [snapshot_start_node_y]-[snapshot_end_node_y]\n"
                      << "--Interactive\n"
                      << "--ShowMap\n"
                      << "--ShowBarrier\n"
                      << "--BarrierFilePath [barrier_file_path]\n" // !todo! retirar - depreciado
                      << "--Padding [padding_cell_size]\n"
                      << "--JsonConfig [json_config_file_path]\n";
            std::exit(0); // qual o código para --help?
        }

        else if ((std::string)argv[i] == "--Debug")
        {
            debug = true;
            std::cout << "Debug mode enabled!\n";
            continue;
        }

        else if ((std::string)argv[i] == "--DebugAll")
        {
            debug_all = true;
            std::cout << "Debug all mode enabled!\n";
            continue;
        }

        else if ((std::string)argv[i] == "--BestPathIndex")
        {
            best_path_index = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowPriorityQueue")
        {
            show_priority_queue = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowVisitedNeighbors")
        {
            show_visited_neighbors = true;
            continue;
        }

        else if ((std::string)argv[i] == "--Snapshot")
        {
            snapshot = true;

            snapshot_start_node_index = std::abs(std::stoi(argv[i + 1]));
            snapshot_end_node_index = std::abs(std::stoi(argv[i + 2]));

            i = i + 2;
            continue;
        }

        // !todo! adicionar o ParserXY
        else if ((std::string)argv[i] == "--SnapshotXY")
        {
            snapshot = true;

            // X-X : start-end
            snapshot_start_node_x = std::abs(std::stoi(&argv[i + 1][0]));
            snapshot_end_node_x = std::abs(std::stoi((&argv[i + 1][2])));

            // Y-Y : start-end
            snapshot_start_node_y = std::abs(std::stoi(&argv[i + 2][0]));
            snapshot_end_node_y = std::abs(std::stoi((&argv[i + 2][2])));
            std::cout << "snapshot_start_node_x :  " << snapshot_start_node_x << " | snapshot_end_node_x:  " << snapshot_end_node_x << "\n"
            << "snapshot_start_node_y :  " << snapshot_start_node_y << " | snapshot_end_node_y:  " << snapshot_end_node_y<< "\n\n";

            i = i + 2;
            continue;
        }

        else if ((std::string)argv[i] == "--Interactive")
        {
            interactive = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowMap")
        {
            show_map = true;
            continue;
        }

        else if ((std::string)argv[i] == "--ShowBarrier")
        {
            show_barrier = true;
            continue;
        }

        // !todo! retirar - depreciado
        else if ((std::string)argv[i] == "--BarrierFilePath")
        {
            barrier_enabled = true;
            barrier_file_path = (std::string)argv[i + 1];
            i = i + 1;
            continue;
        }

        else if ((std::string)argv[i] == "--Padding")
        {
            padding_cell_size = std::stoi(argv[i + 1]);
            i = i + 1;
            continue;
        }

        else if ((std::string)argv[i] == "--JsonConfig")
        {
            json_config_enabled = true;
            json_config_file_path = (std::string)argv[i + 1];
            i = i + 1;
            JsonConfig();
            std::cout << "JSON";
            continue;
        }

        else if (i == 0)
        {
            continue;
        }

        // os parâmetros iniciais (tamanho, início e fim) são estáticos
        if (!json_config_enabled)
        {
            if ( (i == 1) && std::isdigit(*(argv[i])) )
            {
                SetGirdSizeX(std::stoi(argv[i]));
            }

            else if ( (i == 2) && std::isdigit(*(argv[i])) )
            {
                SetGirdSizeY(std::stoi(argv[i]));
            }

            else if ( (i == 3) && std::isdigit(*(argv[i])) )
            {
                SetStart(ParserXY(argv[i], "-").x, ParserXY(argv[i], "-").y);
            }

            else if ( (i == 4) && std::isdigit(*(argv[i])) )
            {
                SetGoal(ParserXY(argv[i], "-").x, ParserXY(argv[i], "-").y);
            }
        }

        else
        {
            std::cout << "Invalid option : " << (std::string)argv[i] << "\n";
            std::exit(1);
        }
    }
}

void SetStart(long start_x, long start_y)
{
    long start_index = Node::GetIndex(start_x, start_y);

    if ( Node::VerifyCoordinate(start_x, start_y) && Node::VerifyIndex(start_index) )
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
    long goal_index = Node::GetIndex(goal_x, goal_y);

    if ( Node::VerifyCoordinate(goal_x, goal_y) && Node::VerifyIndex(goal_index) )
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
        std::cout << "WARNING: Gird size X is negative. Using absolute value\n";
    }

    grid_size_x = std::abs(size_x);
}

void SetGirdSizeY(long size_y)
{
    if (size_y < 0)
    {
        std::cout << "WARNING: Gird size Y is negative. Using absolute value\n";
    }

    grid_size_y = std::abs(size_y);
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

    SetGirdSizeX(configuration["Grid size X"]);
    SetGirdSizeY(configuration["Grid size Y"]);

    SetStart(configuration["Start"][0], configuration["Start"][1]);
    SetGoal(configuration["Goal"][0], configuration["Goal"][1]);

    for (long i = 0; i < configuration["Barrier coordinates"].size(); ++i)
    {
        long barrier_x = configuration["Barrier coordinates"][i][0];
        long barrier_y = configuration["Barrier coordinates"][i][1];
        long barrier_index = Node::GetIndex(barrier_x, barrier_y);

        // Vefifica se é uma barreira válida
        if ( Node::VerifyCoordinate(barrier_x, barrier_y) && Node::VerifyIndex(barrier_index) )
        {
            barrier.insert(Node::GetIndex(barrier_x, barrier_y));
        }
        else
        {
            // A barreira simplesmente não será inserida, não é necessário encerrar o programa
            // Um aviso será envido apenas para se saber do problema
            std::cout << "WARNING: Invalid barrier - " << configuration["Barrier coordinates"][i] << "\n";
        }
    }

    if (barrier.find(START) != barrier.end())
    {
        std::cout << "WARNING: Barrier overlaps the START node\n";
    }

    if (barrier.find(GOAL) != barrier.end())
    {
        std::cout << "WARNING: Barrier overlaps the GOAL node\n";
    }

    /*
    std::cout << "Grid size X " << grid_size_x << "\n";
    std::cout << "Grid size Y " << grid_size_y << "\n";
    std::cout << "Start [" << configuration["Start"][0] << "] [" << configuration["Start"][1] << "]\n";
    std::cout << "Goal [" << configuration["Goal"][0] << "] [" << configuration["Goal"][1] << "]\n";
    std::cout << "Barrier coordinates " << configuration["Barrier coordinates"] << "\n";
    */
}