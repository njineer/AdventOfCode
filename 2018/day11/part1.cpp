#include <iostream>
#include <string>
#include <vector>

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

    return 0;
}
