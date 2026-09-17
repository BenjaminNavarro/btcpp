#pragma once

#include <btcpp/btcpp.hpp>

#include <vector>

namespace btgui {

struct NodePosition {
    int x{};
    int y{};
};

struct NodeStyle {
    int width{200};
    int height{80};
    int horizontal_margin{50};
    int vertical_margin{150};

    [[nodiscard]] int horizontal_spacing() const {
        return width + horizontal_margin;
    }

    [[nodiscard]] int vertical_spacing() const {
        return height + vertical_margin;
    }
};

using GraphLayout = std::vector<NodePosition>;

GraphLayout compute_graph_layout(const btcpp::GraphData& bt_data,
                                 const NodeStyle& style);

} // namespace btgui
