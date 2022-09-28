#include <iostream>
#include <mutex>
#include <thread>
#include <deque>

#include <stlab/scope.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/utility.hpp>

using namespace stlab;
using namespace std;

auto pop_fn() {
    return []{
        static deque<int> q{1, 2, 3, 4};
        static mutex      m;
        return scope<lock_guard<mutex>>(m, []{
            auto v = move(q.front());
            q.pop_front();
            return v;
        });
    };
}

int main(int, char**) {
    cout << "Number of concurrent threads: " << thread::hardware_concurrency() << '\n';

    executor_t           ioq(serial_queue_t(immediate_executor).executor());
    vector<future<void>> futures;
    auto                 popper(pop_fn());

    for (std::size_t i(0); i < 4; ++i)
        futures.emplace_back(stlab::async(default_executor, popper)
            .then(ioq, [](int x) {
                cout << x << '\n';
            }));

    auto done = when_all(default_executor,
                         []{},
                         std::make_pair(begin(futures), end(futures)));

    blocking_get(done);

    pre_exit();
}
/*
    Result:

        1
        2
        3
        4
*/
