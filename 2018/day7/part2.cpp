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

template<typename T>
void print_container(T&& c) {
    cout << "{";
    for (const auto& elem : c) {
        cout << elem << ", ";
    }
    cout << "}" << endl;
}

int main (int argc, char** argv) {
    const regex letter_re("(\\b[A-Z]\\b)");
    const sregex_iterator re_itr_end = sregex_iterator();

    map<char, set<char>> prereqs;
    map<char, set<char>> deps;
    vector<char> done;

    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;
        auto re_itr = sregex_iterator(input.begin(), input.end(), letter_re);
        assert(re_itr != re_itr_end);
        char prereq = (re_itr++)->str().front();
        char dep = (re_itr++)->str().front();
        assert(re_itr == re_itr_end);
        prereqs[dep].insert(prereq);
        deps[prereq].insert(dep);
    } 

    priority_queue<char, vector<char>, greater<char>> q;
  
    for (const auto& step : deps)
        if (!prereqs.count(step.first))
            q.push(step.first);
    
    while(!q.empty()) {
        auto cur = q.top();
        q.pop();
        for (auto& dep : deps[cur]) {
            prereqs[dep].erase(cur);
            if (prereqs[dep].empty()) {
                q.push(dep);
            }
        }
        done.push_back(cur);
    }

    for (const auto& step : done)
        cout << step;
    cout << endl;

    return 0;
}

