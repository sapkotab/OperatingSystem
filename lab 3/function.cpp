//
// Created by Bhuwan on 11/22/17.
//

#include <iostream>
#include <iomanip>
#include "function.h"


vector<page> getSequence(string s) {
    vector<page> p;
    string line;
    ifstream inputFile(s);

    while(getline(inputFile,line)){
        page myPage;
        myPage.setPageNo(atoi(line.c_str()));
        p.push_back(myPage);
    }
    inputFile.close();
    return p;
}

void printAndOutput(vector<result> resultTable, string output, int frameSize) {

    string algorithmType[] = {"FIFO","LRU","LFU","Optimal"};
    cout << "======================================================================\n";
    cout << "       Page Replacement Algorithm Simulation (frame size = " <<  frameSize << ")\n";
    cout << "======================================================================\n";
    cout << "                                           Page-fault-rates\n";
    cout << "Algorithm Total-page-faults  2000     4000     6000     8000     10000\n";
    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < resultTable.size(); ++i) {
        cout << setw(17) << left << algorithmType[i] << setprecision(3) << fixed
             << setw(12) << left << resultTable.at(i).getNumOfFault();
        for (int j = 0; j < resultTable.at(i).getAllFaultRate().size() ; ++j) {
            cout << setw(9) << left << resultTable.at(i).getAllFaultRate().at(j);
        }
        cout << endl;
    }

    ofstream outputFile(output);
    outputFile << "======================================================================\n";
    outputFile << "       Page Replacement Algorithm Simulation (frame size = " <<  frameSize << ")\n";
    outputFile << "======================================================================\n";
    outputFile << "                                           Page fault rates\n";
    outputFile << "Algorithm Total-page-faults  2000     4000     6000     8000     10000\n";
    outputFile << "----------------------------------------------------------------------\n";
    for (int i = 0; i < resultTable.size(); ++i) {
        outputFile << setw(17) << left << algorithmType[i] << setprecision(3) << fixed
                   << setw(12) << left << resultTable.at(i).getNumOfFault();
        for (int j = 0; j < resultTable.at(i).getAllFaultRate().size() ; ++j) {
            outputFile << setw(9) << left << resultTable.at(i).getAllFaultRate().at(j);
        }
        outputFile << endl;
    }
    outputFile.close();

}

// look for page exit in page table.

bool pageExist(vector<page> ps, page p) {
    bool exist = false;
    for (auto &i : ps) {
        if(i.getPageNo() == p.getPageNo()){
            exist = true;
            break;
        }
    }
    return exist;
}

int pageIndex(vector<page> ps, page p) {
    int index;
    for (int i = 0; i < ps.size() ; ++i) {
        if(ps.at(i).getPageNo() == p.getPageNo()){
            index = i;
            break;
        }
    }
    return index;
}

int findReplacementIndex(int algorithmType, vector<page> frame, vector<page> listOfSequence) {
    int replacingIndex;

    switch (algorithmType){
        case 0: {   // fifo
            page old;

            // making it youngest among page
            old.setArrivalTime(32767);              // largest number possible for int // we expecting 10000 entry.
            for (int i = 0; i < frame.size(); ++i) {
                if(frame.at(i).getArrivalTime() < old.getArrivalTime()){
                    old = frame.at(i);
                    replacingIndex = i;
                }
            }
        }
            break;
        case 1:{ // LRU
            page lruPage;
            lruPage.setLastTimeUsed(32767);
            for (int i = 0; i < frame.size(); ++i) {
                if(frame.at(i).getLastTimeUsed() < lruPage.getLastTimeUsed()){
                    lruPage = frame.at(i);
                    replacingIndex = i;
                }
            }
        }
            break;
        case 2:{ // LFU
            page lfuPage;
            lfuPage.setNumOfTimeUsed(32767);
            lfuPage.setArrivalTime(32767);
            for (int i = 0; i < frame.size(); ++i) {
                if(frame.at(i).getNumOfTimeUsed() < lfuPage.getNumOfTimeUsed()){
                    lfuPage = frame.at(i);
                    replacingIndex = i;
                } else if (frame.at(i).getNumOfTimeUsed() == lfuPage.getNumOfTimeUsed()){
                    if(frame.at(i).getArrivalTime() < lfuPage.getArrivalTime()){
                        lfuPage = frame.at(i);
                        replacingIndex = i;
                    }
                }
            }
        }
            break;
        case 3: { // optimal
            page optimal;
            int farIndex = -1;
            bool found;
            for (int i = 0; i < frame.size(); ++i) {

                found = false;
                for (int j = 0; j < listOfSequence.size() ; ++j) {
                    if(frame.at(i).getPageNo() == listOfSequence.at(j).getPageNo()){
                        found = true;
                        if (j > farIndex){
                            farIndex = j;
                            replacingIndex = i;
                        }
                        break;
                    }
                }
                if(!found){
                    replacingIndex = i;
                    break;
                }
            }
        }
            break;

        default:{
            cout << "invalid algorithm\n";
        }
    }

    return replacingIndex;
}

void simulate(int frameSize, vector<result> &resultList, string inputFile) {
    vector<page> requestSequence;
    vector<page> frame;
    int interval = 2000;

    // let us simulate for four different algorithm
    for (int algorithmType = 0; algorithmType < 4 ; ++algorithmType) {
        requestSequence = getSequence(inputFile);
        int sequenceSize = requestSequence.size();
        int pageFaultCount = 0;
        int time = 1;

        // each coming sequence also measure the time passed by simulation.
        if(algorithmType == 0)  {
            while (!requestSequence.empty()){ // i.e. FIFO

                // push to frame until it filled, sometime there are duplicates before we fill fram
                // we will skip them
                // every page come to memory will be deleted in requestsequence list
                if(frame.size() < frameSize){
                    if(!pageExist(frame, requestSequence.at(0))) {
                        frame.push_back(static_cast<page &&>(requestSequence.at(0)));
                        ++pageFaultCount;
                        frame.at(frame.size()-1).setArrivalTime(time);
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                } else{
                    // if page not exist do all stuff. replacing deleting.
                    if(!pageExist(frame, requestSequence.at(0))) {
                        int replacingIndex = findReplacementIndex(algorithmType, frame, requestSequence);
                        frame.at(replacingIndex) = requestSequence.at(0);
                        ++pageFaultCount;
                        frame.at(replacingIndex).setArrivalTime(time);
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                }
                if(time == interval+1 || time == 2*interval+1 || time == 3*interval+1 || time == 4*interval+1 || time == 5*interval+1){
                    resultList.at(algorithmType).pushRate((float)pageFaultCount/(float)(time-1));
                }
            }
            //pageFaultcount
            resultList.at(algorithmType).setNoOfFault(pageFaultCount);

            // clearing vector for next algorithm use.
            requestSequence.clear();
            frame.clear();
        }

        // each coming sequence also measure the time passed by simulation.
        if(algorithmType == 1)  {
            while (!requestSequence.empty()){ // i.e. LRU

                // push to frame until it filled, sometime there are dublicates before we fill fram
                // we will skip them
                // every page come to memory will be deleted in requestsequnce list
                if(frame.size() < frameSize){
                    if(!pageExist(frame, requestSequence.at(0))) {
                        frame.push_back(static_cast<page &&>(requestSequence.at(0)));
                        ++pageFaultCount;
                        frame.at(frame.size()-1).setArrivalTime(time);
                        frame.at(frame.size()-1).setLastTimeUsed(time);
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence after some change
                        int beingChanged = pageIndex(frame,requestSequence.at(0));
                        frame.at(beingChanged).setLastTimeUsed(time);
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                } else{
                    // if page does not exist do all stuff. replacing deleting.
                    if(!pageExist(frame, requestSequence.at(0))) {
                        int replacingIndex = findReplacementIndex(algorithmType, frame, requestSequence);
                        frame.at(replacingIndex) = requestSequence.at(0);
                        ++pageFaultCount;
                        frame.at(replacingIndex).setArrivalTime(time);
                        frame.at(replacingIndex).setLastTimeUsed(time);
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence after some change
                        int beingChanged = pageIndex(frame,requestSequence.at(0));
                        frame.at(beingChanged).setLastTimeUsed(time);
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                }
                if(time == interval+1 || time == 2*interval+1 || time == 3*interval+1 || time == 4*interval+1 || time == 5*interval+1){
                    resultList.at(algorithmType).pushRate((float)pageFaultCount/(float)(time-1));
                }
            }
            //pageFaultcount
            resultList.at(algorithmType).setNoOfFault(pageFaultCount);

            // clearing vector for next algorithm use.
            requestSequence.clear();
            frame.clear();
        }


        // each coming sequence also measure the time passed by simulation.
        if(algorithmType == 2)  {
            while (!requestSequence.empty()){ // i.e. LFU

                // push to frame until it filled, sometime there are duplicates before we fill frame
                // we will skip them
                // every page come to memory will be deleted in requestsequnce list
                if(frame.size() < frameSize){
                    if(!pageExist(frame, requestSequence.at(0))) {
                        frame.push_back(static_cast<page &&>(requestSequence.at(0)));
                        ++pageFaultCount;
                        frame.at(frame.size()-1).setArrivalTime(time);
                        frame.at(frame.size()-1).incNumOfTimeUsed();
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence after some change
                        int beingChanged = pageIndex(frame,requestSequence.at(0));
                        frame.at(beingChanged).incNumOfTimeUsed();
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                } else{
                    // if page does not exist do all stuff. replacing deleting.
                    if(!pageExist(frame, requestSequence.at(0))) {
                        int replacingIndex = findReplacementIndex(algorithmType, frame, requestSequence);
                        frame.at(replacingIndex) = requestSequence.at(0);
                        ++pageFaultCount;
                        frame.at(replacingIndex).setArrivalTime(time);
                        frame.at(replacingIndex).incNumOfTimeUsed();
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence after some change
                        int beingChanged = pageIndex(frame,requestSequence.at(0));
                        frame.at(beingChanged).incNumOfTimeUsed();
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                }
                if(time == interval+1 || time == 2*interval+1 || time == 3*interval+1 || time == 4*interval+1 || time == 5*interval+1){
                    resultList.at(algorithmType).pushRate((float)pageFaultCount/(float)(time-1));
                }
            }
            //pageFaultcount
            resultList.at(algorithmType).setNoOfFault(pageFaultCount);

            // clearing vector for next algorithm use.
            requestSequence.clear();
            frame.clear();
        }
        // each coming sequence also measure the time passed by simulation.
        if(algorithmType == 3)  {
            cout << "Finished FIFO, LRU and LFU.\n";
            cout << "Program is currently working on Optimal, It may take a while.\n";
            cout << "Please wait........\n";
            while (!requestSequence.empty()){ // i.e. optimal

                // push to frame until it filled, sometime there are duplicates before we fill frame
                // we will skip them
                // every page come to memory will be deleted in requestsequnce list
                if(frame.size() < frameSize){
                    if(!pageExist(frame, requestSequence.at(0))) {
                        frame.push_back(static_cast<page &&>(requestSequence.at(0)));
                        ++pageFaultCount;
                        frame.at(frame.size()-1).setArrivalTime(time);
                        frame.at(frame.size()-1).incNumOfTimeUsed();
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence after some change
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                } else{
                    // if page does not exist do all stuff. replacing deleting.
                    if(!pageExist(frame, requestSequence.at(0))) {
                        int replacingIndex = findReplacementIndex(algorithmType, frame, requestSequence);
                        frame.at(replacingIndex) = requestSequence.at(0);
                        ++pageFaultCount;
                        requestSequence.erase(requestSequence.begin());
                    } else{
                        // it already exist in frame so just delete from sequence after some change
                        requestSequence.erase(requestSequence.begin());
                    }
                    ++time;
                }
                if(time == interval+1 || time == 2*interval+1 || time == 3*interval+1 || time == 4*interval+1 || time == 5*interval+1){
                    resultList.at(algorithmType).pushRate((float)pageFaultCount/(float)(time-1));
                }
            }
            //pageFaultcount
            resultList.at(algorithmType).setNoOfFault(pageFaultCount);

            // clearing vector for next algorithm use.
            requestSequence.clear();
            frame.clear();
        }
    }
}


