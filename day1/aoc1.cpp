#include <iostream>
#include <string>

using namespace std;

int aoc1 (const string& input) {
    int sum = 0;
    auto itr1 = input.begin();
    auto itr2 = next(itr1, 1);

    for(; itr2 != input.end(); ++itr1, ++itr2) {
        if (*itr1 == *itr2) {
            sum += *itr1 - '0'; // char to int
        }
    }
    if (input.front() == input.back())
        sum += input.back() - '0';

    return sum;
}

int aoc1_2 (const string& input) {
    int sum = 0;
    int inputLength = input.size();
    int step = inputLength/2;
    for (int i=0; i < inputLength; ++i) {
        int halfway = (i + step) % inputLength;
        if (input[i] == input[halfway]) {
            sum += input[i] - '0';
        }
    }
    return sum;
}


int main(int argc, char** argv) {
    string input;

    switch (argc) {
        case 1: getline(cin, input); break;
        case 2: input = argv[1]; break;
        default: cerr << "Too many arguments." << endl; return 1;
    }

    auto sum = aoc1(input);
    cout << "sum 1: " << sum << endl;

    auto sum2 = aoc1_2(input);
    cout << "sum 2: " << sum2 << endl;

    return 0;
}

