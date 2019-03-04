#include <iostream>
#include <cctype>
#include <regex>
#include <algorithm>
#include <cassert>
#include <map>
#include <set>
#include <iterator>
#include <numeric>

using namespace std;

#define GENERATIONS 20

class Pots {
    private:
        set<int> plants;
        set<unsigned int> rules;

        inline bool get_plant(int i) {
            auto itr = plants.find(i);
            return itr != plants.end();
        }

        inline bool plant_next_gen(unsigned int pattern) {
            return rules.find(pattern) != rules.end();
        }

        unsigned int get_pattern(int center) {
            return get_plant(center-2) << 4 | 
                   get_plant(center-1) << 3 | 
                   get_plant(center)   << 2 | 
                   get_plant(center+1) << 1 |
                   get_plant(center+2);
        }

    public: 
        Pots() { }

        void init(const string& s) {
            for (auto i=0; i < s.size(); i++) {
                if (s[i] == '#') {
                    plants.insert(i);
                }
            }
        }

        void add_rule(const string& pattern_str) {
            unsigned int pattern = 0;
            int i= pattern_str.size() - 1;
            for(auto itr = pattern_str.begin(); itr != pattern_str.end(); itr++, i--) {
                if (*itr == '#') {
                    pattern |= 1 << i;
                }
            }
            rules.insert(pattern);
        }

        void update() {
            auto leftmost_plant = *plants.begin();
            auto cur_pot = get_pattern(leftmost_plant - 3);
            //cout << "starting @ " << leftmost_plant << endl;
            for(int i=leftmost_plant-2; i < *plants.rbegin()+2; i++) {
                cur_pot = (cur_pot << 1 | get_plant(i+2)) & 0x1f;
                if (plant_next_gen(cur_pot)) {
                    plants.insert(i);
                }
                else {
                    plants.erase(i);
                }
            }
        }

        int plant_sum() {
            return accumulate(plants.begin(), plants.end(), 0);
        }

        void display() {
            for(int i=-3; i <= 35; i++) {
                if (plants.count(i)) {
                    cout << "#";
                }
                else {
                    cout << ".";
                }
            }
            cout << endl;
        }
};


int main (int argc, char** argv) {

    Pots pots;

    const regex init_re("initial state:\\s+([#\\.]+)");
    const regex growth_re("([#\\.]+)\\s+=>\\s+([#\\.])");
    smatch match;

    // set initial state of pots
    string input;
    getline(cin, input);
    if (regex_match(input, match, init_re)) {
        pots.init(match.str(1));
    } 
    else {
        cout << "expected initial state in first line, got '" << input << "'" << endl;
        throw;
    }

    // parse generational patterns
    for (string input; getline(cin, input);) {
        // skip blank lines
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            continue;

        if (regex_match(input, match, growth_re)) {
            if (match.str(2) == "#") {
                pots.add_rule(match.str(1));
            }
        } 
        else {
            cout << "unexpected pattern: '" << input << "'" << endl;
            throw;
        }
    }

    //cout << 0 << "\t";
    // pots.display();
    for (int i=1; i <= GENERATIONS; i++) {
        // cout << i << "\t";
        pots.update();         
        // pots.display();
    }

    cout << pots.plant_sum() << endl;

    return 0;
}
