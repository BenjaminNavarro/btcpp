module;

#include <cassert>

export module btcpp:generic_action;

import std;
import :execution_node;

export namespace btcpp {

template <typename F>
concept callable_action = std::is_invocable_r_v<State, F>;

class GenericAction : public Action {
public:
    template <callable_action T>
    GenericAction(T&& action)
        : GenericAction{std::forward<T>(action), nullptr} {
    }

    template <callable_action T>
    GenericAction(T&& action, Node* parent)
        : Action{parent}, action_{std::forward<T>(action)} {
    }

    [[nodiscard]] State tick() final {
        return action_();
    }

private:
    std::function<State()> action_;
};

} // namespace btcpp