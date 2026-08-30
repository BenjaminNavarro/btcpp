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