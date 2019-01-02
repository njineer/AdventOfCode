#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <regex>
#include <map>
#include <stdexcept>

using namespace std;

// guard state
enum State { unknown, awake, asleep };

// timestamps from input
struct Timestamp {
    const static regex num_re;
    const static regex state_re;
    const static sregex_iterator re_itr_end;

    int year, month, day, hour, minute, id;
    State state;

    Timestamp(const string& input) {
        // extract all numbers
        auto re_itr = sregex_iterator(input.begin(), input.end(), num_re);
        int i=0;
        for (; re_itr != re_itr_end; ++re_itr, ++i) {
            switch(i) {
                case 0: year = stoi(re_itr->str()); break;
                case 1: month = stoi(re_itr->str()); break;
                case 2: day = stoi(re_itr->str()); break;
                case 3: hour = stoi(re_itr->str()); break;
                case 4: minute = stoi(re_itr->str()); break;
                case 5: id = stoi(re_itr->str()); break; // optional
                default: break;
            }
        }
        if (i == 5) {
            // status change (i.e. no Guard id in input)
            smatch re_match;
            if (regex_match(input, re_match, state_re)) {
                auto match_str = re_match.str(1);
                if (match_str == "wakes") {
                    state = State::awake;
                } else if (match_str == "falls") {
                    state = State::asleep;
                } else {
                    // should be one of two options
                    throw runtime_error("Bizarre regex error from input:" + input);
                }
            } else {
                // malformed input
                throw runtime_error("Missing expected status keyword: " + input);
            }
        } else {
            // no state necessary (guard change)
            state = State::unknown;
        }
    }
};
const regex Timestamp::num_re("(\\d+)");
const regex Timestamp::state_re("^\\[.*\\](wakes|falls)$");
const sregex_iterator Timestamp::re_itr_end = sregex_iterator();



int main (int argc, char** argv) {

    vector<Timestamp> timestamps;
    // parse inputs into vector
    for (string input; getline(cin, input);) {
        timestamps.emplace_back(input);
    } 

    return 0;
}
