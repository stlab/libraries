
#define STLAB_FUTURE_COROUTINES 1

#include <iostream>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/utility.hpp>


stlab::future<int> get_the_answer() { co_return 42; }

int main() {
	auto f = get_the_answer();

	std::cout << stlab::blocking_get(f) << '\n';

	pre_exit();
}

/*
    Result:

        42
*/
