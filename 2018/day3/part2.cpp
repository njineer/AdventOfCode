#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>

using namespace std;

int main (int argc, char** argv) {

    // for each line
    for (string input; getline(cin, input);) {
        if (any_of(input.begin(), input.end(), [](auto& chr){return !isspace(chr);}))
        //
    }

    return 0;
}
