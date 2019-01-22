#include <iostream>
#include <cctype>
#include <algorithm>
#include <regex>
#include <cassert>
#include <map>
#include <queue>
#include <set>
#include <functional>

using namespace std;

int main (int argc, char** argv) {

    // regex match single capital letters
    const regex letter_re("(\\b[A-Z]\\b)");
    const sregex_iterator re_itr_end = sregex_iterator();

    // two directed graphs for tracking prerequisites and dependencies
    map<char, set<char>> prereqs;
    map<char, set<char>> deps;

    // processing order for final answer
    vector<char> done;

    // read all non-empty lines
    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;

        // first letter is prereq, second is dependency
        auto re_itr = sregex_iterator(input.begin(), input.end(), letter_re);
        assert(re_itr != re_itr_end);
        char prereq = (re_itr++)->str().front();
        char dep = (re_itr++)->str().front();
        assert(re_itr == re_itr_end); // make sure input is relatively OK
        prereqs[dep].insert(prereq);
        deps[prereq].insert(dep);
    } 

    // processing queue (alphabetical order)
    priority_queue<char, vector<char>, greater<char>> q;
 
    // find steps with no prerequisites
    for (const auto& step : deps)
        if (!prereqs.count(step.first))
            q.push(step.first);
    
    while(!q.empty()) {
        auto cur = q.top();
        q.pop();
        // for each dependency of this step
        for (auto& dep : deps[cur]) {
            // remove this step from prereq list
            prereqs[dep].erase(cur);
            // if this was the last prereq, enqueue
            if (prereqs[dep].empty()) {
                q.push(dep);
            }
        }
        // track processing order
        done.push_back(cur);
    }

    for_each(done.begin(), done.end(), [](const auto& chr){ cout << chr; });
    cout << endl;

    return 0;
}
