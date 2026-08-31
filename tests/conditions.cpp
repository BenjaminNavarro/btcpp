#include <catch2/catch_test_macros.hpp>

import btcpp;

TEST_CASE("GenericCondition") {
    SECTION("Basic usage") {
        int value{};

        auto is_even =
            btcpp::GenericCondition([&] { return (value % 2) == 0; });
        auto is_odd = btcpp::GenericCondition(
            [&] { return (value % 2) != 0 ? btcpp::success : btcpp::failure; });

        REQUIRE(is_even.tick() == btcpp::success);
        REQUIRE(is_odd.tick() == btcpp::failure);

        ++value;

        REQUIRE(is_even.tick() == btcpp::failure);
        REQUIRE(is_odd.tick() == btcpp::success);
    }

    SECTION("Invalid return state") {
        auto running_condition =
            btcpp::GenericCondition{[] { return btcpp::running; }};

        REQUIRE_THROWS_AS(running_condition.tick(), std::logic_error);
    }
}