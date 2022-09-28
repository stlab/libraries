#include <atomic>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace std;
using namespace stlab;

struct sum_2
{
    process_state_scheduled _state = await_forever;
    int _counter = 0;
    int _sum = 0;

    void await(int n) {
        _sum += n;
        ++_counter;
        if (_counter == 2) _state = yield_immediate;
    }

    int yield() {
      _state = await_forever;
      auto result = _sum;
      _sum = 0;
      _counter = 0;
      return result;
    }

    auto state() const { return _state; }
};


int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_int r{0};

    auto hold = receive
        | sum_2()
        | [&_r = r](int x) { _r = x; };

    receive.set_ready();

    send(1);
    send(2);

    // Waiting just for illustrational purpose
    while (r == 0) {
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    cout << "The sum is " << r.load() << '\n';

    pre_exit();
}

/*
    Result:

        The sum is 3
*/
