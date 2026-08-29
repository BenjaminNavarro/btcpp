export module btcpp:fallback;

import std;
import :control_node;

export namespace btcpp {

class Fallback final : public ControlNode {
public:
    using ControlNode::ControlNode;

    [[nodiscard]] State tick() final {
        for (auto* child : children()) {
            if (child == nullptr) {
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

private:
};

} // namespace btcpp