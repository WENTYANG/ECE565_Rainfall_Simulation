
#include "header.h"

int main(int argc, char* argv[]) {
  // read input parameters.
  Arguments args;
  read_args(argc, argv, &args);
  check_args_seq(&args);

  // read elevation_file into array
  vector<vector<int>> map(args.dimension, vector<int>(args.dimension, 0));
  read_map(map, args.fileName, args.dimension);
  
  // simulation
  
}
