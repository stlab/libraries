#include <atomic>
#include <iostream>
#include <thread>
#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace stlab;

struct nop_process
{
    process_state_scheduled _state = await_forever;

    int _v = 0;

    ~nop_process() { std::cout << "nop_process::~nop_process()\n"; }

    void await(int x) {
        _v = x;
        _state = yield_immediate;
    }

    int yield() {
        _state = await_forever;
        return _v;
    }

    auto state() const { return _state; }
};

int main() {
  sender<int> send;
  receiver<int> receive;
  std::tie(send, receive) = channel<int>(default_executor);
  std::atomic_bool done{ false };

  auto result = receive |
      nop_process{} |
      [&_done = done](int v) {
      std::cout << "Received " << v << '\n';
      _done = (v > 2);
  };

  receive.set_ready();
  std::cout << "Start sending...\n";
  send(1);
  send(2);
  send(3);
  std::cout << "Closing channel\n";
  send.close();

  while (!done) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << "Waited till the end.\n";

  pre_exit();
}

/*
 Result:
   nop_process::~nop_process()
   Start sending...
   Closing channel
   Received 1
   Received 2
   Received 3
   nop_process::~nop_process()
   Waited till the end.
 */