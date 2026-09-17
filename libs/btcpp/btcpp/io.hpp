#pragma once

#include <btcpp/node.hpp>
#include <btcpp/node_data.hpp>

namespace btcpp {

enum class XMLFormat { Formatted, Unformatted };

constexpr XMLFormat formatted = XMLFormat::Formatted;
constexpr XMLFormat unformatted = XMLFormat::Unformatted;

//! \brief Serialize a behavior tree starting at the given node to XML format.
//!
//! \param node Root node of the tree
//! \param formatted [true] Whether to format the XML representation or leave it
//! raw
//! \return std::string XML representation of the tree
std::string to_xml(const Node& node, XMLFormat format = formatted);

GraphData parse_xml(std::string_view xml_string);

} // namespace btcpp