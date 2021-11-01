// Esse arquivo contém funções de configuração
#include <array>
#include <unordered_set>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <functional>

#include"AStarHeader.h"

// !todo! Adicionar parâmetro obrigatório?
// !done! adicionar supporte para opções com uma única letra "-h"
// !todo! melhorar opções que recebem valores (Snapshot, SnapshotXY)
// - verificar a entrada de dados (números - verificar se são dígitos)
// !done! adicionar um Snapshot/SnapshotXY all: imprimir para todos os nós
// !done! warning flag?
// !todo! Melhorar os warnings?
// !todo! show JSON config info (ex.: file path)???? NÃO
// Obs: Switch só funciona bem com comparação de inteiros!

namespace ArgumentSnapshot
{
    std::string long_option = "--Snapshot";
    std::string short_option= "-s";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + " [[snapshot_start_node_index] [snapshot_end_node_index]|all]  Filter the desired nodes based on their index\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
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

namespace ArgumentSnapshotXY
{
    std::string long_option = "--SnapshotXY";
    std::string short_option= "-S";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + " [snapshot_start_node_x]-[snapshot_end_node_x] [snapshot_start_node_y]-[snapshot_end_node_y]  Filter the desired nodes based on their coordinates\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
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

namespace ArgumentDebug
{
    std::string long_option = "--Debug";
    std::string short_option= "-d";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Enable debug output, used in some tests only\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        debug = true;
        std::cout << "Debug mode enabled!\n";

        return 0;
    }
};

namespace ArgumentBestPathIndex
{
    std::string long_option = "--BestPathIndex";
    std::string short_option= "-p";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Every node in the best path will display its index\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        best_path_index = true;

        return 0;
    }
};

namespace ArgumentShowPriorityQueue
{
    std::string long_option = "--ShowPriorityQueue";
    std::string short_option= "-q";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Show the Priority Queue for each selected node of the Snapshot\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        show_priority_queue = true;

        return 0;
    }
};

namespace ArgumentShowVisitedNeighbors
{
    std::string long_option = "--ShowVisitedNeighbors";
    std::string short_option= "-n";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Show all the visited neighbors for each selected node of the Snapshot\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        show_visited_neighbors = true;

        return 0;
    }
};

namespace ArgumentInteractive
{
    std::string long_option = "--Interactive";
    std::string short_option= "-i";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Allows to stop execution for each selected node of the Snapshot\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        interactive = true;

        return 0;
    }
};

namespace ArgumentShowMap
{
    std::string long_option = "--ShowMap";
    std::string short_option= "-m";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Displays the map for each selected node of the Snapshot\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        show_map = true;

        return 0;
    }
};

namespace ArgumentShowBarrier
{
    std::string long_option = "--ShowBarrier";
    std::string short_option= "-b";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Displays the barrier for each selected node of the Snapshot\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        show_barrier = true;

        return 0;
    }
};

namespace ArgumentPadding
{
    std::string long_option = "--Padding";
    std::string short_option= "-P";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + " [padding_cell_size]  Controls map padding, default value: "+ std::to_string(padding_cell_size) + "\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        padding_cell_size = std::stoi(argv[current_arg + 1]);

        return 1;
    }
};

namespace ArgumentWarningEnabled
{
    std::string long_option = "--WarningEnabled";
    std::string short_option= "-w";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  Enables serveral warnings of non critical problems\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        warning_enabled = true;

        return 0;
    }
};

namespace ArgumentHeuristic
{
    std::string long_option = "--Heuristic";
    std::string short_option= "-H";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + " [heuristic_weight]  Controls heuristic value (heuristic_weight * (dx + dy)), default value: " + std::to_string(heuristic_weight) + "\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        heuristic_weight = std::stoi(argv[current_arg + 1]);

        return 1;
    }
};

namespace ArgumentCost
{
    std::string long_option = "--Cost";
    std::string short_option= "-c";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + " [cost_weight]  Controls cost value (cost_weight * (dx + dy)), default value: " + std::to_string(cost_weight) + "\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        cost_weight = std::stoi(argv[current_arg + 1]);

        return 1;
    }
};

namespace ArgumentJsonConfig
{
    std::string long_option = "--JsonConfig";
    std::string short_option= "-J";

    void help()
    {
        std::string help = "\t" + long_option + "  " + short_option + "  [json_config_file_path]  Path to a Json configuration file\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        json_config_enabled = true;
        json_config_file_path = (std::string)argv[current_arg + 1];
        JsonConfig();

        return 1;
    }
};

namespace ArgumentDefaultArguments
{
    std::string long_option = "";
    std::string short_option= "";

    void help()
    {
        std::string help = "\n  ./SimpleAStarExecutable [gid_size_x] [gid_size_y] [START_X-START_Y] [GOAL_X-GOAL_Y]\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        SetGirdSizeX(std::stoi(argv[current_arg]));

        SetGirdSizeY(std::stoi(argv[current_arg + 1]));

        SetStart(ParserXY(argv[current_arg + 2], "-").x, ParserXY(argv[current_arg + 2], "-").y);

        SetGoal(ParserXY(argv[current_arg + 3], "-").x, ParserXY(argv[current_arg + 3], "-").y);

        return 3;
    }
};


namespace ArgumentTest
{
    std::string long_option = "--test";
    std::string short_option = "-t";

    void help()
    {
        std::string help =  "\t" + long_option + "  " + short_option + "  Execute a test\n\n";
        std::cout << help;
    }

    long run(int current_arg, char* argv[])
    {
        for(long i = 0; i < sliding_puzzle_start.size(); ++i)
        {
            std::cout << sliding_puzzle_start[i] << "\n";
        }

        std::exit(0);
        return 0;
    }
}


namespace ArgumentHelp
{
    std::string long_option = "--help";
    std::string short_option = "-h";

    void help()
    {
        std::string help =  "\t" + long_option + "  " + short_option + "  Display help text\n\n";
        std::cout << help;
    };

    long run(int current_arg, char* argv[])
    {
        ArgumentDefaultArguments::help();
        std::cout << "\tOptions:\n\n";
        ArgumentHelp::help();
        ArgumentSnapshot::help();
        ArgumentSnapshotXY::help();
        ArgumentDebug::help();
        ArgumentBestPathIndex::help();
        ArgumentShowPriorityQueue::help();
        ArgumentShowVisitedNeighbors::help();
        ArgumentInteractive::help();
        ArgumentShowMap::help();
        ArgumentShowBarrier::help();
        ArgumentPadding::help();
        ArgumentWarningEnabled::help();
        ArgumentHeuristic::help();
        ArgumentCost::help();
        ArgumentJsonConfig::help();
        ArgumentTest::help();

        std::exit(0); // !todo! Perigoso?

        return 0;
    }
};

long ExecuteArg(std::string my_argument, int current_arg, char* argv[])
{
    std::unordered_map<std::string, std::function<long(int current_arg, char* argv[])>> arguments_mapping;

    // Mapping
    // Help
    arguments_mapping[ArgumentHelp::long_option] = ArgumentHelp::run;
    arguments_mapping[ArgumentHelp::short_option] = ArgumentHelp::run;

    // DefaultArguments
    arguments_mapping[ArgumentDefaultArguments::long_option] = ArgumentDefaultArguments::run;

    // Snapshot
    arguments_mapping[ArgumentSnapshot::long_option] = ArgumentSnapshot::run;
    arguments_mapping[ArgumentSnapshot::short_option] = ArgumentSnapshot::run;

    // SnapshotXY
    arguments_mapping[ArgumentSnapshotXY::long_option] = ArgumentSnapshotXY::run;
    arguments_mapping[ArgumentSnapshotXY::short_option] = ArgumentSnapshotXY::run;

    // Debug
    arguments_mapping[ArgumentDebug::long_option] = ArgumentDebug::run;
    arguments_mapping[ArgumentDebug::short_option] = ArgumentDebug::run;

    // BestPathIndex
    arguments_mapping[ArgumentBestPathIndex::long_option] = ArgumentBestPathIndex::run;
    arguments_mapping[ArgumentBestPathIndex::short_option] = ArgumentBestPathIndex::run;

    // ShowPriorityQueue
    arguments_mapping[ArgumentShowPriorityQueue::long_option] = ArgumentShowPriorityQueue::run;
    arguments_mapping[ArgumentShowPriorityQueue::short_option] = ArgumentShowPriorityQueue::run;

    // ShowVisitedNeighbors
    arguments_mapping[ArgumentShowVisitedNeighbors::long_option] = ArgumentShowVisitedNeighbors::run;
    arguments_mapping[ArgumentShowVisitedNeighbors::short_option] = ArgumentShowVisitedNeighbors::run;

    // Interactive
    arguments_mapping[ArgumentInteractive::long_option] = ArgumentInteractive::run;
    arguments_mapping[ArgumentInteractive::short_option] = ArgumentInteractive::run;

    // ShowMap
    arguments_mapping[ArgumentShowMap::long_option] = ArgumentShowMap::run;
    arguments_mapping[ArgumentShowMap::short_option] = ArgumentShowMap::run;

    // ShowBarrier
    arguments_mapping[ArgumentShowBarrier::long_option] = ArgumentShowBarrier::run;
    arguments_mapping[ArgumentShowBarrier::short_option] = ArgumentShowBarrier::run;

    // Padding
    arguments_mapping[ArgumentPadding::long_option] = ArgumentPadding::run;
    arguments_mapping[ArgumentPadding::short_option] = ArgumentPadding::run;

    // WarningEnabled
    arguments_mapping[ArgumentWarningEnabled::long_option] = ArgumentWarningEnabled::run;
    arguments_mapping[ArgumentWarningEnabled::short_option] = ArgumentWarningEnabled::run;

    // Heuristic
    arguments_mapping[ArgumentHeuristic::long_option] = ArgumentHeuristic::run;
    arguments_mapping[ArgumentHeuristic::short_option] = ArgumentHeuristic::run;

    // Cost
    arguments_mapping[ArgumentCost::long_option] = ArgumentCost::run;
    arguments_mapping[ArgumentCost::short_option] = ArgumentCost::run;

    // JsonConfig
    arguments_mapping[ArgumentJsonConfig::long_option] = ArgumentJsonConfig::run;
    arguments_mapping[ArgumentJsonConfig::short_option] = ArgumentJsonConfig::run;

    //Test
    arguments_mapping[ArgumentTest::long_option] = ArgumentTest::run;
    arguments_mapping[ArgumentTest::short_option] = ArgumentTest::run;

    if (arguments_mapping.count(my_argument) != 0)
    {
        return arguments_mapping[my_argument](current_arg, argv);
    }
    else
    {
        std::cout << "Invalid option : " << my_argument << "\n";

        std::exit(1);
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

        else
        {
            std::cout << "Invalid option : " << argument << "\n";

            std::exit(1);
        }
    }

}