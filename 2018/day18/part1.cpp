#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <array>

#define RUNTIME 10

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
        static Acre translate(char c);

        vector<vector<Acre>> landscape;

    private:
        // An open acre will become filled with trees if three or more adjacent acres contained trees. 
        //      Otherwise, nothing happens.
        // An acre filled with trees will become a lumberyard if three or more adjacent acres were lumberyards. 
        //      Otherwise, nothing happens.
        // An acre containing a lumberyard will remain a lumberyard if it was adjacent to at least one other lumberyard and at least one acre containing trees. 
        //      Otherwise, it becomes open.
        bool check_adjacent(int y, int x, int count, Acre value) {
            int adj_eq_count = 0;
            array<pair<int,int>, 8> adj { 
                make_pair(y-1, x-1), make_pair(y-1, x), make_pair(y-1, x+1),
                make_pair(y, x-1), make_pair(y, x+1), 
                make_pair(y+1, x-1), make_pair(y+1, x), make_pair(y+1, x+1)
            };
            for (auto [ay,ax] : adj) {
                if (ay >= 0 && ay < landscape.size() &&
                    ax >= 0 && ax < landscape[ay].size() &&
                    landscape[ay][ax] == value) 
                {
                    if (++adj_eq_count >= count) {
                        return true;
                    }
                }
            }
            return false;
        }

    public:

        // build up the landscape
        void add_row(const string& s) {
            vector<Acre> row;
            transform(s.begin(), s.end(), back_inserter(row), 
                      [](char c) { return translate(c); });
            landscape.push_back(move(row));
        }

        // update the landscape one cycle
        void update() {
            // note acres that change, since everthing happens simultaneously
            map<pair<int, int>, Acre> changes;
            // check each acre for transformation accordint to its type
            for (int y=0; y<landscape.size(); ++y) {
                for (int x=0; x<landscape[y].size(); ++x) {
                    switch(landscape[y][x]) {
                        case Acre::OPEN:
                            if (check_adjacent(y, x, 3, Acre::TREE)) {
                                changes[make_pair(y, x)] = Acre::TREE;
                            }
                            break;
                        case Acre::TREE:
                            if (check_adjacent(y, x, 3, Acre::LUMBER)) {
                                changes[make_pair(y, x)] = Acre::LUMBER;
                            }
                            break;
                        case Acre::LUMBER:
                            if (!(check_adjacent(y, x, 1, Acre::LUMBER) &&
                                check_adjacent(y, x, 1, Acre::TREE))) {
                                changes[make_pair(y, x)] = Acre::OPEN;
                            }
                            break;
                        default: break;
                    }
                }
            }
            // apply the changes
            for_each(changes.begin(), changes.end(), [this](auto& change) {
                auto [y,x] = change.first;
                this->landscape[y][x] = change.second;
            });
        }

        // calculate number of TREE acres * number of LUMBER acres (part 1 answer)
        int get_resource_value() {
            int trees=0, lumber=0;
            for (int y=0; y<landscape.size(); ++y) {
                for (int x=0; x<landscape[y].size(); ++x) {
                    if (landscape[y][x] == Acre::TREE) {
                        ++trees;
                    }
                    else if (landscape[y][x] == Acre::LUMBER) {
                        ++lumber;
                    }
                }
            }
            return trees * lumber;
        }

};
// translate input chars to Acre types
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

    // import the landscape
    Landscape landscape;
    for (string input; getline(cin, input);) {
        // note and skip empty lines
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) {
            continue;
        }
        landscape.add_row(input);
    }

    // run the simulation
    for (int i=0; i < RUNTIME; i++) {
        landscape.update();
    }

    // calculate final answer
    cout << landscape.get_resource_value() << endl;

    return 0;
}




