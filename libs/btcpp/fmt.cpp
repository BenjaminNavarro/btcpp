#include <btcpp/fmt.hpp>

std::format_context::iterator
std::formatter<btcpp::State>::format(const btcpp::State& state,
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

std::format_context::iterator
std::formatter<btcpp::NodeData>::format(const btcpp::NodeData& data,
                                        std::format_context& context) {
    auto out = std::format_to(context.out(), "{} @ {} - {}", data.name,
                              data.type, data.state);
    if (not data.children.empty()) {
        return std::format_to(context.out(), " {}", data.children);
    }
    return out;
}
