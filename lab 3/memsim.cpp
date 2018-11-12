#include <iostream>

#include "function.h"

int main(int argc, char* argv[]) {

    if (argc < 4){
        cout << "EROOR:Too few argument, please use the following format\n";
        cout << "i.e.256 pg-reference.txt output.txt" << endl;
        cout << "Program exiting.";
        return 0;
    }
    int framesize = atoi(argv[1]);
    string input = argv[2];
    string output = argv[3];

    // getting sequence from inputfile
    vector<page> inputRequest = getSequence(input);

    // stores the result in this table when we call simulate.
    vector<result> resultTable;
    for (int i = 0; i < 4 ; ++i) {
        resultTable.push_back(result());
    }

    // simulating
    simulate(framesize,resultTable,input);


    // printing.
    printAndOutput(resultTable,output,framesize);

    return 0;
}