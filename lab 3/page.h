//
// Created by Bhuwan on 11/22/17.
//

#ifndef LAB3_PAGE_H
#define LAB3_PAGE_H

#include <vector>

class page {
    int __pageNumber;
    int __arrivalTimeInMem;
    int __lastUsedTime;
    int __numOfTimeUsed;
public:
    page();

    // setter
    void setPageNo(int pageNumber);
    void setArrivalTime(int arrivalTime);
    void setLastTimeUsed(int lastTimeUsed);
    void incNumOfTimeUsed();
    void setNumOfTimeUsed(int);

    //getter
    int getPageNo();
    int getArrivalTime();
    int getLastTimeUsed();
    int getNumOfTimeUsed();

};

class result {
    int __numberOfFault;
    std::vector<float> rates;
public:
    result();

    void setNoOfFault(int NoOfFault);
    void pushRate(float rate);

    int getNumOfFault();
    std::vector<float> getAllFaultRate();
};

#endif //LAB3_PAGE_H
