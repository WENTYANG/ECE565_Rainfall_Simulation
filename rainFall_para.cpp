
#include "header.h"

// synchronized variable for threads
std::mutex mtx;
std::condition_variable cv;
atomic<int> done(0);
int totalPoints;
int numPerTask;

void barrier(const int nThreads) {
  std::unique_lock<std::mutex> lk(mtx);  // mtx locks here
  while (done.load() != nThreads) {
    cv.wait(lk);
  }
  // unlock automatically when lk destructs.
}

void rain_simulation(
    const vector<vector<int>>& map,
    vector<vector<pair<int, int>>>& lowestNeighbours,
    vector<vector<float>>& absorbedRainDrop,
    const Arguments& args,
    int& totalTimeStep);

int main(int argc, char* argv[]) {
  // read input parameters.
  Arguments args;
  read_args(argc, argv, &args);
  check_args(&args);

  // read elevation_file into array
  vector<vector<int>> map(args.dimension, vector<int>(args.dimension, 0));
  read_map(map, args.fileName, args.dimension);

  auto start = chrono::system_clock::now();

  // calculate lowestNeighbours in parallel
  vector<vector<pair<int, int>>> lowestNeighbours;  // record the lowest neighbours of each point
  totalPoints = args.dimension * args.dimension;
  numPerTask = totalPoints / args.nThreads;
  lowestNeighbours.resize(totalPoints);

  done.store(0);
  for (int i = 0; i < args.nThreads; i++) {
    int startIndex = i * numPerTask;
    thread t(find_lowest_neighbour_for_thread, std::ref(map), startIndex,
             numPerTask, std::ref(args), std::ref(lowestNeighbours));
    t.detach();
  }
  barrier(args.nThreads);

  // simulation
  vector<vector<float>> absorbedRainDrop(args.dimension,
                                         vector<float>(args.dimension, 0));
  int totalTimeStep = 0;
  rain_simulation(map, lowestNeighbours, absorbedRainDrop, args, totalTimeStep);

  // output simulation results
  auto end = chrono::system_clock::now();
   std::chrono::duration<double> runtime = end - start;
   cout << "Rainfall simulation completed in " << totalTimeStep << " time steps"
        << endl;
   cout << "Runtime = " << runtime.count() << " seconds" << endl;
   show_results(absorbedRainDrop);
}

void rain_simulation(
    const vector<vector<int>>& map,
    vector<vector<pair<int, int>>>& lowestNeighbours,
    vector<vector<float>>& absorbedRainDrop,
    const Arguments& args,
    int& totalTimeStep) {
  vector<vector<float>> curRainDrops(args.dimension,
                                     vector<float>(args.dimension, 0));
  while (1) {
    totalTimeStep++;

    vector<TrickleInfo>* trickleDrops[args.nThreads];
    for (int i = 0; i < args.nThreads; i++){
      trickleDrops[i] = new vector<TrickleInfo>();
      // trickleDrops[i]->reserve(numPerTask*sizeof(TrickleInfo)*4);
    }
    if (totalTimeStep <= args.timeStep) {

      done.store(0);
      for (int i = 0; i < args.nThreads; i++) {
        int startIndex = i * numPerTask;
        thread t(add_drop_for_thread, std::ref(curRainDrops), startIndex,
                 numPerTask, i, std::ref(args));
        t.detach();
      }
      barrier(args.nThreads);
    }

    done.store(0);
    for (int i = 0; i < args.nThreads; i++) {
      int startIndex = i * numPerTask;
      thread t(absorb_and_calc_trickle_for_thread, std::ref(curRainDrops),
               std::ref(absorbedRainDrop), std::ref(lowestNeighbours),
               std::ref(trickleDrops[i]), i, startIndex, numPerTask,
               std::ref(args));

      t.detach();
    }
    barrier(args.nThreads);
    for (int i = 0; i < args.nThreads; i++) {
      for (auto& trickle : *trickleDrops[i]) {
        curRainDrops[trickle.r][trickle.c] += trickle.amount;
      }
      delete trickleDrops[i];
    }

    // Simulation ending condition
    if (totalTimeStep > args.timeStep && is_all_absorbed(curRainDrops, args)) {
      break;
    }
  }
}
