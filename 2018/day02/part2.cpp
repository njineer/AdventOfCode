#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>

using namespace std;

int main (int argc, char** argv) {

    vector<string> ids;

    // read and store each non-empty line
    for (string input; getline(cin, input);) {
        if (any_of(input.begin(), input.end(), [](auto& chr){return !isspace(chr);}))
            ids.emplace_back(input);    
    }

    int diff_idx = -1;
    auto itr1 = ids.begin();
    auto itr2 = next(itr1);
    auto end = ids.end();

    // compare each unique pair of words; find the two with a single char difference
    for (; itr1 != end; ++itr1) {
        for (itr2 = next(itr1); itr2 != end; ++itr2) {
            diff_idx = -1;
            for (int k=0; k < itr1->size(); k++) {
                if (itr1->at(k) != itr2->at(k)) {
                    if (diff_idx >= 0) {
                        // quit if we already found a diff
                        diff_idx = -1;
                        break;
                    }
                    else
                        // track the index of the diff
                        diff_idx = k;
                }
            }
            if (diff_idx >= 0) {
                // found the index of the single diff
                cout << diff_idx << endl;
                break;
            }
        }
        if (diff_idx >= 0)
            // propagate the early exit
            break;
    }

    // print the matching characters of the two single-difference-words
    cout << itr1->substr(0, diff_idx) << itr1->substr(diff_idx+1) << endl;

    return 0;
}

