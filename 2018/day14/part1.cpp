#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define RECIPES_AFTER 10

// for visualizing/debug
template<typename T>
string str_vec(const vector<T>& v) {
    ostringstream ss;
    for (auto& x : v)
        ss << x;
    return ss.str();
}

int main (int argc, char** argv) {

    string input; 
    getline(cin, input);
    int recipe_count = stoi(input);

    vector<int> recipes {3, 7};
    recipes.reserve(recipe_count + RECIPES_AFTER);
    int elf1 = 0, elf2 = 1;

    for (int i=0; i < recipe_count + RECIPES_AFTER; i++) {
        // combine recipes
        auto cur_sum = recipes[elf1] + recipes[elf2];

        // add new recipes
        if (cur_sum > 9) {
            // recipes are never >9, ergo sum is never >18
            recipes.push_back(1);
        }
        recipes.push_back(cur_sum % 10);

        // update each elf's current recipe
        elf1 = (elf1 + recipes[elf1] + 1) % recipes.size();
        elf2 = (elf2 + recipes[elf2] + 1) % recipes.size();
    }

    ostringstream ss;
    for (int i=recipe_count; i < recipe_count + RECIPES_AFTER; i++) {
        ss << recipes[i];
    }
    cout << ss.str() << endl;

    return 0;
}

