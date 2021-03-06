#include <iostream>
#include <regex>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>

using namespace std;

template <class Iterator, class Container>
inline void forward(Container& c, Iterator& itr, int offset=2) {
    for(int i=0; i < offset; i++) {
        itr++;
        if (itr == c.end()) {
            itr = c.begin();
        }
    }
}

template <class Iterator, class Container>
inline void reverse(Container& c, Iterator& itr, int offset=7) {
    for(int i=0; i < offset; i++) {
        if (itr == c.begin()) {
            itr = c.end();
        }
        itr--;
    }
}

int main (int argc, char** argv) {

    const regex num_re("(\\d+)");

    // parse input
    string input; 
    getline(cin, input);
    auto re_itr = sregex_iterator(input.begin(), input.end(), num_re);
    auto num_players = stoi((re_itr++)->str());
    auto last_marble = stoi((re_itr++)->str());
    assert(re_itr == sregex_iterator());

    cout << "players: " << num_players << endl << "last marble: " << last_marble << endl;

    vector<int64_t> scores(num_players+1, 0);
    list<int> marbles;
    marbles.push_back(0);
    auto cur = marbles.begin();
    for (int m=1, p=0; m <= last_marble; m++, p=(p+1)%num_players) {
        if (m % 23) {
            forward(marbles, cur); 
            if (cur == marbles.begin()) {
                marbles.push_back(m);
                cur = --marbles.end();
            } else {
                cur = marbles.insert(cur, m);
            }
        } else {
            reverse(marbles, cur);
            scores[p+1] += m + *cur;
            cur = marbles.erase(cur);
            if (cur == marbles.end()) {
                cur = marbles.begin();
            }
        }
    }

    cout << "max score: " << (*max_element(scores.begin(), scores.end())) << endl;
    return 0;
}
