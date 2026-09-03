module;

#include <pugixml.hpp>

export module btcpp:io;

import std;
import :node;
import :fmt;
import :demangle;

export namespace btcpp {

//! \brief Serialize a behavior tree starting at the given node to XML format.
//!
//! \param node Root node of the tree
//! \return std::string XML representation of the tree
std::string to_xml(const Node& node) {
    pugi::xml_document doc;

    // Create a root node in the XML document
    pugi::xml_node root = doc.append_child("BehaviorTree");

    // Recursive function to serialize nodes
    auto serialize_node = [&](this auto& self, const Node& node,
                              pugi::xml_node& xml_node) -> void {
        // Create an XML node for the current behavior tree node
        pugi::xml_node child_xml_node = xml_node.append_child("Node");
        child_xml_node.append_attribute("type") = demangle(typeid(node).name());
        child_xml_node.append_attribute("state") =
            std::format("{}", node.state());
        child_xml_node.append_attribute("name") =
            std::format("{}", node.name());

        // Serialize children if it's an internal node
        if (const auto* internal_node =
                dynamic_cast<const InternalNode*>(&node)) {
            for (const auto& child : internal_node->children()) {
                self(*child, child_xml_node);
            }
        }
    };

    // Start serialization from the root node
    serialize_node(node, root);

    // Convert the XML document to a string
    std::ostringstream oss;
    doc.save(oss);
    return oss.str();
}

struct NodeData {
    std::string type;
    State state;
    std::string name;
    std::vector<int> children;
};

std::map<int, NodeData> parse_xml(const std::string& xml_string) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());

    if (!result) {
        throw std::runtime_error(
            std::format("XML parsing error: {}", result.description()));
    }

    std::map<int, NodeData> node_map;
    int current_id{};

    // Recursive function to parse nodes
    auto parse_node = [&](this auto& self,
                          const pugi::xml_node& xml_node) -> void {
        const auto node_id = current_id;
        auto& node_data = node_map[node_id];
        node_data.type = xml_node.attribute("type").as_string();
        node_data.state = from_string(xml_node.attribute("state").as_string());
        node_data.name = xml_node.attribute("name").as_string();

        for (const auto& child_xml_node : xml_node.children("Node")) {
            ++current_id;
            node_data.children.push_back(current_id);
            self(child_xml_node);
        }
    };

    // Start parsing from the root node
    pugi::xml_node root = doc.child("BehaviorTree");
    parse_node(root.child("Node"));

    return node_map;
}

} // namespace btcpp