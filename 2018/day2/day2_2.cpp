#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main (int argc, char** argv) {

    vector<string> ids;

    // for each line
    for (string input; getline(cin, input);) {
        ids.emplace_back(input);    
    }


    return 0;
}

