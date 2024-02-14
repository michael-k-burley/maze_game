
#ifndef MAZE_MAKER_HPP
#define MAZE_MAKER_HPP

#include <iostream>
#include <vector>
#include <array> // for std::array<type, count> 

#include <utility> //pair
#include <stack>
#include <random>
#include <numeric> //accumulate


class MazeMaker {

    public:
        static std::vector<std::vector<int>> makeMaze(int height, int width);

    private:
        enum class CellState { UNVISITED = -1, WALKWAY, WALL };

        //Function to generate maze from make maze
        static std::vector<std::vector<CellState>> generateMaze(int height, int width, int xs, int ys);

        //Function to find offset to move to next available cell from current cell. 
        //(Note returns (0,0) if no unvisited cell reachable from current cell)
        static std::pair<int,int> findOffsetToNextCell(const std::pair<int,int>& curr, const std::vector<std::vector<CellState>>& map); 
};

#endif
