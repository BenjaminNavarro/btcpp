#include <catch2/catch_test_macros.hpp>

import btcpp;

TEST_CASE("GenericAction") {
    SECTION("Basic usage") {
        auto will_succeed = btcpp::GenericAction([idx = 0] mutable {
            const auto state = idx < 1 ? btcpp::running : btcpp::success;
            ++idx;
            return state;
        });

        auto will_fail = btcpp::GenericAction([idx = 0] mutable {
            const auto state = idx < 1 ? btcpp::running : btcpp::failure;
            ++idx;
            return state;
        });

        REQUIRE(will_succeed.tick() == btcpp::running);
        REQUIRE(will_fail.tick() == btcpp::running);

        REQUIRE(will_succeed.tick() == btcpp::success);
        REQUIRE(will_fail.tick() == btcpp::failure);
    }
}