
#include "header.h"

// synchronized variable for threads
std::mutex mtx;
std::condition_variable cv;
atomic<int> done(0);

void rain_simulation(
    const vector<vector<int>>& map,
    unordered_map<int, vector<pair<int, int>>>& lowestNeighbours,
    vector<vector<float>>& absorbedRainDrop, const Arguments& args,
    int& totalTimeStep);

int main(int argc, char* argv[]) {
  // read input parameters.
  Arguments args;
  read_args(argc, argv, &args);
  check_args(&args);

  // read elevation_file into array
  vector<vector<int>> map(args.dimension, vector<int>(args.dimension, 0));
  read_map(map, args.fileName, args.dimension);

  clock_t start, end;
  start = clock();

  // calculate lowestNeighbours in parallel
  unordered_map<int, vector<pair<int, int>>>
      lowestNeighbours;  // record the lowest neighbours of each point
  int totalPoints = args.dimension * args.dimension;
  int numPerTask = totalPoints / args.nThreads;
  done.store(0);
  for (int i = 0; i < args.nThreads; i++) {
    int startIndex = i * numPerTask;
    thread t(find_lowest_neighbour_for_thread, std::ref(map), startIndex,
             numPerTask, std::ref(args), std::ref(lowestNeighbours));
    t.detach();
  }
  // add barrier manually
  {
    std::unique_lock<std::mutex> lk(mtx);  // mtx locks here
    while (done.load() != args.nThreads) {
      cv.wait(lk);
    }
    // unlock automatically when lk destructs.
  }

  // simulation
  vector<vector<float>> absorbedRainDrop(args.dimension,
                                         vector<float>(args.dimension, 0));
  int totalTimeStep;
  rain_simulation(map, lowestNeighbours, absorbedRainDrop, args, totalTimeStep);

  // output simulation results
  end = clock();
  double runtime = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Rainfall simulation completed in %d time steps\n", totalTimeStep);
  printf("Runtime = %f seconds\n", runtime);
  show_results(absorbedRainDrop);
}

void rain_simulation(
    const vector<vector<int>>& map,
    unordered_map<int, vector<pair<int, int>>>& lowestNeighbours,
    vector<vector<float>>& absorbedRainDrop, const Arguments& args,
    int& totalTimeStep) {
  vector<vector<float>> curRainDrops(args.dimension,
                                     vector<float>(args.dimension, 0));
  while (1) {
    totalTimeStep++;
    if(totalTimeStep <= args.timeStep){
      //TODO: rain drop parallel
    }

    // TODO: rain drop absorbs and calculate trickleAmount in parallel

    // TODO: Add trickle amount

    // TODO: test ending condition
  }
}






