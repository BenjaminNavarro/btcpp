#include <catch2/catch_test_macros.hpp>

import btcpp;
import test_utils;

template <int N>
class TestInternalNode final : public btcpp::InternalNode {
public:
    TestInternalNode() : btcpp::InternalNode(N) {
    }

private:
    [[nodiscard]] btcpp::State do_tick() final {
        return btcpp::success;
    }
};

TEST_CASE("InternalNode") {
    SECTION("Construction") {
        {
            auto node = TestInternalNode<0>{};
            REQUIRE(node.max_children_count() == 0);
        }
        {
            auto node = TestInternalNode<3>{};
            REQUIRE(node.max_children_count() == 3);
        }
        {
            REQUIRE_THROWS_AS(TestInternalNode<-3>{}, std::logic_error);
        }
    }

    SECTION("Add child") {
        auto node = TestInternalNode<2>{};

        auto& success_action = node.add_child<testing::SuccessAction>();
        auto& failure_action = node.add_child<testing::FailureAction>();
        REQUIRE_THROWS_AS(node.add_child<testing::SuccessAction>(),
                          std::logic_error);

        STATIC_REQUIRE(
            std::is_same_v<decltype(success_action), testing::SuccessAction&>);
        STATIC_REQUIRE(
            std::is_same_v<decltype(failure_action), testing::FailureAction&>);
    }

    SECTION("Add children") {
        auto node = TestInternalNode<2>{};
        {
            auto& children = node.add_children(testing::SuccessAction{},
                                               testing::FailureAction{});
            REQUIRE(children.size() == 2);
        }
        {
            REQUIRE_THROWS_AS(node.add_children(testing::SuccessAction{},
                                                testing::FailureAction{},
                                                testing::RunningAction{}),
                              std::logic_error);
        }
    }
}