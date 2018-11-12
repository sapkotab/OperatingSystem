//
// Created by Bhuwan on 11/22/17.
//

#include "page.h"

page::page() {
    __pageNumber = 0;
    __arrivalTimeInMem = 0;
    __lastUsedTime = -1;
    __numOfTimeUsed = 0;
}

void page::setPageNo(int pageNumber) {
    __pageNumber = pageNumber;
}

void page::setArrivalTime(int arrivalTime) {
    __arrivalTimeInMem = arrivalTime;
}

void page::setLastTimeUsed(int lastTimeUsed) {
    __lastUsedTime = lastTimeUsed;
}


int page::getPageNo() {
    return __pageNumber;
}

int page::getArrivalTime() {
    return __arrivalTimeInMem;
}

int page::getLastTimeUsed() {
    return __lastUsedTime;
}

int page::getNumOfTimeUsed() {
    return __numOfTimeUsed;
}

void page::incNumOfTimeUsed() {
    ++__numOfTimeUsed;
}

void page::setNumOfTimeUsed(int i) {
    __numOfTimeUsed = i;
}

result::result() {
    __numberOfFault = 0;
}

void result::setNoOfFault(int NoOfFault) {
    __numberOfFault = NoOfFault;
}

void result::pushRate(float rate) {
    rates.push_back(rate);
}

int result::getNumOfFault() {
    return __numberOfFault;
}

std::vector<float> result::getAllFaultRate() {
    return rates;
}
