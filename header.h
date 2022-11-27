#ifndef _HEADER_H
#define _HEADER_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct Arguments {
  int nThreads;
  int timeStep;  // rain time
  float absorptionRate;
  int dimension;  // size of the map
  string fileName;
} Arguments;

typedef struct TrickleInfo {
  float amount;
  int r;  // target row
  int c;  // target col

  TrickleInfo(float amt, int row, int col) {
    this->amount = amt;
    this->r = row;
    this->c = col;
  }
} TrickleInfo;


extern int neightboursRow[4];
extern int neightboursCol[4];

int read_args(int argc, char* argv[], Arguments* args);
int check_args_seq(const Arguments* args);
int read_map(vector<vector<int>>& map, const string& fileName, int n);
int find_lowest_neighbour(const vector<vector<int>>& map,
                          vector<pair<int, int>>& res, int row, int col,
                          const Arguments& args);
bool isAllAbsorbed(const vector<vector<float>>& curRainDrops,
                   const Arguments& args);
void showResult(const vector<vector<float>>& absorbedRainDrop);

// timerlib
extern "C" {
void Timer_Start(char*);
void Timer_Stop(char*);
void Timer_Print();
}

// #define PROFILE


#endif