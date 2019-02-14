#include <iostream>
#include <map>
#include <string>

using namespace std;

int main (int argc, char** argv) {

    int twos=0, threes=0;

    // for each line
    for (string input; getline(cin, input);) {

        // map each character to its frequency
        map<char, int> counts;  
        for (auto& chr : input) {
            counts[chr]++; 
        }

        // look for characters that occur 2x or 3x
        bool has2=false, has3=false;
        for (auto& count : counts) {
            if (count.second == 2)
                has2 = true;
            else if (count.second == 3)
                has3 = true;
        }

        // update overall count
        twos += has2;
        threes += has3;
    }

    cout << twos*threes << endl;

    return 0;
}
