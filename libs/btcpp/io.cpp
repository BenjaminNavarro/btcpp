#include <btcpp/io.hpp>
#include <btcpp/fmt.hpp>
#include "demangle.hpp"

#include <pugixml.hpp>

#include <sstream>
#include <format>
#include <string_view>
#include <stdexcept>

namespace btcpp {

std::string to_xml(const Node& node, XMLFormat format) {
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
    if (format == formatted) {
        doc.save(oss);
    } else {
        doc.save(oss, "\t", pugi::format_raw);
    }
    return oss.str();
}

GraphData parse_xml(std::string_view xml_string) {
    pugi::xml_document doc;
    pugi::xml_parse_result result =
        doc.load_buffer(xml_string.data(), xml_string.size());

    if (!result) {
        throw std::runtime_error(
            std::format("XML parsing error: {}", result.description()));
    }

    // Start parsing from the root node
    pugi::xml_node root = doc.child("BehaviorTree");

    GraphData node_vec;

    // Count nodes to resize the node vector before using it
    const int node_count = [&] {
        int count{1};
        auto count_node = [&](this auto& self,
                              const pugi::xml_node& xml_node) -> void {
            for (const auto& child_xml_node : xml_node.children("Node")) {
                ++count;
                self(child_xml_node);
            }
        };

        count_node(root.child("Node"));

        return count;
    }();

    node_vec.resize(node_count);

    int current_id{};
    // Recursive function to parse nodes
    auto parse_node = [&](this auto& self,
                          const pugi::xml_node& xml_node) -> void {
        const auto node_id = current_id;
        auto& node_data = node_vec[node_id];
        node_data.type = xml_node.attribute("type").as_string();
        node_data.state = from_string(xml_node.attribute("state").as_string());
        node_data.name = xml_node.attribute("name").as_string();

        for (const auto& child_xml_node : xml_node.children("Node")) {
            ++current_id;
            node_data.children.push_back(current_id);
            self(child_xml_node);
        }
    };

    parse_node(root.child("Node"));

    return node_vec;
}

} // namespace btcpp