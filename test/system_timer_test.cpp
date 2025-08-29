
#include <stlab/concurrency/system_timer.hpp>

#include <chrono>
#include <utility>

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/await.hpp>

/**************************************************************************************************/

using namespace stlab;

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(system_timer_cancellation) {
    // Test to ensure that a task scheduled after pre_exit() is not executed
    system_timer(std::chrono::hours(1), []() noexcept { BOOST_CHECK(false); });

    auto [task, future] = package<int()>(immediate_executor, [] { return 42; });
    system_timer(std::chrono::seconds(0), std::move(task));

    BOOST_CHECK_EQUAL(await(std::move(future)), 42);
}

/**************************************************************************************************/
