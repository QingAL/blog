#include <chrono>
#include <coroutine>
#include <iostream>
#include <thread>

struct CoResult {
  struct promise_type {
    CoResult get_return_object() {
      printf("----- in CoResult get_return_object -----\n");
      return {};
    }
    std::suspend_never initial_suspend() {
      printf("----- in CoResult initial_suspend -----\n");
      return {};
    }
    std::suspend_always final_suspend() noexcept {
      printf("----- in CoResult final_suspend -----\n");
      return {};
    }
    void return_void() { printf("----- in CoResult return_void -----\n"); }
    void unhandled_exception() {
      printf("----- in CoResult unhandled_exception -----\n");
    }
  };
};

struct CoAwaiter {

  CoAwaiter(int waiterTime) { _waiterTime = waiterTime; }

  bool await_ready() {
    printf("----- in CoAwaiter await_ready -----\n");
    return false;
  }

  void await_suspend(std::coroutine_handle<> handler) {
    printf("----- in CoAwaiter await_suspend -----\n");
    int time = this->_waiterTime;
    std::thread([handler, time]() {
      std::this_thread::sleep_for(std::chrono::seconds(time));
      handler.resume();
    }).detach();
  }
  void await_resume() noexcept {
    printf("----- in CoAwaiter await_resume -----\n");
  }

private:
  int _waiterTime;
};

CoResult co_demo(int time) {
  printf("begin new CoAwaiter\n");
  co_await CoAwaiter{time};
  printf("after new CoAwaiter\n");
}

int main() {
  printf("begin co_demo function\n");
  co_demo(10);
  printf("after co_demo function\n");
  getchar();
  return 0;
}
