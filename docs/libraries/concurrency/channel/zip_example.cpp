#include <atomic>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace std;
using namespace stlab;

int main() {
    sender<int> send1;
    receiver<int> receive1;
    sender<double> send2;
    receiver<double> receive2;
    tie(send1, receive1) = channel<int>(default_executor);
    tie(send2, receive2) = channel<double>(default_executor);

    std::atomic_int all_done{ 0 };

    auto zipped = zip(default_executor,
                      receive1,
                      receive2)
        | [&_all_done = all_done](std::tuple<int, double> v) {
              cout << get<0>(v) << " " << get<1>(v) << '\n';
              ++_all_done;
          };

    receive1.set_ready();
    receive2.set_ready();

    // even the values are send in mixed order into the channels, the are passed
    // in step order into the zipped process
    send1(1);
    send2(2.5);
    send2(5.7);
    send1(4);

    while (all_done < 2) {
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    pre_exit();
}

/*
    Result:
        1 2.5
        4 5.7
*/