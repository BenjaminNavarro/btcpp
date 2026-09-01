#include <catch2/catch_test_macros.hpp>

import btcpp;
import test_utils;

TEST_CASE("Sequence") {
    SECTION("All success") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::SuccessAction>();
        auto& node2 = sequence.add_child<testing::SuccessAction>();
        auto& node3 = sequence.add_child<testing::SuccessAction>();

        REQUIRE(sequence.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("One failure") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::SuccessAction>();
        auto& node2 = sequence.add_child<testing::FailureAction>();
        auto& node3 = sequence.add_child<testing::SuccessAction>();

        REQUIRE(sequence.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All failures") {

        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::FailureAction>();
        auto& node2 = sequence.add_child<testing::FailureAction>();
        auto& node3 = sequence.add_child<testing::FailureAction>();

        REQUIRE(sequence.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All running") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::RunningAction>();
        auto& node2 = sequence.add_child<testing::RunningAction>();
        auto& node3 = sequence.add_child<testing::RunningAction>();

        REQUIRE(sequence.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("Mixed success / running") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::SuccessAction>();
        auto& node2 = sequence.add_child<testing::RunningAction>();
        auto& node3 = sequence.add_child<testing::SuccessAction>();

        REQUIRE(sequence.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }
}

TEST_CASE("Fallback") {

    SECTION("All success") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::SuccessAction>();
        auto& node2 = fallback.add_child<testing::SuccessAction>();
        auto& node3 = fallback.add_child<testing::SuccessAction>();

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("One failure") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::SuccessAction>();
        auto& node2 = fallback.add_child<testing::SuccessAction>();
        auto& node3 = fallback.add_child<testing::FailureAction>();

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All failure") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::FailureAction>();
        auto& node2 = fallback.add_child<testing::FailureAction>();
        auto& node3 = fallback.add_child<testing::FailureAction>();

        REQUIRE(fallback.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("All running") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::RunningAction>();
        auto& node2 = fallback.add_child<testing::RunningAction>();
        auto& node3 = fallback.add_child<testing::RunningAction>();

        REQUIRE(fallback.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("Mixed success / running") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::SuccessAction>();
        auto& node2 = fallback.add_child<testing::RunningAction>();
        auto& node3 = fallback.add_child<testing::SuccessAction>();

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }
}

TEST_CASE("Parallel") {

    SECTION("Sucess rate") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::SuccessAction>();
        auto& node2 = parallel.add_child<testing::SuccessAction>();
        auto& node3 = parallel.add_child<testing::SuccessAction>();

        REQUIRE(parallel.success_threshold() == 1);
        for (int i = 1; i <= parallel.children().size(); ++i) {
            REQUIRE_NOTHROW(parallel.set_success_threshold(i));
        }

        REQUIRE_THROWS_AS(parallel.set_success_threshold(0), std::logic_error);
        REQUIRE_THROWS_AS(parallel.set_success_threshold(-1), std::logic_error);
        REQUIRE_THROWS_AS(parallel.set_success_threshold(4), std::logic_error);
    }

    SECTION("All success") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::SuccessAction>();
        auto& node2 = parallel.add_child<testing::SuccessAction>();
        auto& node3 = parallel.add_child<testing::SuccessAction>();

        REQUIRE(parallel.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("Some success") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::SuccessAction>();
        auto& node2 = parallel.add_child<testing::SuccessAction>();
        auto& node3 = parallel.add_child<testing::RunningAction>();

        REQUIRE(parallel.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());

        parallel.set_success_threshold(2);
        REQUIRE(parallel.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());

        parallel.set_success_threshold(3);
        REQUIRE(parallel.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("One success") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::SuccessAction>();
        auto& node2 = parallel.add_child<testing::FailureAction>();
        auto& node3 = parallel.add_child<testing::FailureAction>();

        REQUIRE(parallel.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());

        parallel.set_success_threshold(2);
        REQUIRE(parallel.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());

        parallel.set_success_threshold(3);
        REQUIRE(parallel.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("All failure") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::FailureAction>();
        auto& node2 = parallel.add_child<testing::FailureAction>();
        auto& node3 = parallel.add_child<testing::FailureAction>();

        REQUIRE(parallel.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("All running") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::RunningAction>();
        auto& node2 = parallel.add_child<testing::RunningAction>();
        auto& node3 = parallel.add_child<testing::RunningAction>();

        REQUIRE(parallel.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("One running") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::RunningAction>();
        auto& node2 = parallel.add_child<testing::FailureAction>();
        auto& node3 = parallel.add_child<testing::FailureAction>();

        REQUIRE(parallel.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());

        parallel.set_success_threshold(2);
        REQUIRE(parallel.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("Mixed success / running / failure") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::SuccessAction>();
        auto& node2 = parallel.add_child<testing::RunningAction>();
        auto& node3 = parallel.add_child<testing::FailureAction>();

        REQUIRE(parallel.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());

        parallel.set_success_threshold(2);
        REQUIRE(parallel.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());

        parallel.set_success_threshold(3);
        REQUIRE(parallel.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }
}