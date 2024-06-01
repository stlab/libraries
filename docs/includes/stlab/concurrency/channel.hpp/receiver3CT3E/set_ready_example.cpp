#include <atomic>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace std;
using namespace stlab;

int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_bool done{ false };

    auto hold = receive | [&_done = done](int x) {
        cout << x << '\n';
        _done = true;
      };

    // It is necessary to mark the receiver side as ready, when all connections are
    // established
    receive.set_ready();

    send(42);

    // Waiting just for illustrational purpose
    while (!done.load()) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    pre_exit();
}


/*
        Result:
            42
*/