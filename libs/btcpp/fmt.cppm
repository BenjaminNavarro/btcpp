export module btcpp:fmt;

import :node;
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