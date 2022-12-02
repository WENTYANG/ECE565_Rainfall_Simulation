#ifndef _HEADER_H
#define _HEADER_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <atomic>
#include <condition_variable>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <chrono>

using namespace std;

// #define PROFILE
// #define PARALLEL
// #define SEQUENTIAL

// #define SERIAL_NEIGHBOUR
// #define SERIAL_ADDDROP
// #define SERIAL_ABSORB
// #define SERIAL_CALC_TRICKLE

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

// global variables defined in util.cpp
extern int neightboursRow[4];
extern int neightboursCol[4];

// general function
int read_args(int argc, char* argv[], Arguments* args);
int check_args(const Arguments* args);
int read_map(vector<vector<int>>& map, const string& fileName, int n);
int find_lowest_neighbour(const vector<vector<int>>& map,
                          vector<pair<int, int>>& res,
                          int row,
                          int col,
                          const Arguments& args);
bool is_all_absorbed(const vector<vector<float>>& curRainDrops,
                     const Arguments& args);
void show_results(const vector<vector<float>>& absorbedRainDrop);

// timerlib
#ifdef PROFILE
extern "C" {
void Timer_Start(char*);
void Timer_Stop(char*);
void Timer_Print();
}
#endif

// parallel sections
#ifdef PARALLEL
extern std::mutex mtx;
extern std::condition_variable cv;
extern atomic<int> done;

void find_lowest_neighbour_for_thread(
    const vector<vector<int>>& map,
    int startIndex,
    int pointNum,
    const Arguments& args,
    vector<vector<pair<int, int>>>& lowestNeighbours);

void add_drop_for_thread(vector<vector<float>>& curRainDrops,
                         const int startIndex,
                         const int numPoints,
                         const int threadId,
                         const Arguments& args);

#ifdef SERIAL_CALC_TRICKLE
typedef vector<TrickleInfo>& syncVecType;
#endif
#ifndef SERIAL_CALC_TRICKLE
    typedef vector<TrickleInfo>*& syncVecType;
#endif

#ifndef SERIAL_CALC_TRICKLE
void absorb_and_calc_trickle_for_thread(
    vector<vector<float>>& curRainDrops,
    vector<vector<float>>& absorbedRainDrop,
    const vector<vector<pair<int, int>>>& lowestNeighbours,
    syncVecType trickleDrops,
    const int threadId,
    const int startIndex,
    const int numPoints,
    const Arguments& args);

void calc_trickle_for_single_point(
    vector<vector<float>>& curRainDrops,
    const vector<vector<pair<int, int>>>& lowestNeighbours,
    syncVecType trickleDrops,
    const int threadId,
    int row,
    int col,
    const Arguments& args);
#endif

void absorb_for_single_point(vector<vector<float>>& curRainDrops,
                             vector<vector<float>>& absorbedRainDrop,
                             const float absRate,
                             const int row,
                             const int col);

void add_drop_for_single_point(vector<vector<float>>& curRainDrops,
                               int row,
                               int col);
#endif

#endif