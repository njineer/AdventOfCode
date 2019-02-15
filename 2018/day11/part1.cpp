#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define RANGE 301

int calc_fuel(int x, int y, int serial) {
    int rack_id = x + 10;
    int power = ((rack_id * y) + serial) * rack_id;
    power /= 100;
    if (power > 0) {
        power %= 10;
    }     
    return power - 5;
}

inline int z(int x, int y, int w) {
    return x*w + y;
}

int main (int argc, char** argv) {

    string input; 
    getline(cin, input);
    int serial = stoi(input);

    vector<int> grid(RANGE*RANGE);
    for (int x=1; x < RANGE; x++) {
        for (int y=1; y < RANGE; y++) {
            grid[z(x,y,RANGE)] = calc_fuel(x, y, serial);
        }
    }

    map<pair<int, int>, int> grid_3x3;
    for (int x=1; x < RANGE-2; x++) {
        for (int y=1; y < RANGE-2; y++) {
            grid_3x3[make_pair(x,y)] = 
                grid[z(x, y,   RANGE)] + grid[z(x+1, y,   RANGE)]   + grid[z(x+2, y, RANGE)] +
                grid[z(x, y+1, RANGE)] + grid[z(x+1, y+1, RANGE)] + grid[z(x+2, y+1, RANGE)] +
                grid[z(x, y+2, RANGE)] + grid[z(x+1, y+2, RANGE)] + grid[z(x+2, y+2, RANGE)];
        }
    }

    auto xy = max_element(grid_3x3.begin(), grid_3x3.end(), [](const auto& c1, const auto& c2){
        return c1.second < c2.second; })->first;
            
    cout << "max fuel 3x3 @ " << xy.first << ", " << xy.second << endl;

    return 0;
}
