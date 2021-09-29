// Esse arquivo contém funções de configuração
#include <unordered_set>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include"json/single_include/nlohmann/json.hpp"
#include"AStarHeader.h"

// TEMPLATE para a reestruturação dos argumentos

class Snapshot
{
    public:
        inline static std::string long_option = "--Snapshot";
        inline static std::string short_option= "-s";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + " [[snapshot_start_node_index] [snapshot_end_node_index]|all]  Filter the desired nodes based on their index\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            snapshot = true;

            if ((std::string)argv[current_arg + 1] == "all")
            {
                snapshot_start_node_index = 0;
                snapshot_end_node_index = grid_size_x * grid_size_y;

                return 1;
            }
            else
            {
                snapshot_start_node_index = std::abs(std::stoi(argv[current_arg + 1]));
                snapshot_end_node_index = std::abs(std::stoi(argv[current_arg + 2]));

                return 2;
            }
        }
};

class SnapshotXY
{
    public:
        inline static std::string long_option = "--SnapshotXY";
        inline static std::string short_option= "-S";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + " [snapshot_start_node_x]-[snapshot_end_node_x] [snapshot_start_node_y]-[snapshot_end_node_y]  Filter the desired nodes based on their coordinates\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            snapshot = true;

            std::pair<long, long> start_coordinates_pair_x = CoordinateParser(argv[current_arg + 1], "-");
            std::pair<long, long> start_coordinates_pair_y = CoordinateParser(argv[current_arg + 2], "-");

            // X-X : start-end
            snapshot_start_node_x = start_coordinates_pair_x.first;
            snapshot_end_node_x = start_coordinates_pair_x.second;

            // Y-Y : start-end
            snapshot_start_node_y = start_coordinates_pair_y.first;
            snapshot_end_node_y = start_coordinates_pair_y.second;

            std::cout << "snapshot_start_node_x :  " << snapshot_start_node_x << " | snapshot_end_node_x:  " << snapshot_end_node_x << "\n"
            << "snapshot_start_node_y :  " << snapshot_start_node_y << " | snapshot_end_node_y:  " << snapshot_end_node_y<< "\n\n";

            return 2;
        }
};

class Debug
{
    public:
        inline static std::string long_option = "--Debug";
        inline static std::string short_option= "-d";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Enable debug output, used in some tests only\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            debug = true;
            std::cout << "Debug mode enabled!\n";

            return 0;
        }
};

class BestPathIndex
{
    public:
        inline static std::string long_option = "--BestPathIndex";
        inline static std::string short_option= "-p";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Every node in the best path will display its index\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            best_path_index = true;

            return 0;
        }
};

class ShowPriorityQueue
{
    public:
        inline static std::string long_option = "--ShowPriorityQueue";
        inline static std::string short_option= "-q";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Show the Priority Queue for each selected node of the Snapshot\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            show_priority_queue = true;

            return 0;
        }
};

class ShowVisitedNeighbors
{
    public:
        inline static std::string long_option = "--ShowVisitedNeighbors";
        inline static std::string short_option= "-n";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Show all the visited neighbors for each selected node of the Snapshot\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            show_visited_neighbors = true;

            return 0;
        }
};

class Interactive
{
    public:
        inline static std::string long_option = "--Interactive";
        inline static std::string short_option= "-i";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Allows to stop execution for each selected node of the Snapshot\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            interactive = true;

            return 0;
        }
};

class ShowMap
{
    public:
        inline static std::string long_option = "--ShowMap";
        inline static std::string short_option= "-m";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Displays the map for each selected node of the Snapshot\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            show_map = true;

            return 0;
        }
};

class ShowBarrier
{
    public:
        inline static std::string long_option = "--ShowBarrier";
        inline static std::string short_option= "-b";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Displays the barrier for each selected node of the Snapshot\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            show_barrier = true;

            return 0;
        }
};

class Padding
{
    public:
        inline static std::string long_option = "--Padding";
        inline static std::string short_option= "-P";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + " [padding_cell_size]  Controls map padding, default value: "+ std::to_string(padding_cell_size) + "\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            padding_cell_size = std::stoi(argv[current_arg + 1]);

            return 1;
        }
};

class WarningEnabled
{
    public:
        inline static std::string long_option = "--WarningEnabled";
        inline static std::string short_option= "-w";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  Enables serveral warnings of non critical problems\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            warning_enabled = true;

            return 0;
        }
};

class Heuristic
{
    public:
        inline static std::string long_option = "--Heuristic";
        inline static std::string short_option= "-H";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + " [heuristic_weight]  Controls heuristic value (heuristic_weight * (dx + dy)), default value: " + std::to_string(heuristic_weight) + "\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            heuristic_weight = std::stoi(argv[current_arg + 1]);

            return 1;
        }
};

class Cost
{
    public:
        inline static std::string long_option = "--Cost";
        inline static std::string short_option= "-c";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + " [cost_weight]  Controls cost value (cost_weight * (dx + dy)), default value: " + std::to_string(cost_weight) + "\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            cost_weight = std::stoi(argv[current_arg + 1]);

            return 1;
        }
};

class JsonConfig
{
    public:
        inline static std::string long_option = "--JsonConfig";
        inline static std::string short_option= "-J";

        static void help()
        {
            std::string help = "\t" + long_option + "  " + short_option + "  [json_config_file_path]  Path to a Json configuration file\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            json_config_enabled = true;
            json_config_file_path = (std::string)argv[current_arg + 1];
            JsonConfig();

            return 1;
        }
};

class DefaultArguments
{
    public:
        inline static std::string long_option = "";
        inline static std::string short_option= "";

        static void help()
        {
            std::string help = "\n  ./SimpleAStarExecutable [gid_size_x] [gid_size_y] [START_X-START_Y] [GOAL_X-GOAL_Y]\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            SetGirdSizeX(std::stoi(argv[current_arg]));

            SetGirdSizeY(std::stoi(argv[current_arg + 1]));

            SetStart(ParserXY(argv[current_arg + 2], "-").x, ParserXY(argv[current_arg + 2], "-").y);

            SetGoal(ParserXY(argv[current_arg + 3], "-").x, ParserXY(argv[current_arg + 3], "-").y);

            return 3;
        }
};


class Help
{
    public:
        inline static std::string long_option = "--help";
        inline static std::string short_option = "-h";

        static void help()
        {
            std::string help =  "\t" + long_option + "  " + short_option + "  Display help text\n\n";
            std::cout << help;
        };

        static long run(int current_arg, char* argv[])
        {
            DefaultArguments::help();
            std::cout << "\tOptions:\n\n";
            Help::help();
            Snapshot::help();
            SnapshotXY::help();
            Debug::help();
            BestPathIndex::help();
            ShowPriorityQueue::help();
            ShowVisitedNeighbors::help();
            Interactive::help();
            ShowMap::help();
            ShowBarrier::help();
            Padding::help();
            WarningEnabled::help();
            Heuristic::help();
            Cost::help();
            JsonConfig::help();
            std::exit(0); // !todo! Perigoso?

            return 0;
        }
};

long ExecuteArg(std::string my_argument, int current_arg, char* argv[])
{
    std::unordered_map<std::string, std::function<long(int current_arg, char* argv[])>> arguments_mapping;

    // Mapping
    // Help
    arguments_mapping[Help::long_option] = &Help::run;
    arguments_mapping[Help::short_option] = &Help::run;

    // DefaultArguments
    arguments_mapping[DefaultArguments::long_option] = &DefaultArguments::run;

    // Snapshot
    arguments_mapping[Snapshot::long_option] = &Snapshot::run;
    arguments_mapping[Snapshot::short_option] = &Snapshot::run;

    // SnapshotXY
    arguments_mapping[SnapshotXY::long_option] = &SnapshotXY::run;
    arguments_mapping[SnapshotXY::short_option] = &SnapshotXY::run;

    // Debug
    arguments_mapping[Debug::long_option] = &Debug::run;
    arguments_mapping[Debug::short_option] = &Debug::run;

    // BestPathIndex
    arguments_mapping[BestPathIndex::long_option] = &BestPathIndex::run;
    arguments_mapping[BestPathIndex::short_option] = &BestPathIndex::run;

    // ShowPriorityQueue
    arguments_mapping[ShowPriorityQueue::long_option] = &ShowPriorityQueue::run;
    arguments_mapping[ShowPriorityQueue::short_option] = &ShowPriorityQueue::run;

    // ShowVisitedNeighbors
    arguments_mapping[ShowVisitedNeighbors::long_option] = &ShowVisitedNeighbors::run;
    arguments_mapping[ShowVisitedNeighbors::short_option] = &ShowVisitedNeighbors::run;

    // Interactive
    arguments_mapping[Interactive::long_option] = &Interactive::run;
    arguments_mapping[Interactive::short_option] = &Interactive::run;

    // ShowMap
    arguments_mapping[ShowMap::long_option] = &ShowMap::run;
    arguments_mapping[ShowMap::short_option] = &ShowMap::run;

    // ShowBarrier
    arguments_mapping[ShowBarrier::long_option] = &ShowBarrier::run;
    arguments_mapping[ShowBarrier::short_option] = &ShowBarrier::run;

    // Padding
    arguments_mapping[Padding::long_option] = &Padding::run;
    arguments_mapping[Padding::short_option] = &Padding::run;

    // WarningEnabled
    arguments_mapping[WarningEnabled::long_option] = &WarningEnabled::run;
    arguments_mapping[WarningEnabled::short_option] = &WarningEnabled::run;

    // Heuristic
    arguments_mapping[Heuristic::long_option] = &Heuristic::run;
    arguments_mapping[Heuristic::short_option] = &Heuristic::run;

    // Cost
    arguments_mapping[Cost::long_option] = &Cost::run;
    arguments_mapping[Cost::short_option] = &Cost::run;

    // JsonConfig
    arguments_mapping[JsonConfig::long_option] = &JsonConfig::run;
    arguments_mapping[JsonConfig::short_option] = &JsonConfig::run;

    if (arguments_mapping.count(my_argument) != 0)
    {
        return arguments_mapping[my_argument](current_arg, argv);
    }
    else
    {
        std::cout << "Invalid option : " << my_argument << "\n";

        return 0;
    }
}

void ArgsOptions(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string argument = argv[i];

        // long option
        if (argument.compare(0, 2, "--") == 0)
        {
            i = i + ExecuteArg(argument, i, argv);
        }

        // short option
        else if(argument.compare(0, 1, "-") == 0)
        {
            // Retira o "-" e depois segmenta todas as letras
            argument.erase(0, 1);
            std::vector<char> character_vec(argument.begin(), argument.end());

            for (char character : character_vec)
            {
                // Adiciona o "-" para todas as letras para serem interpretadas com argumentos separados - AVISO: pode gerar problemas se não usado corretamente
                std::string character_arg = "-" + std::string(1, character);

                i = i + ExecuteArg(character_arg, i, argv);
            }
        }

        // DefaultArguments
        else if( !json_config_enabled && (i == 1) )
        {
            i = i + ExecuteArg("", i, argv);
        }
    }

}




// --------------------------------------- TESTE ACIMA ------------------------------------------ //



// !todo! Adicionar parâmetro obrigatório
// !todo! adicionar supporte para opções com uma única letra "-h"
// !todo! melhorar opções que recebem valores (Snapshot, SnapshotXY)
// - verificar a entrada de dados (números - verificar se são dígitos)
// !done! adicionar um Snapshot/SnapshotXY all: imprimir para todos os nós
// !done! warning flag?
// !todo! Melhorar os warnings?
// !todo! show JSON config info (ex.: file path)????
// Obs: Switch só funciona bem com comparação de inteiros!
void ArgsOptions_old(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if ((std::string)argv[i] == "--help")
        {
            std::cout << "./SimpleAStarExecutable [gid_size_x] [gid_size_y] [START_X-START_Y] [GOAL_X-GOAL_Y]\n\n"
                      << "Options:\n\n"
                      << "--help\n"
                      << "--Debug\n"
                      << "--DebugAll\n" // !todo! Retirar
                      << "--BestPathIndex\n"
                      << "--ShowPriorityQueue\n"
                      << "--ShowVisitedNeighbors\n"
                      << "--Snapshot [[snapshot_start_node_index] [snapshot_end_node_index]|all]\n"
                      << "--SnapshotXY [snapshot_start_node_x]-[snapshot_end_node_x] [snapshot_start_node_y]-[snapshot_end_node_y]\n"
                      << "--Interactive\n"
                      << "--ShowMap\n"
                      << "--ShowBarrier\n"
                      << "--Padding [padding_cell_size]\n"
                      << "--WarningEnabled\n"
                      << "--Heuristic [heuristic_weight]\n"
                      << "--Cost [cost_weight]\n"
                      << "--JsonConfig [json_config_file_path]\n"
                      << "--Test\n\n"; // !todo! Talvez usar unit tests simples


            std::exit(0); // qual o código para --help?
        }

        else if ((std::string)argv[i] == "--Debug")
        {
            debug = true;
            std::cout << "Debug mode enabled!\n";
            continue;
        }

        else if ((std::string)argv[i] == "--DebugAll") // !todo! Retirar
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

            if ((std::string)argv[i + 1] == "all")
            {
                snapshot_start_node_index = 0;
                snapshot_end_node_index = grid_size_x * grid_size_y;

                i = i + 1;
                continue;
            }
            else
            {
                snapshot_start_node_index = std::abs(std::stoi(argv[i + 1]));
                snapshot_end_node_index = std::abs(std::stoi(argv[i + 2]));

                i = i + 2;
                continue;
            }
        }

        // !done! adicionar o CoordinateParser
        // ParserXY(std::string string_coordinate, std::string separator)
        else if ((std::string)argv[i] == "--SnapshotXY")
        {
            snapshot = true;

            // !todo! Retirar trecho
            /*
            // X-X : start-end
            snapshot_start_node_x = std::abs(std::stoi(&argv[i + 1][0]));
            snapshot_end_node_x = std::abs(std::stoi((&argv[i + 1][2])));

            // Y-Y : start-end
            snapshot_start_node_y = std::abs(std::stoi(&argv[i + 2][0]));
            snapshot_end_node_y = std::abs(std::stoi((&argv[i + 2][2])));
            */

            std::pair<long, long> start_coordinates_pair_x = CoordinateParser(argv[i + 1], "-");
            std::pair<long, long> start_coordinates_pair_y = CoordinateParser(argv[i + 2], "-");

            // X-X : start-end
            snapshot_start_node_x = start_coordinates_pair_x.first;
            snapshot_end_node_x = start_coordinates_pair_x.second;

            // Y-Y : start-end
            snapshot_start_node_y = start_coordinates_pair_y.first;
            snapshot_end_node_y = start_coordinates_pair_y.second;

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
            continue;
        }

        else if ((std::string)argv[i] == "--WarningEnabled")
        {
            warning_enabled = true;
            continue;
        }

        else if ((std::string)argv[i] == "--Heuristic")
        {
            heuristic_weight = std::stoi(argv[i + 1]);
            i = i + 1;
            continue;
        }

        else if ((std::string)argv[i] == "--Cost")
        {
            cost_weight = std::stoi(argv[i + 1]);
            i = i + 1;
            continue;
        }

        // !todo! Retirar trecho
        /*// !todo! ineficiente - "setar" todos os valores logo de uma vez
        // os parâmetros iniciais (tamanho, início e fim) são estáticos
        else if ( !json_config_enabled && ( (i == 1) || (i == 2) || (i == 3) || (i == 4) ) && std::isdigit(*(argv[i])) )
        {
            // !todo! usar switch case - talvez nem precise
            if (i == 1)
            {
                SetGirdSizeX(std::stoi(argv[i]));
            }

            else if (i == 2)
            {
                SetGirdSizeY(std::stoi(argv[i]));
            }

            else if (i == 3)
            {
                SetStart(ParserXY(argv[i], "-").x, ParserXY(argv[i], "-").y);
            }

            else if (i == 4)
            {
                SetGoal(ParserXY(argv[i], "-").x, ParserXY(argv[i], "-").y);
            }
        }*/

        // !done! ineficiente - "setar" todos os valores logo de uma vez
        // os parâmetros iniciais (tamanho, início e fim) são estáticos
        else if ( !json_config_enabled && (i == 1) )
        {
            SetGirdSizeX(std::stoi(argv[i]));

            SetGirdSizeY(std::stoi(argv[i + 1]));

            SetStart(ParserXY(argv[i + 2], "-").x, ParserXY(argv[i + 2], "-").y);

            SetGoal(ParserXY(argv[i + 3], "-").x, ParserXY(argv[i + 3], "-").y);

            i = i + 3;
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

/*
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
        else if (warning_enabled)
        {
            // A barreira simplesmente não será inserida, não é necessário encerrar o programa
            // Um aviso será envido apenas para se saber do problema
            std::cout << "WARNING: Invalid barrier - " << configuration["Barrier coordinates"][i] << "\n";
        }
    }

    if (barrier.find(START) != barrier.end())
    {
        std::cout << "WARNING: Barrier overlaps the START node\n";
        std::exit(0);
    }

    if (barrier.find(GOAL) != barrier.end())
    {
        std::cout << "WARNING: Barrier overlaps the GOAL node\n";
        std::exit(0);
    }

    
    std::cout << "Grid size X " << grid_size_x << "\n";
    std::cout << "Grid size Y " << grid_size_y << "\n";
    std::cout << "Start [" << configuration["Start"][0] << "] [" << configuration["Start"][1] << "]\n";
    std::cout << "Goal [" << configuration["Goal"][0] << "] [" << configuration["Goal"][1] << "]\n";
    std::cout << "Barrier coordinates " << configuration["Barrier coordinates"] << "\n";
    
}

*/