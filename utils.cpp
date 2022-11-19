
#include "header.h"

// order: N,S,W,E
int neightboursRow[4] = {-1, 1, 0, 0};
int neightboursCol[4] = {0, 0, -1, 1};

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

// Find lowest neighbour(at least lower than current point) and return
// their position through res.
int find_lowest_neighbour(const vector<vector<int>>& map,
                          vector<pair<int, int>>& res, int row, int col,
                          const Arguments& args) {
  int curHeight = map[row][col];
  int x, y;

  // iterate neighbours to find the lowest one
  int minH = INT32_MAX;
  for (int i = 0; i < 4; i++) {
    x = row + neightboursRow[i];
    y = col + neightboursCol[i];
    if (x >= args.dimension || x < 0 || y >= args.dimension || y < 0) {
      continue;
    }
    if(map[x][y] < curHeight && map[x][y] < minH){
      minH = map[x][y];
    }
  }
  
  // not found
  if(minH == INT32_MAX)
    return 0;
  
  // return the lowest neighbour's position
  for (int i = 0; i < 4; i++) {
    x = row + neightboursRow[i];
    y = col + neightboursCol[i];
    if (x >= args.dimension || x < 0 || y >= args.dimension || y < 0) {
      continue;
    }
    if(map[x][y] == minH){
      res.push_back(make_pair(x,y));
    }
  }

  return 0;
}

bool isAllAbsorbed(const vector<vector<float>>& curRainDrops,
                   const Arguments& args) {
  int n = args.dimension;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (abs(curRainDrops[i][j] - 0) > 1e-6) return false;
    }
  }

  return true;
}

void showResult(const vector<vector<float>>& absorbedRainDrop) {
  int n = absorbedRainDrop.size();

  printf(
      "The following grid shows the number of raindrops absorbed at each "
      "point:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%f   ", absorbedRainDrop[i][j]);
    }
    printf("\n");
  }
}