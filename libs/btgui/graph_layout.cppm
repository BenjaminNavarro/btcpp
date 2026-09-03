module;

#include <QtCore/QPointF>

export module btgui:layout;

import std;
import btcpp;

namespace {
std::map<int, int> compute_layer_widths(const btcpp::GraphData& bt_data,
                                        int node_id) {
    std::map<int, int> max_per_layer;

    auto width_under = [&](this auto& self, int node_id,
                           int current_layer) -> void {
        max_per_layer[current_layer] += 1;

        const auto& node_data = bt_data.at(node_id);

        for (int child_id : node_data.children) {
            self(child_id, current_layer + 1);
        }
    };

    width_under(node_id, 0);

    return max_per_layer;
}
} // namespace

export namespace btgui {

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
                                 const NodeStyle& style) {
    GraphLayout positions;
    positions.resize(bt_data.size());

    const auto layer_widths = compute_layer_widths(bt_data, 0);
    const auto max_width = std::ranges::max_element(layer_widths)->second;

    const auto graph_width = max_width * style.horizontal_spacing();

    std::map<int, double> index_in_layer;
    auto set_position = [&](this auto& self, int node_id,
                            int current_layer) -> void {
        const auto& node_data = bt_data.at(node_id);

        const auto layer_width = layer_widths.at(current_layer);

        auto set_node_position = [&](auto pos_x, auto pos_y) {
            positions[node_id] = NodePosition{.x = static_cast<int>(pos_x),
                                              .y = static_cast<int>(pos_y)};
        };

        const auto pos_y = current_layer * style.vertical_spacing();
        if (layer_width == max_width) {
            const auto pos_x = (graph_width * 1. / layer_width *
                                index_in_layer[current_layer]) +
                               style.horizontal_spacing();
            set_node_position(pos_x, pos_y);
        } else {
            const auto pos_x = (graph_width * 1. / (1. + layer_width) *
                                (1. + index_in_layer[current_layer])) +
                               (style.width / 2.);
            set_node_position(pos_x, pos_y);
        }

        for (int child_id : node_data.children) {
            self(child_id, current_layer + 1);
        }

        index_in_layer[current_layer] += 1.;
    };

    set_position(0, 0);

    return positions;
}

} // namespace btgui
