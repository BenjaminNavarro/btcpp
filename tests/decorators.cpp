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

    [[nodiscard]] int failures() const {
        return failures_;
    }

private:
    int failures_;
};

TEST_CASE("Invert") {
    auto node1_inv = btcpp::Invert{testing::SuccessAction{}};
    auto node2_inv = btcpp::Invert{testing::RunningAction{}};
    auto node3_inv = btcpp::Invert{testing::FailureAction{}};

    REQUIRE(node1_inv.tick() == btcpp::failure);
    REQUIRE(node2_inv.tick() == btcpp::running);
    REQUIRE(node3_inv.tick() == btcpp::success);
}

TEST_CASE("Retry") {
    SECTION("RetryableAction") {
        auto action = RetryableAction{2};
        REQUIRE(action.failures() == 2);
        REQUIRE(action.tick() == btcpp::failure);
        REQUIRE(action.failures() == 1);
        REQUIRE(action.tick() == btcpp::failure);
        REQUIRE(action.failures() == 0);
        REQUIRE(action.tick() == btcpp::success);
        REQUIRE(action.failures() == 0);
    }

    SECTION("Enough retries") {
        auto retry = btcpp::Retry{RetryableAction{2}, 3};

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
        auto retry = btcpp::Retry{2};
        auto& action = retry.add_child<RetryableAction>(3);

        REQUIRE(retry.failures_count() == 0);
        REQUIRE(retry.retries() == 2);
        REQUIRE(action.failures() == 3);
        REQUIRE(retry.tick() == btcpp::running);
        REQUIRE(retry.failures_count() == 1);
        REQUIRE(action.failures() == 2);
        REQUIRE(retry.tick() == btcpp::running);
        REQUIRE(retry.failures_count() == 2);
        REQUIRE(action.failures() == 1);
        REQUIRE(retry.tick() == btcpp::failure);
        REQUIRE(retry.failures_count() == 3);
        REQUIRE(action.failures() == 0);
        REQUIRE(retry.tick() == btcpp::success);
        REQUIRE(retry.failures_count() == 0);
        REQUIRE(action.failures() == 0);
    }
}