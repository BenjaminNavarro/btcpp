module;

#include <cassert>

export module btcpp:generic_condition;

import std;
import :execution_node;

export namespace btcpp {

template <typename F>
concept callable_condition =
    std::is_invocable_r_v<State, F> or std::is_invocable_r_v<bool, F>;

class GenericCondition : public Condition {
public:
    template <callable_condition T>
    GenericCondition(T&& condition) {
        if constexpr (std::is_invocable_r_v<bool, T>) {
            condition_ = [condition] mutable {
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

    [[nodiscard]] State tick() final {
        const auto state = condition_();
        if (state == State::Running) {
            throw std::logic_error(
                "[GenericCondition] conditions cannot be in a 'Running' state");
        }
        return state;
    }

private:
    std::function<State()> condition_;
};

} // namespace btcpp