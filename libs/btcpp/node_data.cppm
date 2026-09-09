export module btcpp:node_data;

import std;
import :node;

export namespace btcpp {

struct NodeData {
    std::string type;
    State state;
    std::string name;
    std::vector<int> children;
};

using GraphData = std::vector<NodeData>;

} // namespace btcpp