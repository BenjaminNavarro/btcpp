export module btcpp:fmt;

import :node;
import :node_data;
import std;

template <>
struct std::formatter<btcpp::State> {
    static constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const btcpp::State& state,
                       std::format_context& context) {
        switch (state) {
        case btcpp::State::Success:
            return std::format_to(context.out(), "success");
            break;
        case btcpp::State::Failure:
            return std::format_to(context.out(), "failure");
            break;
        case btcpp::State::Running:
            return std::format_to(context.out(), "running");
            break;
        }
    }
};

template <>
struct std::formatter<btcpp::NodeData> {
    static constexpr auto parse(std::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const btcpp::NodeData& data,
                       std::format_context& context) {
        auto out = std::format_to(context.out(), "{} @ {} - {}", data.name,
                                  data.type, data.state);
        if (not data.children.empty()) {
            return std::format_to(context.out(), " {}", data.children);
        }
        return out;
    }
};