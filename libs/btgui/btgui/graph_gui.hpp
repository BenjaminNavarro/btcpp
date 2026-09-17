#pragma once

#include <btcpp/btcpp.hpp>
#include <btgui/graph_layout.hpp>

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>

#include <vector>

namespace btgui {

class BTGui : public QObject {
    Q_OBJECT
public:
    BTGui(NodeStyle style = {});

    void set_node_style(NodeStyle style);

    QGraphicsScene* scene();

public slots:
    void set_graph(btcpp::GraphData data);

    void update_graph_state(const btcpp::GraphData& data);

private:
    void draw_graph();

    void draw_edges();

    void draw_nodes();

    void update_node_state(int id);

    static QString state_color(btcpp::State state);

    static QString node_type_text(int id, const btcpp::NodeData& node_data);

    static QPointF to_qpointf(const auto& pos);

    static QPoint to_qpoint(const auto& pos);

    QGraphicsScene scene_;
    btcpp::GraphData data_;
    std::vector<QWidget*> node_widgets_;
    GraphLayout layout_;
    NodeStyle style_;
};

} // namespace btgui
