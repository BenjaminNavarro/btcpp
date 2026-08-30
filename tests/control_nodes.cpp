#include <catch2/catch_test_macros.hpp>

import btcpp;

template <btcpp::State S>
class ActionResult final : public btcpp::ExecutionNode {
public:
    using btcpp::ExecutionNode::ExecutionNode;

    [[nodiscard]] btcpp::State tick() final {
        ticked_ = true;
        return S;
    }

    [[nodiscard]] bool ticked() const {
        return ticked_;
    }

private:
    bool ticked_{};
};

using SuccessAction = ActionResult<btcpp::success>;
using FailureAction = ActionResult<btcpp::failure>;
using RunningAction = ActionResult<btcpp::running>;

TEST_CASE("Sequence") {

    SECTION("All success") {
        auto node1 = SuccessAction{};
        auto node2 = SuccessAction{};
        auto node3 = SuccessAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = FailureAction{};
        auto node3 = SuccessAction{};

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
        auto node1 = FailureAction{};
        auto node2 = FailureAction{};
        auto node3 = FailureAction{};

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
        auto node1 = RunningAction{};
        auto node2 = RunningAction{};
        auto node3 = RunningAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = RunningAction{};
        auto node3 = SuccessAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = SuccessAction{};
        auto node3 = SuccessAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = SuccessAction{};
        auto node3 = FailureAction{};

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
        auto node1 = FailureAction{};
        auto node2 = FailureAction{};
        auto node3 = FailureAction{};

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
        auto node1 = RunningAction{};
        auto node2 = RunningAction{};
        auto node3 = RunningAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = RunningAction{};
        auto node3 = SuccessAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = SuccessAction{};
        auto node3 = SuccessAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = SuccessAction{};
        auto node3 = SuccessAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = SuccessAction{};
        auto node3 = RunningAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = FailureAction{};
        auto node3 = FailureAction{};

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
        auto node1 = FailureAction{};
        auto node2 = FailureAction{};
        auto node3 = FailureAction{};

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
        auto node1 = RunningAction{};
        auto node2 = RunningAction{};
        auto node3 = RunningAction{};

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
        auto node1 = RunningAction{};
        auto node2 = FailureAction{};
        auto node3 = FailureAction{};

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
        auto node1 = SuccessAction{};
        auto node2 = RunningAction{};
        auto node3 = FailureAction{};

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