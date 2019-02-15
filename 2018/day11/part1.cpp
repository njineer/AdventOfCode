#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

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

    // calculate fuel level for each cell
    vector<int> grid(RANGE*RANGE);
    for (int x=1; x < RANGE; x++) {
        for (int y=1; y < RANGE; y++) {
            grid[z(x,y,RANGE)] = calc_fuel(x, y, serial);
        }
    }

    int max_fuel = INT_MIN;
    pair<int, int> max_fuel_ul = make_pair(0,0);

    // naively calculate fuel for all 3x3 squares; track max
    for (int x=1; x < RANGE-2; x++) {
        for (int y=1; y < RANGE-2; y++) {
            auto xy_fuel = 
                grid[z(x, y,   RANGE)] + grid[z(x+1, y,   RANGE)] + grid[z(x+2, y, RANGE)]   +
                grid[z(x, y+1, RANGE)] + grid[z(x+1, y+1, RANGE)] + grid[z(x+2, y+1, RANGE)] +
                grid[z(x, y+2, RANGE)] + grid[z(x+1, y+2, RANGE)] + grid[z(x+2, y+2, RANGE)];
            if (xy_fuel > max_fuel) {
                max_fuel = xy_fuel;
                max_fuel_ul = make_pair(x,y);
            }
        }
    }

    cout << "max fuel 3x3 @ " << max_fuel_ul.first << ", " << max_fuel_ul.second << endl;

    return 0;
}

