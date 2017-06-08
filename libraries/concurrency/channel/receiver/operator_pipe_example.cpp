#include <atomic>
#include <thread>
#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/main_executor.hpp>

int main() {
    sender<int> send;
    receiver<int> receive;

    tie(send, receive) = channel<int>(default_executor);

    std::atomic_int v{0};

    // The code demonstrates how a process can be annotated
    auto result = receive 
        | buffer_size{ 3 } & [](int x) { return x * 2; }
        | [](int x) { return x * 2; } & buffer_size{ 2 }
        | buffer_size{ 3 } & executor{ default_executor } & [](int x) { return x * 2; } 

        | executor{ default_executor } & [](int x) { return x + 1; }
        | [](int x) { return x + 1; } & executor{ main_executor }
        | executor{ default_executor } & buffer_size{ 3 } & [](int x) { return x * 2; }
    
        | [](int x) { return x + 1; } & executor{ default_executor } & buffer_size{ 3 }
        | [](int x) { return x * 2; } & buffer_size{ 3 } & executor{ main_executor }
        
        | [&v](int x) { v = x; };
        
    receive.set_ready();

    send(1);

    while (v == 0) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

