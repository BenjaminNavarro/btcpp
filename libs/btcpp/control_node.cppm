export module btcpp:control_node;

import std;
import :node;

export namespace btcpp {

class ControlNode : public InternalNode {
protected:
    using InternalNode::InternalNode;
};

} // namespace btcpp
