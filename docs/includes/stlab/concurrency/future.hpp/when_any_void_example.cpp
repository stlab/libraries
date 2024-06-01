#include <iostream>
#include <string>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/utility.hpp>


using namespace std;
using namespace stlab;

int main() {
    auto fv = []{
        return stlab::make_ready_future(immediate_executor);
    };

    auto f = when_any(stlab::default_executor, [](size_t index){
        std::cout << "f: " << index << '\n';
    }, fv(), fv(), fv(), fv(), fv(), fv());

    while (!f.get_try()) std::this_thread::sleep_for(std::chrono::milliseconds(1));

    pre_exit();
}

/*
    Result: (index number may vary)

        f: 0
*/
