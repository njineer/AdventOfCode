#include <iostream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <climits>

using namespace std;


// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(
        std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), 
        s.end()
    );
}

int react(const std::string& input) {
    vector<bool> keep(input.size(), true);
    int l=0, r=1;

    while(l < input.size() && r < input.size()) {
        // upper/lower case match (e.g. a/A)
        if (abs(input[l]-input[r]) == 32) {
            // delete these indices
            keep[l] = false;
            keep[r] = false;

            // find their adjacent, non-deleted indices (look for nearest true)
            auto left = find(next(keep.rbegin(), keep.size()-l), keep.rend(), true); 
            auto right = find(next(keep.begin(), r), keep.end(), true); 

            // if we hit the left end, go right
            if (left == keep.rend()) {
                l = distance(keep.begin(), right);
                right = find(next(keep.begin(), l), keep.end(), true); 
            } else {
                // go left
                l = distance(left, keep.rend()) - 1;
            }
            // if we hit the right end (before or after hitting left end), we're done
            if (right == keep.end()) {
                break;
            } else {
                r = distance(keep.begin(), right);
            }
        } 
        // no match/reaction; keep going right
        else {
            l = r++;
        }
    }

    return count(keep.begin(), keep.end(), true);
}

int main (int argc, char** argv) {
    string input; 
    getline(cin, input);
    rtrim(input);

    map<char, int> removal_results;
    transform(input.begin(), input.end(), inserter(removal_results, removal_results.begin()),
              [](const char& c){ return make_pair(toupper(c), INT_MAX); });

    for (auto& chr : removal_results) {
        string chr_removed;
        remove_copy_if(input.begin(), input.end(), back_inserter(chr_removed),
                       [chr](const char& c){ return toupper(c) == chr.first; });
        chr.second = react(chr_removed);
    }

    auto min_itr = min_element(removal_results.begin(), removal_results.end(),
        [](const auto& p1, const auto& p2){ return p1.second < p2.second; });

    cout << "min: " << min_itr->first 
         << " leaves " << min_itr->second 
         << " units remaining." << endl;
    
    return 0;
}
