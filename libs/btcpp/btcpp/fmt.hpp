#pragma once

#include <btcpp/node.hpp>
#include <btcpp/node_data.hpp>

template <>
struct std::formatter<btcpp::State> {
    static constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }

    static std::format_context::iterator format(const btcpp::State& state,
                                                std::format_context& context);
};

template <>
struct std::formatter<btcpp::NodeData> {
    static constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }

    static std::format_context::iterator format(const btcpp::NodeData& data,
                                                std::format_context& context);
};