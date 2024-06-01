#include <iostream>
#include <stdexcept>
#include <thread>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

using namespace std;
using namespace stlab;

int main() {
    auto x = async(default_executor, [] {
      throw runtime_error("Vogons did arrive!");
      return 42;
    });

    auto r = x.recover([](future<int> f) {
        try {
          auto answer = *f.get_try();
          cout << "The answer is " << answer << '\n';
        }
        catch (const exception& ex) {
          cout << "The error \"" << ex.what() << "\" happened!\n";
        }
      });

    // Waiting just for illustration purpose
    while (!r.get_try()) { this_thread::sleep_for(chrono::milliseconds(1)); }

    pre_exit();
}

// Result: The error "Vogons did arrive!" happened!
