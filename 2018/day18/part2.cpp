#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <array>
#include <unordered_map>
#include <optional>
#include <algorithm>

#define RUNTIME 1000000000

using namespace std;

class Landscape {
    private: 
        vector<string> landscape;
        unordered_map<size_t, pair<int, int>> cache;
        int cur_time;

    private:
        // An open acre will become filled with trees if three or more adjacent acres contained trees. 
        //      Otherwise, nothing happens.
        // An acre filled with trees will become a lumberyard if three or more adjacent acres were lumberyards. 
        //      Otherwise, nothing happens.
        // An acre containing a lumberyard will remain a lumberyard if it was adjacent to at least one other lumberyard and at least one acre containing trees. 
        //      Otherwise, it becomes open.
        bool check_adjacent(int y, int x, int count, char value) {
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
        Landscape() : cur_time(0) { }

        // build up the landscape
        void add_row(const string& s) {
            landscape.push_back(move(s));
        }

        // update the landscape one cycle
        void update() {
            // note acres that change, since everthing happens simultaneously
            map<pair<int, int>, char> changes;
            // check each acre for transformation accordint to its type
            for (int y=0; y<landscape.size(); ++y) {
                for (int x=0; x<landscape[y].size(); ++x) {
                    switch(landscape[y][x]) {
                        case '.':
                            if (check_adjacent(y, x, 3, '|')) {
                                changes[make_pair(y, x)] = '|';
                            }
                            break;
                        case '|':
                            if (check_adjacent(y, x, 3, '#')) {
                                changes[make_pair(y, x)] = '#';
                            }
                            break;
                        case '#':
                            if (!(check_adjacent(y, x, 1, '#') &&
                                check_adjacent(y, x, 1, '|'))) {
                                changes[make_pair(y, x)] = '.';
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
            ++cur_time;
        }

        // calculate number of TREE acres * number of LUMBER acres (part 1 answer)
        int get_resource_value() {
            int trees=0, lumber=0;
            for (int y=0; y<landscape.size(); ++y) {
                for (int x=0; x<landscape[y].size(); ++x) {
                    if (landscape[y][x] == '|') {
                        ++trees;
                    }
                    else if (landscape[y][x] == '#') {
                        ++lumber;
                    }
                }
            }
            return trees * lumber;
        }

        // Return the time and resource value if we've seen the current state before.
        // Otherwise, record the current state
        optional<pair<int, int>> update_cache() {
            // For the sake of simplicity, just build a string from the landscape chars and hash it
            const auto row_len = landscape[0].size();
            string cur_state;
            cur_state.resize(landscape.size() * row_len);

            auto itr = cur_state.begin();
            for(auto& row : landscape) {
                copy(row.begin(), row.end(), itr);
                advance(itr, row_len);
            }

            hash<string> hash_fn;
            auto hashed_state = hash_fn(cur_state);
            // we've seen this state before
            if (cache.count(hashed_state)) {
                return cache[hashed_state];
            } 
            // new state
            else {
                // map (hashed) state to time and resource value
                cache[hashed_state] = make_pair(cur_time, get_resource_value());
                return nullopt;
            }
        }

        // lookup in the cache the resrouce value at a given time
        int get_resource_at_time(int time) {
            auto itr = find_if(cache.begin(), cache.end(), [time](auto& record) {
                return record.second.first == time;
            });
            if (itr != cache.end()) {
                return itr->second.second;
            }
            else {
                return -1;
            }
        }

        // get the current time
        inline int get_time() {
            return cur_time;
        }

};

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

    // run the simulation until we find a cycle
    int cycle_begin, cycle_len;
    while(true) {
        auto cached = landscape.update_cache();
        if (cached.has_value()) {
            cycle_begin = cached.value().first;
            cycle_len = landscape.get_time() - cached.value().first;
            break;
        }
        landscape.update();
    }

    // given the time remaining and the length of a cycle, how far into a cycle will t=RUNTIME be
    auto offset = (RUNTIME - landscape.get_time()) % cycle_len;
    // get the resource value at that offset in the cycle
    cout << landscape.get_resource_at_time(cycle_begin + offset) << endl;

    return 0;
}


