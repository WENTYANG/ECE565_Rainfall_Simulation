
#include "header.h"

int read_args(int argc, char* argv[], Arguments* args) {
  if (argc != 6) {
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

int check_args_seq(const Arguments* args) {
  assert(args->nThreads >= 0);
  assert(args->timeStep > 0);
  assert(args->absorptionRate > 0);
  assert(args->dimension > 0);
  assert(args->fileName.empty() == false);

  if (args->nThreads != 0) {
    printf("nThreads arg will be ignored.");
  }
  return 0;
}

int read_map(vector<vector<int>>& map, const string& fileName, int n) {
  ifstream file(fileName);
  if (file.is_open() == false) {
    printf("fail to open file\n");
    exit(1);
  }

  string buf;
  int count = 0;
  int row = 0, col = 0;
  while (file >> buf) {
    int num = stoi(buf);
    map[row][col] = num;
    count++;
    col++;
    if (col == n) {
      col = 0;
      row++;
    }
  }
  assert(count = n * n);
  file.close();
  return 0;
}