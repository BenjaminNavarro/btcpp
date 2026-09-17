#pragma once

#include <btcpp/control_node.hpp>

namespace btcpp {

class Parallel final : public ControlNode {
public:
    Parallel(std::string_view name = {});

    void set_success_threshold(int threshold);

    [[nodiscard]] int success_threshold() const {
        return success_threshold_;
    }

private:
    [[nodiscard]] State do_tick() final;

    int success_threshold_{1};
};

} // namespace btcpp