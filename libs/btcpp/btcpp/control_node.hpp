#pragma once

#include <btcpp/node.hpp>

namespace btcpp {

class ControlNode : public InternalNode {
protected:
    using InternalNode::InternalNode;
};

} // namespace btcpp
