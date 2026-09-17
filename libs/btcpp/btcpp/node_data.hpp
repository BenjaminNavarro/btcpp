#pragma once

#include <btcpp/node.hpp>

#include <string>
#include <vector>

namespace btcpp {

struct NodeData {
    std::string type;
    State state;
    std::string name;
    std::vector<int> children;
};

using GraphData = std::vector<NodeData>;

} // namespace btcpp