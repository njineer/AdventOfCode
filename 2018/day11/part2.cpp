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

inline int z(int x, int y, int w=RANGE) {
    return x*w + y;
}

int main (int argc, char** argv) {
    string input; 
    getline(cin, input);
    int serial = stoi(input);

    // calculate fuel level for each cell
    vector<int> sum_grid(RANGE*RANGE, 0);
    for (int x=1; x < RANGE; x++) {
        for (int y=1; y < RANGE; y++) {
            sum_grid[z(x,y)] = calc_fuel(x, y, serial) 
                               + sum_grid[z(x-1, y)] 
                               + sum_grid[z(x, y-1)] 
                               - sum_grid[z(x-1, y-1)];
        }
    }

    int max_fuel = INT_MIN;
    int mx=0, my=0, ms=0;

    // incremental sums shamelessly borrowd from:
    //     https://www.reddit.com/r/adventofcode/comments/a53r6i/2018_day_11_solutions/ebjogd7/
    for (int s=1; s < RANGE; s++) {
        for (int x=s; x < RANGE; x++) {
            for (int y=s; y < RANGE; y++) {
                auto xy_fuel = sum_grid[z(x,y)] 
                    - sum_grid[z(x-s, y)] 
                    - sum_grid[z(x, y-s)] 
                    + sum_grid[z(x-s, y-s)];
                if (xy_fuel > max_fuel) {
                    max_fuel = xy_fuel;
                    mx = x;
                    my = y;
                    ms = s;
                }
            }
        }
    }


    // translate bottom-right to top-left
    cout << "max fuel @ " << (mx - ms + 1) << ", " << (my - ms + 1) << ", " << ms << endl;

    return 0;
}

