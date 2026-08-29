module;

#include <cassert>

export module btcpp:generic_action;

import std;
import :execution_node;

export namespace btcpp {

class GenericAction : public Action {
public:
    GenericAction(std::function<State()> action)
        : GenericAction{std::move(action), nullptr} {
    }

    GenericAction(std::function<State()> action, Node* parent)
        : Action{parent}, action_{std::move(action)} {
        assert(action);
    }

    [[nodiscard]] State tick() final {
        return action_();
    }

private:
    std::function<State()> action_;
};

} // namespace btcpp