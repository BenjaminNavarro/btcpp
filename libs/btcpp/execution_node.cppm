module;

export module btcpp:execution_node;
import :node;

export namespace btcpp {

class ExecutionNode : public Node {
public:
    using Node::Node;

private:
};

class Action : public ExecutionNode {
public:
    using ExecutionNode::ExecutionNode;

private:
};

class Condition : public ExecutionNode {
public:
    using ExecutionNode::ExecutionNode;

private:
};

} // namespace btcpp
