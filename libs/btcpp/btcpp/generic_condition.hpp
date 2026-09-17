#pragma once

#include <btcpp/execution_node.hpp>

#include <cassert>
#include <type_traits>
#include <string_view>
#include <functional>

namespace btcpp {

template <typename F>
concept callable_condition =
    std::is_invocable_r_v<State, F> or std::is_invocable_r_v<bool, F>;

class GenericCondition final : public Condition {
public:
    template <callable_condition T>
    GenericCondition(T&& condition, std::string_view name = {})
        : Condition{name} {
        if constexpr (std::is_invocable_r_v<bool, T>) {
            condition_ = [condition]() mutable {
                if (condition()) {
                    return State::Success;
                } else {
                    return State::Failure;
                }
            };
        } else {
            condition_ = condition;
        }
    }

private:
    [[nodiscard]] State do_tick() final;

    std::function<State()> condition_;
};

} // namespace btcpp