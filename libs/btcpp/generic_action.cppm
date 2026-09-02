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
    GenericAction(T&& action) : action_{std::forward<T>(action)} {
    }

private:
    [[nodiscard]] State do_tick() final {
        return action_();
    }

    std::function<State()> action_;
};

} // namespace btcpp