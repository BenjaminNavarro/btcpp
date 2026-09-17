#pragma once

#include <btcpp/execution_node.hpp>

#include <cassert>
#include <functional>
#include <string_view>
#include <type_traits>

namespace btcpp {

template <typename F>
concept callable_action = std::is_invocable_r_v<State, F>;

class GenericAction final : public Action {
public:
    template <callable_action T>
    GenericAction(T&& action, std::string_view name = {})
        : Action{name}, action_{std::forward<T>(action)} {
    }

private:
    [[nodiscard]] State do_tick() final;

    std::function<State()> action_;
};

} // namespace btcpp