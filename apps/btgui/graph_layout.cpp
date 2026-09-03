#include "graph_layout.hpp"

#include <algorithm>

import std;
import btcpp;

std::map<int, std::size_t>
compute_layer_widths(const std::map<int, btcpp::NodeData>& bt_data,
                     int node_id) {
    std::map<int, std::size_t> max_per_layer;

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

// For each node, compute the maximum width (number of node per layer) for all
// its descendants.
// Then, for each layer, from left to right, center each node based on the
// maximum width of its descendants.
std::map<int, QPointF>
graph_layout(const std::map<int, btcpp::NodeData>& bt_data) {
    std::map<int, QPointF> positions;

    constexpr auto node_width = 200.;
    constexpr auto node_height = 80.;
    constexpr auto horizontal_margin = 50.;
    constexpr auto vertical_margin = 150.;
    constexpr auto horizontal_spacing = node_width + horizontal_margin;
    constexpr auto vertical_spacing = node_height + vertical_margin;

    const auto layer_widths = compute_layer_widths(bt_data, 0);
    std::println("Layer widths: {}", layer_widths);
    const auto max_width =
        static_cast<double>(std::ranges::max_element(layer_widths)->second);

    const auto graph_width = max_width * horizontal_spacing;
    const auto graph_height =
        static_cast<double>(layer_widths.size()) * vertical_spacing;

    std::map<int, double> index_in_layer;
    auto set_position = [&](this auto& self, int node_id,
                            int current_layer) -> void {
        const auto& node_data = bt_data.at(node_id);

        const auto layer_width =
            static_cast<double>(layer_widths.at(current_layer));

        if (layer_width == max_width) {
            positions[node_id] = QPointF((graph_width * 1. / layer_width *
                                          index_in_layer[current_layer]) +
                                             horizontal_spacing,
                                         current_layer * vertical_spacing);
        } else {
            positions[node_id] =
                QPointF((graph_width * 1. / (1. + layer_width) *
                         (1. + index_in_layer[current_layer])) +
                            (node_width / 2.),
                        current_layer * vertical_spacing);
        }

        for (int child_id : node_data.children) {
            self(child_id, current_layer + 1);
        }

        index_in_layer[current_layer] += 1.;
    };

    set_position(0, 0);

    for (const auto& [node_id, pos] : positions) {
        std::println("Node {}: position = ({}, {})", node_id, pos.x(), pos.y());
    }

    return positions;
}
