
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

int check_args(const Arguments* args) {
  assert(args->nThreads >= 0);
  assert(args->timeStep > 0);
  assert(args->absorptionRate > 0);
  assert(args->dimension > 0);
  assert(args->fileName.empty() == false);

#ifdef SEQUENTIAL
  if (args->nThreads == 0) {
    printf("nThreads arg will be ignored.");
  }
#endif
#ifdef PARALLEL
  if (args->nThreads == 0) {
    printf("nThreads arg should not be 0.");
  }
#endif
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
    if (map[x][y] < curHeight && map[x][y] < minH) {
      minH = map[x][y];
    }
  }

  // not found
  if (minH == INT32_MAX) return 0;

  // return the lowest neighbour's position
  for (int i = 0; i < 4; i++) {
    x = row + neightboursRow[i];
    y = col + neightboursCol[i];
    if (x >= args.dimension || x < 0 || y >= args.dimension || y < 0) {
      continue;
    }
    if (map[x][y] == minH) {
      res.push_back(make_pair(x, y));
    }
  }

  return 0;
}

bool is_all_absorbed(const vector<vector<float>>& curRainDrops,
                     const Arguments& args) {
#ifdef PROFILE
  Timer_Start("is_all_absorbed");
#endif
  int n = args.dimension;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (abs(curRainDrops[i][j] - 0) > 1e-6) {
#ifdef PROFILE
        Timer_Stop("is_all_absorbed");
#endif
        return false;
      }
    }
  }
#ifdef PROFILE
  Timer_Stop("is_all_absorbed");
#endif
  return true;
}

void show_results(const vector<vector<float>>& absorbedRainDrop) {
  int n = absorbedRainDrop.size();

  printf(
      "The following grid shows the number of raindrops absorbed at each "
      "point:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%-5g   ", absorbedRainDrop[i][j]);
    }
    printf("\n");
  }
}

/*------------------------PARALLEL FUNTIONS----------------------*/
#ifdef PARALLEL
void find_lowest_neighbour_for_thread(
    const vector<vector<int>>& map, int startIndex, int pointNum,
    const Arguments& args,
    unordered_map<int, vector<pair<int, int>>>& lowestNeighbours) {
  int row, col;
  vector<pair<int, int>> res;
  for (int p = startIndex; p < pointNum; p++) {
    row = p / args.dimension;
    col = p % args.dimension;
    res.clear();
    find_lowest_neighbour(map, res, row, col, args);
    if (!res.empty()) {
      mtx.lock();
      lowestNeighbours[p] = res;
      mtx.unlock();
    }
  }
  done++;
  cv.notify_all();
}
#endif