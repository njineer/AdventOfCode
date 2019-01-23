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

#define WORKER_COUNT 5
#define STEP_TIME 60

template <class T>
void heap_push(vector<T>& v, const T& item) {
    v.push_back(item);
    push_heap(v.begin(), v.end());
}

template <class T>
T heap_pop(vector<T>& v) {
    pop_heap(v.begin(), v.end());
    auto item = v.back();
    v.pop_back();
    return item;
}


int main (int argc, char** argv) {

    // regex match single capital letters
    const regex letter_re("(\\b[A-Z]\\b)");
    const sregex_iterator re_itr_end = sregex_iterator();

    // two directed graphs for tracking prerequisites and dependencies
    map<char, set<char>> prereqs;
    map<char, set<char>> deps;

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
    for (const auto& step : deps) {
        if (!prereqs.count(step.first)) {
            q.push(step.first);
        }
    }

    int total_time = 0;
    vector<pair<int, char>> wq;
    while(!q.empty() || !wq.empty()) {

        // while there are workers and work to be done
        while(!q.empty() && wq.size() < WORKER_COUNT) {
            auto cur = q.top();
            q.pop();
            // assign to worker (with time to completion)
            heap_push(wq, make_pair(-(STEP_TIME + cur - 'A' + 1), cur)); // negative for 'min-heap'
        }

        while(!wq.empty()) {
            auto cur = heap_pop(wq);
            
            // steps that are now (or already) done
            if (cur.first >= 0) {
                // for each dependency of this step
                for (auto& dep : deps[cur.second]) {
                    // remove this step from its prereq list
                    prereqs[dep].erase(cur.second);
                    // if this was the last prereq, enqueue
                    if (prereqs[dep].empty()) {
                        q.push(dep);
                    }
                }
            } 
            // nearest to completion
            else {
                // for each dependency of this step
                for (auto& dep : deps[cur.second]) {
                    // remove this step from its prereq list
                    prereqs[dep].erase(cur.second);
                    // if this was the last prereq, enqueue
                    if (prereqs[dep].empty()) {
                        q.push(dep);
                    }
                }
                // move time up to when the next step is done
                for_each(wq.begin(), wq.end(), [cur](auto& step){step.first -= cur.first;});
                
                // track total time
                total_time -= cur.first;
                break;
            }
        }
    }

    cout << total_time << endl;
    return 0;
}

