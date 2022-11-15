#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include "header.h"

int read_args(int argc, char* argv[], Arguments* args){
    if(argc != 6){
        printf("correct Usage: ./rainfall_seq <P> <M> <A> <N> <elevation_file>\n");
        exit(1);
    }
    args->nThreads = atoi(argv[1]);
    args->timeStep = atoi(argv[2]);
    args->absorptionRate = atof(argv[3]);
    args->dimension = atoi(argv[4]);
    args->fileName = argv[5];

    return 0;
}

int check_args_seq(const Arguments* args){
    assert(args->nThreads > 0);
    assert(args->timeStep > 0);
    assert(args->absorptionRate > 0);
    assert(args->dimension > 0);
    assert(args->fileName.empty() == false);

    if(args->nThreads != 0){
        printf("nThreads arg will be ignored.");
    }
    return 0;
}
