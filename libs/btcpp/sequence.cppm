export module btcpp:sequence;

import std;
import :control_node;

export namespace btcpp {

class Sequence final : public ControlNode {
public:
    using ControlNode::ControlNode;

    [[nodiscard]] State tick() final {
        for (auto* child : children()) {
            if (child == nullptr) {
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

private:
};

} // namespace btcpp