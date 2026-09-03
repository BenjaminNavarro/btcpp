#pragma once

#include <QtCore/QPointF>

import std;
import btcpp;

std::map<int, QPointF>
graph_layout(const std::map<int, btcpp::NodeData>& bt_data);