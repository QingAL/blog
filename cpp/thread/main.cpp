#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
int indexNum = 10;

void WorkThread() {
  std::cout << "call WorkThread...\n";

  // wait lock
  mtx.lock();
  // simulated work
  for (int i = 0; i < 10; ++i) {
    --indexNum;
    std::cout << "WorkThread do work index:" << i << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  mtx.unlock();

  std::cout << "WorkThread finished\n";
}

void ConditionThread() {
  std::cout << "call ConditionThread...\n";

  // wait for cv
  std::unique_lock lock(mtx);
  cv.wait(lock, [] { return indexNum == 0; });
  for (int i = 0; i < 10; ++i) {
    std::cout << "ConditionThread do work index: " << i << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

int main() {
  mtx.lock();

  // work thread will be blocked while mtx lock
  std::thread th(WorkThread);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  mtx.unlock();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // lock first block ConditionThread wait for condition_variable
  // unique_lock out of scope unlock auto, or call unlock()
  std::unique_lock lock(mtx);
  std::thread cth(ConditionThread);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  lock.unlock();
  cv.notify_one();

  cth.join();
  th.join();
}