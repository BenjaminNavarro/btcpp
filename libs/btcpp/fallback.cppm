export module btcpp:fallback;

import std;
import :control_node;

export namespace btcpp {

class Fallback final : public ControlNode {
public:
    [[nodiscard]] State tick() final {
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

private:
};

} // namespace btcpp