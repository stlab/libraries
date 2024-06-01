#include <atomic>
#include <cstdio>
#include <thread>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
  atomic_bool done { false };
  {
    auto x = async(default_executor,
      [] {
        this_thread::sleep_for(chrono::seconds(42));
        return 42; }
    );

    auto y = x.then([&_done = done](int x) {
      printf("Result %d \n", x);
      _done = true;
    });

    y.detach();
  } // The futures x and y are destructed here, but not canceled,
    // because the continuation was detached.

  // Waiting just for illustration purpose
  while (!done) { this_thread::sleep_for(chrono::milliseconds(1)); }

  pre_exit();
}

/*
  Result:
  Waiting for 42s...
    Result 42
*/
