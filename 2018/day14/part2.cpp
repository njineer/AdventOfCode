#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#define RECIPES_AFTER 10

vector<int> make_pattern(const string& s) {
    vector<int> pattern;
    for (auto& chr : s) {
        if (isspace(chr)) {
            continue;
        }
        else {
            pattern.push_back(chr-'0');
        }
    }
    return pattern;
}

int main (int argc, char** argv) {

    string input; 
    getline(cin, input);
    vector<int> target_pattern;
    for (auto& chr : input) {
        if (isspace(chr)) {
            continue;
        }
        else {
            target_pattern.push_back(chr-'0');
        }
    }

    vector<int> recipes {3, 7};
    int elf1 = 0, elf2 = 1;
    int current_pattern = 37;
    int recipe_count = 0;

    while(true) {
        auto cur_sum = recipes[elf1] + recipes[elf2];
        if (cur_sum > 9) {
            // recipes are never >9, ergo sum is never >18
            recipes.push_back(1);
            if (equal(target_pattern.begin(), target_pattern.end(), recipes.end()-target_pattern.size())) {
                break;
            }
        }
        recipes.push_back(cur_sum % 10);
        if (equal(target_pattern.begin(), target_pattern.end(), recipes.end()-target_pattern.size())) {
            break;
        }

        elf1 = (elf1 + recipes[elf1] + 1) % recipes.size();
        elf2 = (elf2 + recipes[elf2] + 1) % recipes.size();
    }

    cout << recipes.size()-target_pattern.size() << endl;
    return 0;
}


