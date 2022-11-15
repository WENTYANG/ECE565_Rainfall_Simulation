#include <assert.h>
#include <stdio.h>

#include "header.h"

int main(int argc, char* argv[]) {
  // read input parameters.
  Arguments args;
  read_args(argc, argv, &args);
  check_args_seq(&args);

  // TODO: read elevation_file into array
  

  // TODO: simulation
}
