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

int manhattan(const Nanobot& nb1, const Nanobot& nb2) {
    return abs(nb1.x - nb2.x) + abs(nb1.y - nb2.y) + abs(nb1.z - nb2.z);
}


int main (int argc, char** argv) {

    const regex re("pos=<([+-]?\\d+),([+-]?\\d+),([+-]?\\d+)>,\\s+r=([+-]?\\d+)");
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

    auto& strongest = *max_element(nanobots.begin(), nanobots.end(), 
        [](const auto& nb1, const auto& nb2) { return nb1.r < nb2.r; });

    auto in_range = count_if(nanobots.begin(), nanobots.end(), [&strongest](const auto& nb) {
        return manhattan(nb, strongest) <= strongest.r; });

    cout << in_range << endl;
    return 0;
}

