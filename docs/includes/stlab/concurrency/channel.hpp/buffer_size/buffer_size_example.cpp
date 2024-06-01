#include <atomic>
#include <thread>
#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/default_executor.hpp>

using namespace stlab;
using namespace std;

int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_int v{0};

    // The order of the process and buffer_size is not relevant, so calling
    // times_two & buffer_size{ 2 } would be equivalent

    auto result = receive
        | buffer_size{ 2 } & [](int x) { return x * 2; }
        | [&v](int x) { v = x; };

    receive.set_ready();

    send(1);

    while (v == 0) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    pre_exit();
}

