
#include "header.h"

void rainSimulation(const vector<vector<int>>& map,
                    vector<vector<float>>& absorbedRainDrop,
                    vector<vector<float>>& curRainDrops, const Arguments& args,
                    int& totalTimeStep);

int main(int argc, char* argv[]) {
  // read input parameters.
  Arguments args;
  read_args(argc, argv, &args);
  check_args_seq(&args);

  // read elevation_file into array
  vector<vector<int>> map(args.dimension, vector<int>(args.dimension, 0));
  read_map(map, args.fileName, args.dimension);

  // simulation
  vector<vector<float>> absorbedRainDrop(args.dimension,
                                         vector<float>(args.dimension, 0));
  vector<vector<float>> curRainDrops(args.dimension,
                                     vector<float>(args.dimension, 0));
  int totalTimeStep = 0;
  time_t start, end;
  time(&start);
  rainSimulation(map, absorbedRainDrop, curRainDrops, args, totalTimeStep);
  time(&end);

  // output simulation results
  double runtime = double(end - start);
  printf("Rainfall simulation completed in %d time steps\n", totalTimeStep);
  printf("Runtime = %f seconds\n", runtime);
  showResult(absorbedRainDrop);
}

void rainSimulation(const vector<vector<int>>& map,
                    vector<vector<float>>& absorbedRainDrop,
                    vector<vector<float>>& curRainDrops, const Arguments& args,
                    int& totalTimeStep) {
  vector<TrickleInfo> trickleDrops;
  vector<pair<int, int>> lowestNeighbours;
  float trickleAmount = 0;

  while (1) {
    // printf("timestep:%d\n", totalTimeStep);
    totalTimeStep++;
    trickleDrops.clear();

    for (int row = 0; row < args.dimension; row++) {
      for (int col = 0; col < args.dimension; col++) {
        // rain, add new drop
        if (totalTimeStep <= args.timeStep) {
          curRainDrops[row][col]++;
        }

        // drop get abosrbed (//TODO: abstract this section?)
        if (curRainDrops[row][col] > 0) {
          float absRate = args.absorptionRate;
          if (curRainDrops[row][col] <= args.absorptionRate) {
            absRate = curRainDrops[row][col];
          }
          curRainDrops[row][col] -= absRate;
          absorbedRainDrop[row][col] += absRate;
        }

        // decide trickle amount and direction
        if (curRainDrops[row][col] < 0 || abs(curRainDrops[row][col]) < 1e-6) {
          continue;
        }
        trickleAmount =
            curRainDrops[row][col] > 1 ? 1.0 : curRainDrops[row][col];

        // decide trickle direction and record
        lowestNeighbours.clear();
        find_lowest_neighbour(map, lowestNeighbours, row, col, args);
        for (auto& point : lowestNeighbours) {
          int r = point.first;
          int c = point.second;
          trickleDrops.push_back(
              TrickleInfo(trickleAmount / lowestNeighbours.size(), r, c));
        }

        // drops are reduced from current point for trickling
        if (!lowestNeighbours.empty()) {
          curRainDrops[row][col] -= trickleAmount;
        }
      }
    }

    // Add trickle drops to each point
    for (auto& it : trickleDrops) {
      curRainDrops[it.r][it.c] += it.amount;
    }

    if (totalTimeStep > args.timeStep && isAllAbsorbed(curRainDrops, args)) {
      break;
    }
  }
}
