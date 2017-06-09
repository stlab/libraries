#include <iostream>

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

  auto calculator = receiver | adder{} | 
    [](int x) { std::cout << x << '\n'; };
    
  receiver.set_ready();

  while (true) {
    int x;
    std::cin >> x;
    send(x);
  }
}