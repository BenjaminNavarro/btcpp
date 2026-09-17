#pragma once

#include <btcpp/control_node.hpp>

namespace btcpp {

class Sequence final : public ControlNode {
public:
    Sequence(std::string_view name = {});

private:
    [[nodiscard]] State do_tick() final;
};

} // namespace btcpp