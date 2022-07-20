#include <iostream>
#include <fstream>
#include <map>

using std::cout;
using std::endl;
using std::string;
using point = std::pair<int,int>;
using gridType = std::map<point, int>;
void printMap(const gridType& grid);
inline point move(const point& pt, int direction);
point simulateA(const point& pt, gridType& grid);
enum direction {north = 0, east, south, west}; // clock-based modulus arithmetics -> mod 4
enum states {clean, weakened, infected, flagged};
const int Nsteps = 10000000;
int INFECTIONS = 0;

point simulateB(const point& pt, gridType& grid) {
    static int direction = north;
    int status = grid.at(pt);
    int newStatus;
    switch (status) {
        case infected:   
            direction = (direction + 1) % 4; // turn right
            newStatus = flagged;
            break;
        case clean:
            direction = (direction + 3) % 4; // turn left
            newStatus = weakened;
            break;
        case flagged:
            direction = (direction + 2) % 4; // turn around
            newStatus = clean;
            break;
        case weakened:
            newStatus = infected;
            INFECTIONS++;
            break;
        default:
            throw std::runtime_error("unexpected status'\n");
    }
    grid[pt] = newStatus;

    point newPoint = move(pt, direction);
    if (grid.find(newPoint) == grid.end())    // if pt not in grid ...
        grid[newPoint] = clean;

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
            const int nodeStatus = (symbol == '#' ? infected : clean);
            grid[std::make_pair(x,y)] = nodeStatus;
            ++x;
        }
        --y;
    }

    point position = std::make_pair(0,0);
    for (int i = 0; i < Nsteps; ++i)
        position = simulateB(position, grid); // case A: simulateA(...) / Nsteps=1e5

    printMap(grid);
    std::cout << "\n\nposition = (" << position.first << ", " << position.second << ")\n";
    std::cout << endl << "total infections = " << INFECTIONS << std::endl;
    return 0;
}

point simulateA(const point& pt, gridType& grid) {
    static int direction = north;
    int status = grid.at(pt);
    if (status == infected)
        direction = (direction + 1) % 4; // turn right
    else {
        direction = (direction + 3) % 4; // turn left
        INFECTIONS++;
    }
    grid[pt] = (status == infected ? clean : infected);

    point newPoint = move(pt, direction);
    if (grid.find(newPoint) == grid.end())    // if pt not in grid ...
        grid[newPoint] = clean;

    return newPoint;
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
                int status = grid.at(pt);
                char symbol;
                switch (status) {
                    case clean:    symbol = '.'; break;
                    case weakened: symbol = 'w'; break;
                    case infected: symbol = '#'; break;
                    case flagged:  symbol = 'f'; break;
                    default: throw std::runtime_error("unexpected status'\n");
                }
                std::cout << symbol;
            }
            else
                std::cout << 'x';
        }
        std::cout << '\n';
    }
    std::cout << endl;
}