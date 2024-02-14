
#include "../headers/MazeMaker.hpp"

std::pair<int,int> MazeMaker::findOffsetToNextCell(const std::pair<int,int>& curr, const std::vector<std::vector<CellState>>& map) 
{
    std::pair<int,int> offset{0,0}; //Amount to move in (y direction, x direction) -1 for down, left, +1 for up, right

    //Make random number generator (Note: Static so successive function calls don't create each time)
    static std::random_device rd;   // Only used once to initialise (seed) engine
    static std::mt19937 rng(rd());  // Random-number engine used (Mersenne-Twister in this case)
    static std::uniform_int_distribution<int> dist(0, 3); // Range [0,3] : since top=0, right=1, bottom=2, left=3

    int height = map.size();
    int width = map[0].size();

    //Array to keep track of available directions of travel (Note: Zero means cell cannot be reached from current position)
    std::array<int, 4> dir{0,0,0,0}; // (4)<<Magik Const Alert // 4 since up, right, down, left
 
    //If neighbouring cells can be reached from current position then set to 1
    //Need to check map bounds and if cells is unvisited            
    if(curr.first - 2 >= 0 && map[curr.first - 2][curr.second] == CellState::UNVISITED)       dir[0] = 1; //Up
    if(curr.second + 2 < width && map[curr.first][curr.second + 2] == CellState::UNVISITED)   dir[1] = 1; //Right
    if(curr.first + 2 < height && map[curr.first + 2][curr.second] == CellState::UNVISITED)   dir[2] = 1; //Down
    if(curr.second - 2 >= 0 && map[curr.first][curr.second - 2] == CellState::UNVISITED)      dir[3] = 1; //Left

    //Early return if no candidate cells possible to move to from current cell
    if(std::accumulate(dir.begin(), dir.end(), 0) == 0)  return offset;

    //Pass number engine to distribution to yield random integer in range [0,3]
    int randInt = dist(rng); 

    //If randomly choosen direction is not a viable option then loop 
    while(!dir[randInt])  randInt = dist(rng);

    //Shift offset in y direction (first) OR x direction (second)
    if(randInt == 0)       offset.first = -1;   //Up
    else if(randInt == 1)  offset.second = 1;   //Right
    else if(randInt == 2)  offset.first = 1;    //Down
    else if(randInt == 3)  offset.second = -1;  //Left

    return offset;
}

// Maze Generation Algorithm // Depth first search, wilson's algo (loop-erased random walk), prim or kruskal
std::vector<std::vector<MazeMaker::CellState>> MazeMaker::generateMaze(int height, int width, int xs, int ys) 
{
    std::vector<std::vector<CellState>> bitmap(height, std::vector(width, CellState::WALKWAY)); //Create map of all zeros w correct dimensions

    for(int i{}; i < height; i++)   //Create map of cells with -1s and with wall of 1 seperating each cell
        for(int j{}; j < width; j++)
            if(i%2 || j%2) bitmap[i][j] = CellState::WALL; 
            else bitmap[i][j] = CellState::UNVISITED;      

    //Stack will keep track of current "path" of nodes, when stack is empty of cells of map have been visited
    std::stack< std::pair<int,int>, std::vector<std::pair<int,int>> > stk{};
    //Use c++ stack adapter class to convert vector to stack (Note: Is List better choice for LIFO???)

    // Starting cell ie. cell currently occupied
    std::pair<int,int> curr{ys, xs}; 

    //Once starting node has been visited, it is now accessible ie. is now a walkway
    bitmap[curr.first][curr.second] = CellState::WALKWAY;

    stk.push(curr); //Push start position onto stack

    //While stack is not empty, maze space left to be explored
    while(true){  //!stk.empty()

        //Offset to shift between current cell and next cell
        std::pair<int,int> offsets = findOffsetToNextCell(curr, bitmap);

        //If no candidates then pop off stack till cell with candidates
        //Note: offsets is only (0,0), if no unvisited cell reachable from current cell
        while(offsets == std::make_pair(0,0) && !stk.empty()){     
        
            //Pop the current node, since if node had any candidates then would skip loop
            stk.pop();

            //If stack is empty then terminate 
            if(stk.empty())  return bitmap; 

            //Shift current cell to previous cell
            curr = stk.top();

            //Update candidate array with candidate cells from new previous node
            offsets = findOffsetToNextCell(curr, bitmap);
        }

        //Turn "wall" from map into "walkway", ie. connects the current cell to the next cell
        bitmap[curr.first + offsets.first][curr.second + offsets.second] = CellState::WALKWAY; 

        //Update current to next candidate node selected 
        //(Note: Offset multiplied by 2 since "wall" of 1s seperate each cell)
        curr = std::make_pair(curr.first + (offsets.first *2), curr.second + (offsets.second *2));

        //Push next node selected onto the stack
        stk.push(curr);

        //Once a node has been visited, it is now accessible ie. is now a walkway
        bitmap[curr.first][curr.second] = CellState::WALKWAY;
    }
}

std::vector<std::vector<int>> MazeMaker::makeMaze(int height, int width)
{                  
    int adjustedWidth = width +1;   //Try just width no +1
    int adjustedHeight = height +1; //+1 since top and right "dead"????
                                                                    
    std::vector<std::vector<CellState>> maze(adjustedHeight, std::vector(adjustedWidth, CellState::WALKWAY));  //make this 1???!!!!!!!!!!!!!

    int innerWidth = width - 2;   //Minus 2 for each side (left & right) of outer wall
    int innerHeight = height - 2; //Minus 2 for each side (top & bottom) of outer wall

    int startingX = 0;          //Far left side of screen is starting x value
    int startingY = height / 2; //Divide by 2 to get starting y value (centerish of screen)

    std::vector<std::vector<CellState>> innerMaze = generateMaze(innerHeight, innerWidth, startingX, startingY);

    //Create outer walls of maze
    for(int j{}; j < width; j++) 
    {
        maze[1][j] = CellState::WALL;      //Top wall
        maze[height][j] = CellState::WALL; //Bottom wall
    }
    for(int i{1}; i < height; i++) //Start at 1 to keep top row all zeros
    {
        maze[i][0] = CellState::WALL;       //Left wall
        maze[i][width-1] = CellState::WALL; //Right wall
    }

    //Copy maze to maze padded to screen dimensions
    for(int i{}; i < innerHeight; i++){
        for(int j{}; j < innerWidth; j++)  
            maze[i+2][j+1] = innerMaze[i][j]; //+2 rows, +1 col, to shift maze inside walls created above
    }

    //Make sure spot in front of starting position is not wall (+1 Since border wall around maze)
    maze[startingY +1][startingX] = CellState::WALKWAY;      
    maze[startingY +1][startingX +1] = CellState::WALKWAY;   

    //Make sure spot in front of exit position is not wall (-1 Since border wall around maze)
    maze[startingY +1][width -1] = CellState::WALKWAY;   
    maze[startingY +1][width -2] = CellState::WALKWAY;


    //(TESTING) Convert to bitmap // -1 - unvisited, 0 - walkway, 1 - wall
    std::vector<std::vector<int>> mazey(maze.size(), std::vector(maze[0].size(), 0)); 

    for(size_t i{}; i < maze.size(); i++)
        for(size_t j{}; j < maze[0].size(); j++)
            mazey[i][j] = static_cast<int>(maze[i][j]);

    return mazey;
}
