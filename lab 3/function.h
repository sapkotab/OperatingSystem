//
// Created by Bhuwan on 11/22/17.
//

#ifndef LAB3_FUNCTION_H
#define LAB3_FUNCTION_H

#include <string>
#include "page.h"
#include <fstream>
using namespace std;

vector<page> getSequence(string);

void printAndOutput(vector <result>, string, int);

bool pageExist(vector<page>,page);

int pageIndex(vector<page>, page p);

int findReplacementIndex(int, vector <page>, vector <page>);

void simulate(int, vector<result> &, string);

#endif //LAB3_FUNCTION_H
