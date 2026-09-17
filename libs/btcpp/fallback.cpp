#include <btcpp/fallback.hpp>

namespace btcpp {

Fallback::Fallback(std::string_view name) : ControlNode{name} {
}

State Fallback::do_tick() {
    for (const auto& child : children()) {
        if (not child) {
            continue;
        }

        switch (child->tick()) {
        case State::Success:
            return State::Success;
        case State::Failure:
            continue;
        case State::Running:
            return State::Running;
        }
    }

    return State::Failure;
}

} // namespace btcpp