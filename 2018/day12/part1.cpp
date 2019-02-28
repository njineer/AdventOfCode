#include <iostream>
#include <cctype>
#include <regex>
#include <algorithm>
#include <cassert>
#include <map>
#include <set>
#include <iterator>

using namespace std;

enum class Neighbors : unsigned int {
    LEFT2   = 0x10,
    LEFT1   = 0x08,
    CURRENT = 0x04,
    RIGHT1  = 0x02,
    RIGHT2  = 0x01
};

template <typename Iterator>
unsigned int make_pattern(Iterator begin, Iterator end) {
    unsigned int pattern = 0;
    int i=distance(begin, end);
    for(auto itr = begin; itr != end; itr++, i--) {
        if (*itr == '#') {
            pattern |= 1 << (i-1);
        }
    }
    return pattern;
}

int main (int argc, char** argv) {

    map<int, bool> pots;
    set<unsigned int> rules;

    const regex init_re("initial state:\\s+([#\\.]+)");
    const regex growth_re("([#\\.]+)\\s+=>\\s+([#\\.])");
    smatch match;

    // set initial state of pots
    string input;
    getline(cin, input);
    if (regex_match(input, match, init_re)) {
        auto initial = match.str(1);
        for (auto i=0; i < initial.size(); i++) {
            if (initial[i] == '#') {
                pots[i] = true;
            }
        }
    } 
    else {
        cout << "Expected initial state in first line, got '" << input << "'" << endl;
        throw;
    }

    // parse generational patterns
    for (string input; getline(cin, input);) {
        // skip blank lines
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            continue;

        if (regex_match(input, match, growth_re)) {
            if (match.str(2) == "#") {
                auto pattern_str = match.str(1);
                rules.insert(make_pattern(pattern_str.begin(), pattern_str.end()));
            }
        } 
        else {
            cout << "Unexpected pattern: '" << input << "'" << endl;
            throw;
        }
    }
   
    cout << hex; for (auto& r : rules) cout << r << endl;
    
    return 0;
}
