#include <catch2/catch_test_macros.hpp>

import btcpp;
import test_utils;

TEST_CASE("Sequence") {

    SECTION("All success") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::SuccessAction{};
        auto node3 = testing::SuccessAction{};

        auto sequence = btcpp::Sequence{};
        sequence.add_child(&node1);
        sequence.add_child(&node2);
        sequence.add_child(&node3);

        REQUIRE(sequence.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("One failure") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::FailureAction{};
        auto node3 = testing::SuccessAction{};

        auto sequence = btcpp::Sequence{};
        sequence.add_child(&node1);
        sequence.add_child(&node2);
        sequence.add_child(&node3);

        REQUIRE(sequence.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All failures") {
        auto node1 = testing::FailureAction{};
        auto node2 = testing::FailureAction{};
        auto node3 = testing::FailureAction{};

        auto sequence = btcpp::Sequence{};
        sequence.add_child(&node1);
        sequence.add_child(&node2);
        sequence.add_child(&node3);

        REQUIRE(sequence.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All running") {
        auto node1 = testing::RunningAction{};
        auto node2 = testing::RunningAction{};
        auto node3 = testing::RunningAction{};

        auto sequence = btcpp::Sequence{};
        sequence.add_child(&node1);
        sequence.add_child(&node2);
        sequence.add_child(&node3);

        REQUIRE(sequence.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("Mixed success / running") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::RunningAction{};
        auto node3 = testing::SuccessAction{};

        auto sequence = btcpp::Sequence{};
        sequence.add_child(&node1);
        sequence.add_child(&node2);
        sequence.add_child(&node3);

        REQUIRE(sequence.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }
}

TEST_CASE("Fallback") {

    SECTION("All success") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::SuccessAction{};
        auto node3 = testing::SuccessAction{};

        auto fallback = btcpp::Fallback{};
        fallback.add_child(&node1);
        fallback.add_child(&node2);
        fallback.add_child(&node3);

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("One failure") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::SuccessAction{};
        auto node3 = testing::FailureAction{};

        auto fallback = btcpp::Fallback{};
        fallback.add_child(&node1);
        fallback.add_child(&node2);
        fallback.add_child(&node3);

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All failure") {
        auto node1 = testing::FailureAction{};
        auto node2 = testing::FailureAction{};
        auto node3 = testing::FailureAction{};

        auto fallback = btcpp::Fallback{};
        fallback.add_child(&node1);
        fallback.add_child(&node2);
        fallback.add_child(&node3);

        REQUIRE(fallback.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("All running") {
        auto node1 = testing::RunningAction{};
        auto node2 = testing::RunningAction{};
        auto node3 = testing::RunningAction{};

        auto fallback = btcpp::Fallback{};
        fallback.add_child(&node1);
        fallback.add_child(&node2);
        fallback.add_child(&node3);

        REQUIRE(fallback.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("Mixed success / running") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::RunningAction{};
        auto node3 = testing::SuccessAction{};

        auto fallback = btcpp::Fallback{};
        fallback.add_child(&node1);
        fallback.add_child(&node2);
        fallback.add_child(&node3);

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }
}

TEST_CASE("Parallel") {

    SECTION("Sucess rate") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::SuccessAction{};
        auto node3 = testing::SuccessAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

        REQUIRE(parallel.success_threshold() == 1);
        for (int i = 1; i <= parallel.children().size(); ++i) {
            REQUIRE_NOTHROW(parallel.set_success_threshold(i));
        }

        REQUIRE_THROWS_AS(parallel.set_success_threshold(0), std::logic_error);
        REQUIRE_THROWS_AS(parallel.set_success_threshold(-1), std::logic_error);
        REQUIRE_THROWS_AS(parallel.set_success_threshold(4), std::logic_error);
    }

    SECTION("All success") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::SuccessAction{};
        auto node3 = testing::SuccessAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

        REQUIRE(parallel.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("Some success") {
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::SuccessAction{};
        auto node3 = testing::RunningAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

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
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::FailureAction{};
        auto node3 = testing::FailureAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

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
        auto node1 = testing::FailureAction{};
        auto node2 = testing::FailureAction{};
        auto node3 = testing::FailureAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

        REQUIRE(parallel.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("All running") {
        auto node1 = testing::RunningAction{};
        auto node2 = testing::RunningAction{};
        auto node3 = testing::RunningAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

        REQUIRE(parallel.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("One running") {
        auto node1 = testing::RunningAction{};
        auto node2 = testing::FailureAction{};
        auto node3 = testing::FailureAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

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
        auto node1 = testing::SuccessAction{};
        auto node2 = testing::RunningAction{};
        auto node3 = testing::FailureAction{};

        auto parallel = btcpp::Parallel{};
        parallel.add_child(&node1);
        parallel.add_child(&node2);
        parallel.add_child(&node3);

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