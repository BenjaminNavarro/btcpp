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
    std::function<void(const Node&, pugi::xml_node&)> serialize_node;
    serialize_node = [&](const Node& node, pugi::xml_node& xml_node) {
        // Create an XML node for the current behavior tree node
        pugi::xml_node child_xml_node = xml_node.append_child("Node");
        child_xml_node.append_attribute("type") = demangle(typeid(node).name());
        child_xml_node.append_attribute("state") =
            std::format("{}", node.state());

        // Serialize children if it's an internal node
        if (const auto* internal_node =
                dynamic_cast<const InternalNode*>(&node)) {
            for (const auto& child : internal_node->children()) {
                serialize_node(*child, child_xml_node);
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

} // namespace btcpp