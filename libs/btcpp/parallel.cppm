export module btcpp:parallel;

import std;
import :control_node;

export namespace btcpp {

class Parallel final : public ControlNode {
public:
    void set_success_threshold(int threshold) {
        if (threshold < 1) {
            throw std::logic_error{std::format(
                "[Parallel] Success threshold ({}) must be at least 1",
                threshold, children().size())};
        }

        if (threshold > children().size()) {
            throw std::logic_error{
                std::format("[Parallel] Success threshold ({}) too high for "
                            "the number of children ({})",
                            threshold, children().size())};
        }

        success_threshold_ = threshold;
    }

    [[nodiscard]] int success_threshold() const {
        return success_threshold_;
    }

    [[nodiscard]] State tick() final {
        int successes{0};
        int failures{0};
        for (const auto& child : children()) {
            if (not child) {
                continue;
            }

            switch (child->tick()) {
            case success:
                ++successes;
                continue;
            case failure:
                ++failures;
                continue;
            case running:
                continue;
            }
        }

        if (successes >= success_threshold_) {
            return success;
        } else if (failures > (children().size() - success_threshold_)) {
            return failure;
        } else {
            return running;
        }
    }

private:
    int success_threshold_{1};
};

} // namespace btcpp