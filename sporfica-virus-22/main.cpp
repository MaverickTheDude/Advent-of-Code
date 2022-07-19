#include <iostream>
#include <fstream>
#include <map>

using std::cout;
using std::endl;
using std::string;
using point = std::pair<int,int>;
using gridType = std::map<point, bool>;
void printMap(const gridType& grid);
inline point move(const point& pt, int direction);
enum direction {north, east, south, west}; // clock-based modulus arithmetics -> mod 4
const int Nsteps = 10000;
int INFECTIONS = 0;

point simulate(const point& pt, gridType& grid) {
    static int direction = north;
    bool isInfected = grid.at(pt);
    if (isInfected)
        direction = (direction + 1) % 4; // turn right
    else {
        direction = (direction + 3) % 4; // turn left
        INFECTIONS++;
    }
    grid[pt] = !isInfected;

    point newPoint = move(pt, direction);
    if (grid.find(newPoint) == grid.end())    // if pt not in grid ...
        grid[newPoint] = false;

    return newPoint;
}



int main() {
    string line;
    std::ifstream myfile("../input.txt");
    if (!myfile)
        std::cerr << "Error opening 'test.txt'\n";

    gridType grid;
    int y = 12, x;
    while( getline(myfile, line) ) {
        x = -12;
        for (char symbol : line) {
            const bool isInfected = (symbol == '#' ? true : false);
            grid[std::make_pair(x,y)] = isInfected;
            ++x;
        }
        --y;
    }

    point position = std::make_pair(0,0);
    for (int i = 0; i < Nsteps; ++i)
        position = simulate(position, grid);

    printMap(grid);
    std::cout << "\n\nposition = (" << position.first << ", " << position.second << ")\n";
    std::cout << endl << "total infections = " << INFECTIONS << std::endl;
    return 0;
}

inline point move(const point& pt, int direction) {
    int x = pt.first;
    int y = pt.second;
    switch (direction) {
        case north: ++y; break;
        case south: --y; break;
        case east:  ++x; break;
        case west:  --x; break;
    }
    return std::make_pair(x,y);
}

void printMap(const gridType& grid) {
    for (int y = 12; y > -13; --y) {
        for (int x = -12; x < 13; ++x) {
            point pt = std::make_pair(x,y);
            if (grid.find(pt) != grid.end()) { // if pt in grid ...
                bool isInfected = grid.at(pt);
                char symbol = (isInfected ? '#' : '.');
                std::cout << symbol;
            }
            else
                std::cout << 'x';
        }
        std::cout << '\n';
    }
    std::cout << endl;
}