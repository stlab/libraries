#include <atomic>
#include <iostream>
#include <thread>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace std;
using namespace stlab;

int main() {
    sender<int> send1, send2, send3;
    receiver<int> receive1, receive2, receive3;

    tie(send1, receive1) = channel<int>(default_executor);
    tie(send2, receive2) = channel<int>(default_executor);
    tie(send3, receive3) = channel<int>(default_executor);

    std::atomic_int all_done{ 0 };

    auto merged = merge_channel<unordered_t>(default_executor,
                        [](int x) { return x; },
                        receive1,
                        receive2,
                        receive3)
        | [&_all_done = all_done](int x) {
              cout << x << '\n';
              ++_all_done;
          };

    receive1.set_ready();
    receive2.set_ready();
    receive3.set_ready();

    send1(1);
    send3(3);
    send2(2);
    send2(5);
    send3(6);
    send1(4);

    // Waiting just for illustrational purpose
    while (all_done < 6) {
        this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    pre_exit();
}

/*
        Result:

        The order of the value is not defined!

            1
            3
            2
            4
            5
            6
*/