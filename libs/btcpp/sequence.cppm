export module btcpp:sequence;

import std;
import :control_node;

export namespace btcpp {

class Sequence final : public ControlNode {
public:
    Sequence(std::string_view name = {}) : ControlNode{name} {
    }

private:
    [[nodiscard]] State do_tick() final {
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
};

} // namespace btcpp