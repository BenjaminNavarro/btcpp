module;

export module btcpp:execution_node;
import :node;

export namespace btcpp {

class ExecutionNode : public LeafNode {};

class Action : public ExecutionNode {};

class Condition : public ExecutionNode {};

} // namespace btcpp
