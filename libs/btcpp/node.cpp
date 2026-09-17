#include <btcpp/node.hpp>

namespace btcpp {

State from_string(std::string_view state_str) {
    if (state_str == "success") {
        return success;
    } else if (state_str == "failure") {
        return failure;
    } else if (state_str == "running") {
        return running;
    } else {
        throw std::invalid_argument{
            std::format("Invalid state string: {}", state_str)};
    }
}

Node::Node(std::string_view name) : name_{name} {
}

State Node::tick() {
    state_ = do_tick();
    return state_;
}

InternalNode::InternalNode(std::string_view name) : Node{name} {
}

InternalNode::InternalNode(int max_children_count, std::string_view name)
    : Node{name}, max_children_count_{max_children_count} {
    if (max_children_count_ < 0) {
        throw std::logic_error{
            std::format("Maximum number of children ({}) can't be negative",
                        max_children_count_)};
    }
}

} // namespace btcpp
