#include <catch2/catch_test_macros.hpp>

#include <btcpp/btcpp.hpp>
#include "test_utils.hpp"

TEST_CASE("Sequence") {
    SECTION("All success") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::SuccessAction>("node1");
        auto& node2 = sequence.add_child<testing::SuccessAction>("node2");
        auto& node3 = sequence.add_child<testing::SuccessAction>("node3");

        REQUIRE(sequence.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("One failure") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::SuccessAction>("node1");
        auto& node2 = sequence.add_child<testing::FailureAction>("node2");
        auto& node3 = sequence.add_child<testing::SuccessAction>("node3");

        REQUIRE(sequence.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All failures") {

        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::FailureAction>("node1");
        auto& node2 = sequence.add_child<testing::FailureAction>("node2");
        auto& node3 = sequence.add_child<testing::FailureAction>("node3");

        REQUIRE(sequence.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All running") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::RunningAction>("node1");
        auto& node2 = sequence.add_child<testing::RunningAction>("node2");
        auto& node3 = sequence.add_child<testing::RunningAction>("node3");

        REQUIRE(sequence.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("Mixed success / running") {
        auto sequence = btcpp::Sequence{};
        auto& node1 = sequence.add_child<testing::SuccessAction>("node1");
        auto& node2 = sequence.add_child<testing::RunningAction>("node2");
        auto& node3 = sequence.add_child<testing::SuccessAction>("node3");

        REQUIRE(sequence.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }
}

TEST_CASE("Fallback") {

    SECTION("All success") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::SuccessAction>("node1");
        auto& node2 = fallback.add_child<testing::SuccessAction>("node2");
        auto& node3 = fallback.add_child<testing::SuccessAction>("node3");

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("One failure") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::SuccessAction>("node1");
        auto& node2 = fallback.add_child<testing::SuccessAction>("node2");
        auto& node3 = fallback.add_child<testing::FailureAction>("node3");

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("All failure") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::FailureAction>("node1");
        auto& node2 = fallback.add_child<testing::FailureAction>("node2");
        auto& node3 = fallback.add_child<testing::FailureAction>("node3");

        REQUIRE(fallback.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("All running") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::RunningAction>("node1");
        auto& node2 = fallback.add_child<testing::RunningAction>("node2");
        auto& node3 = fallback.add_child<testing::RunningAction>("node3");

        REQUIRE(fallback.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }

    SECTION("Mixed success / running") {
        auto fallback = btcpp::Fallback{};
        auto& node1 = fallback.add_child<testing::SuccessAction>("node1");
        auto& node2 = fallback.add_child<testing::RunningAction>("node2");
        auto& node3 = fallback.add_child<testing::SuccessAction>("node3");

        REQUIRE(fallback.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE_FALSE(node2.ticked());
        REQUIRE_FALSE(node3.ticked());
    }
}

TEST_CASE("Parallel") {

    SECTION("Sucess rate") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::SuccessAction>("node1");
        auto& node2 = parallel.add_child<testing::SuccessAction>("node2");
        auto& node3 = parallel.add_child<testing::SuccessAction>("node3");

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
        auto& node1 = parallel.add_child<testing::SuccessAction>("node1");
        auto& node2 = parallel.add_child<testing::SuccessAction>("node2");
        auto& node3 = parallel.add_child<testing::SuccessAction>("node3");

        REQUIRE(parallel.tick() == btcpp::success);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("Some success") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::SuccessAction>("node1");
        auto& node2 = parallel.add_child<testing::SuccessAction>("node2");
        auto& node3 = parallel.add_child<testing::RunningAction>("node3");

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
        auto& node1 = parallel.add_child<testing::SuccessAction>("node1");
        auto& node2 = parallel.add_child<testing::FailureAction>("node2");
        auto& node3 = parallel.add_child<testing::FailureAction>("node3");

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
        auto& node1 = parallel.add_child<testing::FailureAction>("node1");
        auto& node2 = parallel.add_child<testing::FailureAction>("node2");
        auto& node3 = parallel.add_child<testing::FailureAction>("node3");

        REQUIRE(parallel.tick() == btcpp::failure);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("All running") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::RunningAction>("node1");
        auto& node2 = parallel.add_child<testing::RunningAction>("node2");
        auto& node3 = parallel.add_child<testing::RunningAction>("node3");

        REQUIRE(parallel.tick() == btcpp::running);
        REQUIRE(node1.ticked());
        REQUIRE(node2.ticked());
        REQUIRE(node3.ticked());
    }

    SECTION("One running") {
        auto parallel = btcpp::Parallel{};
        auto& node1 = parallel.add_child<testing::RunningAction>("node1");
        auto& node2 = parallel.add_child<testing::FailureAction>("node2");
        auto& node3 = parallel.add_child<testing::FailureAction>("node3");

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
        auto& node1 = parallel.add_child<testing::SuccessAction>("node1");
        auto& node2 = parallel.add_child<testing::RunningAction>("node2");
        auto& node3 = parallel.add_child<testing::FailureAction>("node3");

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