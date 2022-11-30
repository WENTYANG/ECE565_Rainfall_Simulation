#include "header.h"
#define PUSHTIMES 10000
#define nThreads 8

std::mutex mtx;
std::condition_variable cv;
atomic<int> done(0);

void pushToVector(vector<vector<int>>& testVec, int id){
  for (int value = 0; value < PUSHTIMES; value++){
    testVec[id].push_back(value);
  }
  done++;
  cv.notify_all();
}

void barrier() {
  std::unique_lock<std::mutex> lk(mtx);  // mtx locks here
  while (done.load() != nThreads) {
    cv.wait(lk);
  }
  // unlock automatically when lk destructs.
}

int main(int argc, char* argv[]){
  vector<vector<int>> testVec(nThreads, vector<int>());
  for (int id = 0; id < nThreads; id++) {
    thread t(pushToVector, std::ref(testVec), id);
    t.detach();
  }
  barrier();

  for (auto& subVec : testVec){
    assert(subVec.size() == PUSHTIMES);
    for(auto& elem:subVec){
      cout << elem << " ";
    }
    cout << endl;
  }
  return EXIT_SUCCESS;
}