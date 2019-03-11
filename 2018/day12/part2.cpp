#include <iostream>
#include <cctype>
#include <regex>
#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <tuple>
#include <optional>
#include <vector>

// plant pattern should converge by this point
#define GENERATIONS 200

// Difference in plant sums between generations after convergence
//      Need to print a (few?) generation(s) after a duplicate to confirm
//      Input-dependent
#define CONVERGED_DIFF 22

#define TARGET_GENERATION 50000000000

using namespace std;

class Pots {
    private:
        using tuple2i = tuple<int, int>;

        // indices of pots that currently contain plants
        unordered_set<int> plants;
        // patterns that result in a plant next generation
        unordered_set<unsigned int> rules;

        // a cache of the pot layouts we've seen
        unordered_map<size_t, tuple2i> history;

        // is there a plant in this pot?
        inline bool get_plant(int i) {
            return plants.count(i) != 0;
        }

        // will there be a plant in this pot next generation?
        inline bool plant_next_gen(unsigned int pattern) {
            return rules.count(pattern) != 0;
        }

        // get the plant pattern/rule centered on a pot
        //      i.e. LLCRR => [#.]
        inline unsigned int get_pattern(int center) {
            return get_plant(center-2) << 4 | 
                   get_plant(center-1) << 3 | 
                   get_plant(center)   << 2 | 
                   get_plant(center+1) << 1 |
                   get_plant(center+2);
        }

        // hash the current pots layout in lieu of storing the whole vector
        //      std::hash<vector<bool>> is predefined
        //      could use a std::string, but that seems less efficient
        size_t hash(int leftmost_plant, int rightmost_plant) {
            vector<bool> layout(rightmost_plant-leftmost_plant);
            for(int i=leftmost_plant; i < rightmost_plant; i++) {
                layout[i-leftmost_plant] = get_plant(i); 
            }
            return std::hash<vector<bool>>()(layout);
        }

    public: 
        Pots() { }

        // establish the initial pots/plants layout
        void init(const string& s) {
            for (auto i=0; i < s.size(); i++) {
                if (s[i] == '#') {
                    plants.insert(i);
                }
            }
        }

        // add a generational rule
        //      i.e. LLCRR => [#.]
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

        // update the pots/plants layout based on current state and generational rules
        size_t update() {
            // get the outermost plants
            auto leftmost_plant = *std::min_element(plants.begin(), plants.end());
            auto rightmost_plant = *std::max_element(plants.begin(), plants.end());


            // current state to return
            auto cur_layout = hash(leftmost_plant, rightmost_plant);

            // starting at the left end, create the 5-pot pattern to check against
            //     generational rules 
            // shift in/out the next pot to the right until we hit the right end
            auto cur_pot = get_pattern(leftmost_plant - 3);
            for(int i=leftmost_plant-2; i < rightmost_plant+2; i++) {
                cur_pot = (cur_pot << 1 | get_plant(i+2)) & 0x1f;
                if (plant_next_gen(cur_pot)) {
                    plants.insert(i);
                }
                else {
                    plants.erase(i);
                }
            }
            return cur_layout;
        }

        // return the current sum of plants
        int plant_sum() {
            return std::accumulate(plants.begin(), plants.end(), 0);
        }

        // cache the current plants/endpoints/sum/generation
        optional<tuple2i> cache(size_t layout, int sum, int gen) {
            auto itr = history.find(layout);
            if(itr != history.end()) {
                return itr->second;
            }
            else {
                history[layout] = make_tuple(sum, gen);
                return nullopt;
            }
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

        // add a rule if it results in a plant next generation
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

    // update each generation until we find a duplicate pots layout
    for (int i=1; i <= GENERATIONS; i++) {
        // get the new layout
        auto layout = pots.update();
        // ... and the sum of that layout
        auto cur_sum = pots.plant_sum();
        // update/check the cache for this layout
        auto cached = pots.cache(layout, cur_sum, i);

        // found a repeated pots layout
        if (cached.has_value()) {
            auto [sum, gen] = cached.value();
            cout << "Duplicate found: "
                 << "{gen=" << i << ", sum=" << cur_sum << "} "
                 << " matches {gen=" << gen << ", sum=" << sum << "}" << endl;

            // CONVERGED_DIFF was found by printing diffs between historical layouts;
            //     It remains constant after a repeat.
            // The TARGET_GENERATION's sum should be CONVERGED_DIFF multiplied by the
            //     number of generations remaining between the current and TARGET_GENERATION
            cout << cur_sum + (TARGET_GENERATION-i)*CONVERGED_DIFF << endl;
            break;
        }
    }

    return 0;
}

