---
layout: free-function
title: stlab::package
tags: [library]
pure-name: package
brief: Create a packaged_task/future pair
entities:
  - kind: overloads
    name: stlab::package
    defined-in-header: stlab/future.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/future.hpp
    list:
      - name: package
        pure-name: package
        declaration: |
            template <typename Sig, typename E, typename F>
            auto package(E executor, F f) ->
                std::pair<detail::packaged_task_from_signature_t<Sig>,
                          future<detail::result_of_t_<Sig>>>
        description: The template function package creates a pair of a packaged_task and a future.
  - kind: parameters
    list:
      - name: executor
        description: The passed function will run on this executor
      - name: f
        description: Callable object to call
  - kind: result
    description: A std::pair of a packaged_task and the associated future.
---

### Example ###

~~~ c++
#include <cstdio>
#include <string>
#include <thread>
#include <stlab/default_executor.hpp>
#include <stlab/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto p = package<int(int)>(default_executor, [](int x) { return x+x; });
    auto packagedTask = p.first;
    auto f = p.second;

    packagedTask(21);

    // Waiting just for illustrational purpose
    while (!f.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    cout << "The answer is " << f.get_try().value() << "\n";
}
~~~

### Result ###

~~~
The answer is 42
~~~
