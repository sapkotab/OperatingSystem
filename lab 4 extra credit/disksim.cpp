#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;
const int MIN_NUM_ON_CYLINDER = 0;

const int MAX_NUM_ON_CYLINDER = 199;


struct result {
    int cylinderCount;
    string type;
    vector<int> simulated;
};

// forward declaration of fuctions
void assignRequest(string input, vector<int>& request);
void simulateAlgorithm (vector<int> request, vector <result>& allresult, int head, int direction);
void printAndOutput (vector <result>& allresult, int head, string output, int direction);



int main(int argc, char *argv[]) {
    int     head,
            direction;
    string  input,
            output;

    // making sure all arguments are available
    if ( argc < 5){
        cout << "Too few argument. Please following format.\n";
        cout << "./disksim <position> <direction> <input> <output>\n";
        cout << "Program exiting.\n";
        return 0;
    }

    // making sure head is withing range.
    head = atoi(argv[1]);
    if(head < MIN_NUM_ON_CYLINDER || head > MAX_NUM_ON_CYLINDER ){
        cout << "Position value is out of range, it must be between 0-199.\n";
        cout << "Program exiting.\n";
        return 0;
    }

    // making sure direction is within range
    direction  = atoi(argv[2]);
    if(direction != 1 && direction != 0 ){
        cout << "Wrong direction value, it must be 0 or 1\n";
        cout << "Program exiting\n";
        return  0;
    }

    // reading input and output file name.
    input = argv[3];
    output = argv[4];

    // disk request sequence.
    vector<int> request;

    // result storage
    vector <result> allResult;

    assignRequest(input, request);

    simulateAlgorithm(request,allResult,head,direction);

    printAndOutput(allResult,head,output, direction);

    return 0;
}

void assignRequest(string input, vector<int>& request){
    ifstream inputFile(input);
    string value;

    while(!inputFile.eof()){
        getline(inputFile,value,',');
        request.push_back(stoi(value));
    }
    inputFile.close();
}

void simulateAlgorithm (vector<int> request, vector <result>& allresult, int head, int direction){

    // we manipulate tempRequest in each simulation.
    vector<int> tempRequest;
    for (int i = 0; i < 4; ++i) {
        switch (i){
            case 0:{
                tempRequest = request;
                result temp;
                temp.cylinderCount = 0;
                temp.type = "FCFS";
                allresult.push_back(temp);

                while (!tempRequest.empty()) {
                    // we push whatever comes first in tempRequest.
                    allresult.at(i).simulated.push_back(reinterpret_cast<int &&>(tempRequest.front()));
                    tempRequest.erase(tempRequest.begin());
                }

                // calculating cylinder count
                // first from head and and from simulated request.
                if(!allresult.at(i).simulated.empty()) {
                    allresult.at(i).cylinderCount += (abs(head - allresult.at(i).simulated.at(0)));
                }
                for (int k = 1; k < allresult.at(i).simulated.size(); ++k) {
                    allresult.at(i).cylinderCount += (abs(allresult.at(i).simulated.at(k)-allresult.at(i).simulated.at(k-1)));
                }
                tempRequest.clear();
            }
                break;
            case 1:{
                tempRequest = request;
                result temp;
                temp.cylinderCount = 0;
                temp.type = "SSTF";
                allresult.push_back(temp);

                int current = head;
                int shortestSeek = std::numeric_limits<int>::max();
                int shortestSeekIndex = 0;

                // while request is not assigned properly
                while(!tempRequest.empty()){
                    for (int j = 0; j < tempRequest.size(); ++j) {
                        if(abs(tempRequest.at(j)-current) < shortestSeek){
                            shortestSeek = abs(tempRequest.at(j)- current);
                            shortestSeekIndex = j;
                        }
                    }
                    allresult.at(i).simulated.push_back(reinterpret_cast<int &&>(tempRequest.at(shortestSeekIndex)));
                    tempRequest.erase(tempRequest.begin()+shortestSeekIndex);
                    current = allresult.at(i).simulated.back();
                    shortestSeek = std::numeric_limits<int>::max();
                    shortestSeekIndex = 0;
                }

                // calculating cylinder count
                // from head to next request
                if(!allresult.at(i).simulated.empty()) {
                    allresult.at(i).cylinderCount += (abs(head - allresult.at(i).simulated.at(0)));
                }
                // from request to request.
                for (int k = 1; k < allresult.at(i).simulated.size(); ++k) {
                    allresult.at(i).cylinderCount += (abs(allresult.at(i).simulated.at(k)-allresult.at(i).simulated.at(k-1)));
                }
                tempRequest.clear();
            }
                break;
            case 2: {
                tempRequest = request;
                result temp;
                temp.cylinderCount = 0;
                temp.type = "SCAN";
                allresult.push_back(temp);


                // while head is going up
                if(direction == 1) {
                    int current = head;
                    int shortestSeek = std::numeric_limits<int>::max();
                    int shortestSeekIndex = 0;
                    int maxElement = *max_element(tempRequest.begin(), tempRequest.end());

                    // while request is still available
                    while (!tempRequest.empty()) {

                        // finding shortest seek element
                        for (int j = 0; j < tempRequest.size(); ++j) {
                            if ((tempRequest.at(j) - current) < shortestSeek && current < tempRequest.at(j) ) {
                                shortestSeek = abs(tempRequest.at(j) - current);
                                shortestSeekIndex = j;
                            }
                        }

                        // and push that element in result vector
                        allresult.at(i).simulated.push_back( reinterpret_cast<int &&>(tempRequest.at(shortestSeekIndex)));

                        // when it reache the highest number  push 199 and go other way
                        if (tempRequest.at(shortestSeekIndex) == maxElement) {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            allresult.at(i).simulated.push_back(MAX_NUM_ON_CYLINDER);
                            current = MAX_NUM_ON_CYLINDER;
                            shortestSeek = std::numeric_limits<int>::max();
                            while (!tempRequest.empty()) {
                                for (int j = 0; j < tempRequest.size(); ++j) {
                                    if ((current - tempRequest.at(j)) < shortestSeek) {
                                        shortestSeek = abs(tempRequest.at(j) - current);
                                        shortestSeekIndex = j;
                                    }
                                }
                                allresult.at(i).simulated.push_back( reinterpret_cast<int &&>(tempRequest.at(shortestSeekIndex)));
                                tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                                current = allresult.at(i).simulated.back();
                                shortestSeek = std::numeric_limits<int>::max();
                                shortestSeekIndex = 0;
                            }
                        }else {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            current = allresult.at(i).simulated.back();
                            shortestSeek = std::numeric_limits<int>::max();
                            shortestSeekIndex = 0;
                        }
                    }

                } else{ // similar as above but just opposite
                    int current = head;
                    int shortestSeek = std::numeric_limits<int>::max();
                    int shortestSeekIndex = 0;
                    int minElement = *min_element(tempRequest.begin(), tempRequest.end());
                    while (!tempRequest.empty()) {
                        for (int j = 0; j < tempRequest.size(); ++j) {
                            if ((current - tempRequest.at(j)) < shortestSeek && current > tempRequest.at(j) ) {
                                shortestSeek = abs(current - tempRequest.at(j));
                                shortestSeekIndex = j;
                            }
                        }
                        allresult.at(i).simulated.push_back( reinterpret_cast<int &&>(tempRequest.at(shortestSeekIndex)));
                        if (tempRequest.at(shortestSeekIndex) == minElement) {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            allresult.at(i).simulated.push_back(MIN_NUM_ON_CYLINDER);
                            current = MIN_NUM_ON_CYLINDER;
                            shortestSeek = std::numeric_limits<int>::max();
                            while (!tempRequest.empty()) {
                                for (int j = 0; j < tempRequest.size(); ++j) {
                                    if ((tempRequest.at(j) - current) < shortestSeek) {
                                        shortestSeek = abs(tempRequest.at(j) - current);
                                        shortestSeekIndex = j;
                                    }
                                }
                                allresult.at(i).simulated.push_back(reinterpret_cast<int &&>(tempRequest.at(shortestSeekIndex)));
                                tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                                current = allresult.at(i).simulated.back();
                                shortestSeek = std::numeric_limits<int>::max();
                                shortestSeekIndex = 0;
                            }
                        }else {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            current = allresult.at(i).simulated.back();
                            shortestSeek = std::numeric_limits<int>::max();
                            shortestSeekIndex = 0;
                        }
                    }
                }

                // calculating cylinder count
                if(!allresult.at(i).simulated.empty()) {
                    allresult.at(i).cylinderCount += (abs(head - allresult.at(i).simulated.at(0)));
                }
                for (int k = 1; k < allresult.at(i).simulated.size(); ++k) {
                    allresult.at(i).cylinderCount += (abs(allresult.at(i).simulated.at(k)-allresult.at(i).simulated.at(k-1)));
                }
                tempRequest.clear();
            }
                break;
            case 3:{
                tempRequest = request;
                result temp;
                temp.cylinderCount = 0;
                temp.type = "C-LOOK";
                allresult.push_back(temp);

                int maxElement = *max_element(tempRequest.begin(), tempRequest.end());
                int minElement = *min_element(tempRequest.begin(), tempRequest.end());

                // while head is going up
                if(direction == 1) {
                    int current = head;
                    int shortestSeek = std::numeric_limits<int>::max();
                    int shortestSeekIndex = 0;

                    // while request is still available
                    while (!tempRequest.empty()) {

                        // finding shortest seek element
                        for (int j = 0; j < tempRequest.size(); ++j) {
                            if ((tempRequest.at(j) - current) < shortestSeek && current < tempRequest.at(j) ) {
                                shortestSeek = abs(tempRequest.at(j) - current);
                                shortestSeekIndex = j;
                            }
                        }

                        // and push that element in result vector
                        allresult.at(i).simulated.push_back( reinterpret_cast<int &&>(tempRequest.at(shortestSeekIndex)));

                        // when it reache the highest number  push 199 and go other way
                        if (tempRequest.at(shortestSeekIndex) == maxElement) {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            allresult.at(i).simulated.push_back(minElement);
                            tempRequest.erase(find(tempRequest.begin(), tempRequest.end(),minElement));
                            current = minElement;
                            shortestSeek = std::numeric_limits<int>::max();
                        }else {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            current = allresult.at(i).simulated.back();
                            shortestSeek = std::numeric_limits<int>::max();
                            shortestSeekIndex = 0;
                        }
                    }

                } else{ // similar as above but just opposite
                    int current = head;
                    int shortestSeek = std::numeric_limits<int>::max();
                    int shortestSeekIndex = 0;
                    while (!tempRequest.empty()) {
                        for (int j = 0; j < tempRequest.size(); ++j) {
                            if ((current - tempRequest.at(j)) < shortestSeek && current > tempRequest.at(j) ) {
                                shortestSeek = abs(current - tempRequest.at(j));
                                shortestSeekIndex = j;
                            }
                        }
                        allresult.at(i).simulated.push_back( reinterpret_cast<int &&>(tempRequest.at(shortestSeekIndex)));
                        if (tempRequest.at(shortestSeekIndex) == minElement) {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            allresult.at(i).simulated.push_back(maxElement);
                            tempRequest.erase(find(tempRequest.begin(), tempRequest.end(),maxElement));
                            current = maxElement;
                            shortestSeek = std::numeric_limits<int>::max();
                        }else {
                            tempRequest.erase(tempRequest.begin() + shortestSeekIndex);
                            current = allresult.at(i).simulated.back();
                            shortestSeek = std::numeric_limits<int>::max();
                            shortestSeekIndex = 0;
                        }
                    }
                }

                // calculating cylinder count
                if(!allresult.at(i).simulated.empty()) {
                    allresult.at(i).cylinderCount += (abs(head - allresult.at(i).simulated.at(0)));
                }
                for (int k = 1; k < allresult.at(i).simulated.size(); ++k) {
                    allresult.at(i).cylinderCount += (abs(allresult.at(i).simulated.at(k)-allresult.at(i).simulated.at(k-1)));
                }
                tempRequest.clear();
            }
                break;
            default:
                cout << "ERROR: never should reach here\n";
        }
    }
}

void printAndOutput (vector <result>& allresult, int head, string output, int direction){

    for (int i = 0; i < 4 ; ++i) {
        cout << "\n==============================================================================\n";
        cout << "[" << allresult.at(i).type << "] Disk Scheduling Algorithm Simulation\n";
        cout << "Currently disk head at " << head << " and is moving ";
        if (direction == 1){
            cout << "up\n";
        } else{
            cout << "down\n";
        }
        cout << "==============================================================================\n";
        for (int j = 0; j < allresult.at(i).simulated.size() ; ++j) {
            if(j==0){
                cout << "[" << head << "->";
            }
            cout << allresult.at(i).simulated.at(j) << "]";
            // we don't want this for last element.
            if (j != allresult.at(i).simulated.size()-1){
                cout << ", ";
                if (((j+1)%7) == 0 && j != 0){
                    cout << "\n";
                }
                cout << "[" << allresult.at(i).simulated.at(j) << "->";
            } else{
                cout << endl << endl;
            }
        }

        cout << "Total number of cylinder movements: " << allresult.at(i).cylinderCount  << " cylinders\n";
    }

    ofstream outputFile(output);
    for (int i = 0; i < 4 ; ++i) {
        outputFile << "\n==============================================================================\n";
        outputFile << "[" << allresult.at(i).type << "] Disk Scheduling Algorithm Simulation\n";
        outputFile << "Currently disk head at " << head << " and is moving ";
        if (direction == 1){
            outputFile << "up\n";
        } else{
            outputFile << "down\n";
        }
        outputFile << "==============================================================================\n";
        for (int j = 0; j < allresult.at(i).simulated.size() ; ++j) {
            if(j==0){
                outputFile << "[" << head << "->";
            }
            outputFile << allresult.at(i).simulated.at(j) << "]";
            // we don't want this for last element.
            if (j != allresult.at(i).simulated.size()-1){
                outputFile << ", ";
                if (((j+1)%7) == 0 && j != 0){
                    outputFile << "\n";
                }
                outputFile << "[" << allresult.at(i).simulated.at(j) << "->";
            } else{
                outputFile << endl << endl;
            }
        }

        outputFile << "Total number of cylinder movements: " << allresult.at(i).cylinderCount  << " cylinders\n";
    }

    outputFile.close();

}