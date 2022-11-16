#ifndef _HEADER_H
#define _HEADER_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <string>
#include <vector>

using namespace std;

typedef struct Arguments {
  int nThreads;
  int timeStep;
  float absorptionRate;
  int dimension;
  string fileName;
} Arguments;

int read_args(int argc, char* argv[], Arguments* args);
int check_args_seq(const Arguments* args);
int read_map(vector<vector<int>>& map, const string& fileName, int n);

#endif