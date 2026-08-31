#include <catch2/catch_test_macros.hpp>

import btcpp;
import test_utils;

class RetryableAction final : public btcpp::Action {
public:
    RetryableAction(int failures) : failures_{failures} {
    }

    [[nodiscard]] btcpp::State tick() final {
        if (failures_ > 0) {
            --failures_;
            return btcpp::failure;
        } else {
            return btcpp::success;
        }
    }

private:
    int failures_;
};

TEST_CASE("Invert") {
    auto node1 = testing::SuccessAction{};
    auto node2 = testing::RunningAction{};
    auto node3 = testing::FailureAction{};

    auto node1_inv = btcpp::Invert{&node1};
    auto node2_inv = btcpp::Invert{&node2};
    auto node3_inv = btcpp::Invert{&node3};

    REQUIRE(node1_inv.tick() == btcpp::failure);
    REQUIRE(node2_inv.tick() == btcpp::running);
    REQUIRE(node3_inv.tick() == btcpp::success);
}

TEST_CASE("Retry") {
    SECTION("RetryableAction") {
        auto action = RetryableAction{2};
        REQUIRE(action.tick() == btcpp::failure);
        REQUIRE(action.tick() == btcpp::failure);
        REQUIRE(action.tick() == btcpp::success);
    }
    SECTION("Enough retries") {
        auto action = RetryableAction{2};
        auto retry = btcpp::Retry{&action, 3};

        REQUIRE(retry.failures_count() == 0);
        REQUIRE(retry.retries() == 3);
        REQUIRE(retry.tick() == btcpp::running);
        REQUIRE(retry.failures_count() == 1);
        REQUIRE(retry.tick() == btcpp::running);
        REQUIRE(retry.failures_count() == 2);
        REQUIRE(retry.tick() == btcpp::success);
        REQUIRE(retry.failures_count() == 0);
    }
    SECTION("Not enough retries") {
        auto action = RetryableAction{3};
        auto retry = btcpp::Retry{&action, 2};

        REQUIRE(retry.failures_count() == 0);
        REQUIRE(retry.retries() == 2);
        REQUIRE(retry.tick() == btcpp::running);
        REQUIRE(retry.failures_count() == 1);
        REQUIRE(retry.tick() == btcpp::running);
        REQUIRE(retry.failures_count() == 2);
        REQUIRE(retry.tick() == btcpp::failure);
        REQUIRE(retry.failures_count() == 3);
        REQUIRE(retry.tick() == btcpp::success);
        REQUIRE(retry.failures_count() == 0);
    }
}