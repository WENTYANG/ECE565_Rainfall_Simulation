#ifndef _HEADER_H
#define _HEADER_H

using namespace std;

typedef struct Arguments {
  int nThreads;
  int timeStep;
  float absorptionRate;
  int dimension;
  string fileName;
}Arguments;

int read_args(int argc, char* argv[], Arguments* args);
int check_args_seq(const Arguments* args);

#endif