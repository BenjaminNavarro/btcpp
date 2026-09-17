#pragma once

#include <btcpp/node.hpp>

namespace btcpp {

class ExecutionNode : public LeafNode {
protected:
    using LeafNode::LeafNode;
};

class Action : public ExecutionNode {
protected:
    using ExecutionNode::ExecutionNode;
};

class Condition : public ExecutionNode {
protected:
    using ExecutionNode::ExecutionNode;
};

} // namespace btcpp
