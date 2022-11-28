
#include "header.h"

// synchronized variable for threads
std::mutex mtx;
std::condition_variable cv;
atomic<int> done(0);

int main(int argc, char* argv[]) {
  // read input parameters.
  Arguments args;
  read_args(argc, argv, &args);
  check_args(&args);

  // read elevation_file into array
  vector<vector<int>> map(args.dimension, vector<int>(args.dimension, 0));
  read_map(map, args.fileName, args.dimension);

  // calculate lowestNeighbours(parallel)
  unordered_map<int, vector<pair<int, int>>>
      lowestNeighbours;  // record the lowest neighbours of each point
  int totalPoints = args.dimension * args.dimension;
  int numPerTask = totalPoints / args.nThreads;
  for (int i = 0; i < args.nThreads; i++) {
    int startIndex = i * numPerTask;
    thread t(find_lowest_neighbour_for_thread, std::ref(map), startIndex,
             numPerTask, std::ref(args), std::ref(lowestNeighbours));
    t.detach();
  }
  // add barrier manually
  {
    std::unique_lock<std::mutex> lk(mtx);  // mtx locks here
    while (done != args.nThreads) {
      cv.wait(lk);
    }
    // unlock automatically when lk destructs.
  }




  // print out lowestNeighbours
  for(auto item:lowestNeighbours){
    printf("[%d][%d]: ", item.first/args.dimension, item.first%args.dimension);
    for(auto point:item.second){
      printf("(%d, %d) ", point.first,point.second);
    }
    printf("\n");
  }

}