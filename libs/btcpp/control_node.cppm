export module btcpp:control_node;

import std;
import :node;

export namespace btcpp {

class ControlNode : public Node {
public:
    using Node::Node;

    [[nodiscard]] const std::vector<Node*>& children() const {
        return children_;
    }

    [[nodiscard]] std::vector<Node*>& children() {
        return children_;
    }

    Node* add_child(Node* node) {
        children_.push_back(node);
        node->set_parent(this);
        return children_.back();
    }

private:
    std::vector<Node*> children_;
};

} // namespace btcpp
