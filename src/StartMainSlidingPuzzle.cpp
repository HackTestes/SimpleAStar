#include "AStarHeader.h"
#include "MainSlidingPuzzle.h"

int StartSlidingPuzzleImplementations()
{
    switch(sliding_puzzle_goal.size())
    {
        //Sliding Puzzle 3x3
        case 9:
            return mainSP< std::array<uint8_t, 9>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);


        //Sliding Puzzle 4x4
        case 16:
            return mainSP< std::array<uint8_t, 16>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);


        //Sliding Puzzle 5x5
        case 25:
            return mainSP< std::array<uint8_t, 25>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);


        //Sliding Puzzle 6x6
        case 36:
            return mainSP< std::array<uint8_t, 36>, uint8_t >(sliding_puzzle_start, sliding_puzzle_goal);

        default:
            return -1;
    }
}