//
// Created by Bhuwan on 10/26/17.
//

#include "process.h"

#ifndef LAB2_READYQUEUE_H
#define LAB2_READYQUEUE_H

const int MAX_PROCESS = 101;

class readyQueue {
public:
    process * myQueue[MAX_PROCESS];  // upto 100 process requirement.
    int size;
                        // we track size by size
    readyQueue();       // just set queue process to null pointer
    void push(process*);       // push in the back of queue
    process* pop();            // pop from front

    void sortByTime();          // sort by lowest remaining time
    void sortByPriority();      // sort by priority
    bool procExist(process&);    // check weather the process is in queue
};


#endif //LAB2_READYQUEUE_H
