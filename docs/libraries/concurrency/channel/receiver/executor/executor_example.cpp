#include <atomic>
#include <thread>
#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>

using namespace stlab;
using namespace std;

int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_int v{0};

    // The process times_two will be executed immediately
    // The order of the process and executor is not relevant so calling
    // times_two & executor{ immediate_executor } would be equivalent
    auto result = receive
        | executor{ immediate_executor } & [](int x) { return x * 2; }
        | [&v](int x) { v = x; };

    receive.set_ready();

    send(1);

    // Waiting just for illustrational purpose
    while (v == 0) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    pre_exit();
}
