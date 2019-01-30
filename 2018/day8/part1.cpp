#include <iostream>
#include <string>

using namespace std;

int parse(istream& is) {
    int children, metadata, md_sum=0, md;

    // read counts for this node
    is >> children >> metadata;

    // recursively accumulate metadata sum for each child
    for (int i=0; i < children; i++) {
        md_sum += parse(is);
    } 

    // accumulate metadata sum for this child
    for (int i=0; i<metadata; i++) {
        is >> md;
        md_sum += md;
    }
    
    // return total/recursive metadata sum for this node
    return md_sum;
}

int main (int argc, char** argv) {
    cout << endl << "metadata total: " << parse(cin) << endl;
    return 0;
}
