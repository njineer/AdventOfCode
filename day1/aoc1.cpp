#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    string input;

    switch (argc) {
        case 1: getline(cin, input); break;
        case 2: input = argv[1]; break;
        default: cerr << "Too many arguments." << endl; return 1;
    }

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

    cout << "sum: " << sum << endl;

    return 0;
}

