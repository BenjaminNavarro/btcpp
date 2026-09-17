#include <btcpp/sequence.hpp>

namespace btcpp {

Sequence::Sequence(std::string_view name) : ControlNode{name} {
}

State Sequence::do_tick() {
    for (const auto& child : children()) {
        if (not child) {
            continue;
        }

        switch (child->tick()) {
        case State::Success:
            continue;
        case State::Failure:
            return State::Failure;
        case State::Running:
            return State::Running;
        }
    }

    return State::Success;
}

} // namespace btcpp