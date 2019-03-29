#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <map>

using namespace std;

class Landscape {
    public:
        enum class Acre {
            OPEN,
            TREE,
            LUMBER,
            UNKNOWN
        };

    private: 
        vector<vector<Acre>> landscape;

    public:
        void add_row(const string& s) {
            vector<Acre> row;
            transform(s.begin(), s.end(), back_inserter(row), 
                      [](char c) { return translate(c); });
            landscape.push_back(move(row));
        }

        void update() {
            map<pair<int, int>, Acre> changes;
        }

    private:
        static Acre translate(char c);

};
Landscape::Acre Landscape::translate(char c) {
    Acre a;
    switch (c) {
        case '.': a = Acre::OPEN; break;
        case '|': a = Acre::TREE; break;
        case '#': a = Acre::LUMBER; break;
        default:  a = Acre::UNKNOWN; break;
    }
    return a;
}

int main (int argc, char** argv) {

    Landscape landscape;
    for (string input; getline(cin, input);) {
        // note and skip empty lines
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) {
            continue;
        }
        landscape.add_row(input);
    }

    return 0;
}

