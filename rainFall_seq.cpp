
#include "header.h"
#define PROFILE

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
#ifdef PROFILE
  Timer_Start("Read map");
#endif
  vector<vector<int>> map(args.dimension, vector<int>(args.dimension, 0));
  read_map(map, args.fileName, args.dimension);
#ifdef PROFILE
  Timer_Stop("Read map");
#endif

  // simulation
  vector<vector<float>> absorbedRainDrop(args.dimension,
                                         vector<float>(args.dimension, 0));
  vector<vector<float>> curRainDrops(args.dimension,
                                     vector<float>(args.dimension, 0));
  int totalTimeStep = 0;
  clock_t start, end;
  start = clock();
  rainSimulation(map, absorbedRainDrop, curRainDrops, args, totalTimeStep);
  end = clock();

  // output simulation results
  double runtime = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Rainfall simulation completed in %d time steps\n", totalTimeStep);
  printf("Runtime = %f seconds\n", runtime);
  showResult(absorbedRainDrop);
#ifdef PROFILE
  Timer_Print();
#endif
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
#ifdef PROFILE
        Timer_Start("Add rain drops");
#endif
        // rain, add new drop to each point
        if (totalTimeStep <= args.timeStep) {
          curRainDrops[row][col]++;
        }
#ifdef PROFILE
        Timer_Stop("Add rain drops");
#endif

#ifdef PROFILE
        Timer_Start("Drops absorbed");
#endif
        // drop get abosrbed (//TODO: abstract this section?)
        if (curRainDrops[row][col] > 0) {
          float absRate = args.absorptionRate;
          if (curRainDrops[row][col] <= args.absorptionRate) {
            absRate = curRainDrops[row][col];
          }
          curRainDrops[row][col] -= absRate;
          absorbedRainDrop[row][col] += absRate;
        }
#ifdef PROFILE
        Timer_Stop("Drops absorbed");
#endif

        // decide trickle amount
        if (curRainDrops[row][col] < 0 || abs(curRainDrops[row][col]) < 1e-6) {
          continue;
        }
        trickleAmount =
            curRainDrops[row][col] > 1 ? 1.0 : curRainDrops[row][col];

#ifdef PROFILE
        Timer_Start("Record trickle direction");
#endif
        // decide trickle direction and record it
        lowestNeighbours.clear();
        find_lowest_neighbour(map, lowestNeighbours, row, col, args);
        for (auto& point : lowestNeighbours) {
          int r = point.first;
          int c = point.second;
          trickleDrops.push_back(
              TrickleInfo(trickleAmount / lowestNeighbours.size(), r, c));
        }
#ifdef PROFILE
        Timer_Stop("Record trickle direction");
#endif

#ifdef PROFILE
        Timer_Start("Reduce trickle drops");
#endif
        // drops are reduced from current point for trickling
        if (!lowestNeighbours.empty()) {
          curRainDrops[row][col] -= trickleAmount;
        }
#ifdef PROFILE
        Timer_Stop("Reduce trickle drops");
#endif
      }
    }

    // Add trickle drops to each point
#ifdef PROFILE
    Timer_Start("Add trickle drops");
#endif
    for (auto& it : trickleDrops) {
      curRainDrops[it.r][it.c] += it.amount;
    }
#ifdef PROFILE
    Timer_Stop("Add trickle drops");
#endif

    // Simulation ends
    if (totalTimeStep > args.timeStep && isAllAbsorbed(curRainDrops, args)) {
      break;
    }
  }
}
