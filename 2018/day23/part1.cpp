#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <algorithm>

using namespace std;

struct Nanobot {
    const int x, y, z, r;

    Nanobot(int _x, int _y, int _z, int _r) : x(_x), y(_y), z(_z), r(_r) { }

    string str() {
        ostringstream ss;
        ss << "Nanobot(x=" << x << ", y=" << y << ", z=" << z << ", r=" << r << ")";
        return ss.str();
    }
};

int main (int argc, char** argv) {

    const regex re("pos=<(\\d+),(\\d+),(\\d+)>,\\s+r=(\\d+)");

    vector<Nanobot> nanobots;
    for (string input; getline(cin, input); ) {
        smatch match;
        if (regex_match(input, match, re)) {
            nanobots.emplace_back(stoi(match[1]), stoi(match[2]), stoi(match[3]), stoi(match[4]));
        }
        // skip empty lines
        else if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) {
            continue;
        }
        else {
            cerr << "Skipping bad input: " << input << endl;
        }
    }

    for (auto& nb : nanobots) {
        cout << nb.str() << endl;
    }

    return 0;
}

