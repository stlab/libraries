#include <atomic>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace stlab;

/*
  This process adds all values until a zero is passed as value.
  Then it will yield the result and start over again.
*/

struct adder
{
  int _sum = 0;
  process_state_scheduled _state = await_forever;

  void await(int x) {
    _sum += x;
    if (x == 0) {
      _state = yield_immediate;
    }
  }

  int yield() {
    int result = _sum;
    _sum = 0;
    _state = await_forever;
    return result;
  }

  auto state() const { return _state; }
};


int main() {
  sender<int> send;
  receiver<int> receiver;
  std::tie(send, receiver) = channel<int>(default_executor);

  std::atomic_bool done{false};

  auto calculator = receiver |
    adder{} |
    [&_done = done](int x) { std::cout << x << '\n';
      _done = true;
    };

  receiver.set_ready();

  send(1);
  send(2);
  send(3);
  send(0);

  // Waiting just for illustrational purpose
  while (!done) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  pre_exit();
}

/*
    Result:
      6
*/