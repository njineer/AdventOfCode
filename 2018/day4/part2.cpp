#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <tuple>
#include <vector>
#include <map>
#include <array>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iterator>

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
            // state change (i.e. no Guard id in input)
            id = -1;
            smatch re_match;
            if (regex_match(input, re_match, state_re)) {
                auto match_str = re_match.str(1);
                if (match_str == "wakes") {
                    state = State::awake;
                } else if (match_str == "falls") {
                    state = State::asleep;
                } else {
                    // should be one of two options
                    //throw runtime_error("Bizarre regex error from input:" + input);
                    cerr << ("Bizarre regex error from input:" + input) << endl;
                }
            } else {
                // malformed input
                //throw runtime_error("Missing expected state keyword: " + input);
                cerr << ("Missing expected state keyword: " + input) << endl;
            }
        } else {
            // no state necessary (guard change)
            state = State::unknown;
        }
    }

    auto str() const {
        ostringstream ss;
        ss << "<Timestamp("
           << "year=" << year
           << ", month=" << month
           << ", day=" << day
           << ", hour=" << hour
           << ", minute=" << minute
           << ", id=" << id
           << ", state=";
        switch(state) {
            case State::awake: ss << "awake"; break;
            case State::asleep: ss << "asleep"; break;
            case State::unknown: 
            default: ss << "unknown"; break;
        }
        ss << ")>";
        return ss.str();
    }

    auto tuple() const {
        return tie(year, month, day, hour, minute, id, state);
    }

    friend bool operator< (const Timestamp& a, const Timestamp& b);
    friend bool operator== (const Timestamp& a, const Timestamp& b);

};
const regex Timestamp::num_re("(\\d+)");
const regex Timestamp::state_re("\\[.*\\]\\s*(wakes|falls)\\s+\\w+\\s*$");
const sregex_iterator Timestamp::re_itr_end = sregex_iterator();

//
// Timestamp operators
//
bool operator< (const Timestamp& a, const Timestamp& b) {
    return a.tuple() < b.tuple();
}

bool operator== (const Timestamp& a, const Timestamp& b) {
    return a.tuple() == b.tuple();
}


int main (int argc, char** argv) {

    // parse inputs into vector; sort
    vector<Timestamp> timestamps;
    for (string input; getline(cin, input);) {
        timestamps.emplace_back(input);
    } 
    sort(timestamps.begin(), timestamps.end());

    map<int, array<int, 60>> guard_sleep_counts;

    int cur_guard = -1;
    int sleep_start = -1;
    for (auto& ts : timestamps) {
        if (ts.id != -1) {
            cur_guard = ts.id;
        } else {
            if (ts.state == State::asleep) {
                sleep_start = ts.minute;
            } else if (ts.state == State::awake) {
                for (int i=sleep_start; i < ts.minute; ++i) {
                    guard_sleep_counts[cur_guard][i]++;
                }
            }
        }
    }

    auto consistent_guard = -1;
    auto max_minutes_slept = -1;
    for (auto& guard : guard_sleep_counts) {
        auto minutes_slept = max_element(guard.second.begin(), guard.second.end());
        if (*minutes_slept > max_minutes_slept) {
            max_minutes_slept = *minutes_slept;
            consistent_guard = guard.first;
        }
    }
    cout << "consistent guard: " << consistent_guard << endl;

    auto consistent_minute = distance(
            guard_sleep_counts[consistent_guard].begin(), 
            max_element(guard_sleep_counts[consistent_guard].begin(),
                        guard_sleep_counts[consistent_guard].end()));
    cout << "consistent minute: " << consistent_minute << endl;

    cout << consistent_guard * consistent_minute << endl;
    
    return 0;
}
