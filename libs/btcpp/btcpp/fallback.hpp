#pragma once

#include <btcpp/control_node.hpp>

namespace btcpp {

class Fallback final : public ControlNode {
public:
    Fallback(std::string_view name = {});

private:
    [[nodiscard]] State do_tick() final;
};

} // namespace btcpp