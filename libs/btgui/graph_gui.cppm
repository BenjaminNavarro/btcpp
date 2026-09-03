module;

#include <QtGui/QPainterPath>
#include <QtGui/QPen>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPathItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

export module btgui:gui;

import std;
import btcpp;
import :layout;

using namespace std::literals;

export namespace btgui {

class BTGui {
public:
    BTGui(NodeStyle style = {}) : style_{style} {
    }

    void set_graph(btcpp::GraphData data) {
        data_ = std::move(data);
        node_widgets_.clear();
        node_widgets_.resize(data_.size());
        draw_graph();
    }

    void set_node_style(NodeStyle style) {
        style_ = style;
        draw_graph();
    }

    void update_graph_state(const btcpp::GraphData& data) {
        // TODO
    }

    QGraphicsScene* scene() {
        return &scene_;
    }

private:
    void draw_graph() {
        layout_ = compute_graph_layout(data_, style_);
        draw_edges();
        draw_nodes();
    }

    void draw_edges() {
        for (int id = 0; const auto& node_data : data_) {
            const auto parent_position = to_qpointf(layout_.at(id));
            for (const int child_id : node_data.children) {
                const auto start = parent_position +
                                   QPointF{style_.width / 2.0,
                                           static_cast<double>(style_.height)};
                const auto child_position = to_qpointf(layout_.at(child_id));
                const auto end =
                    child_position + QPointF{style_.width / 2.0, 0.0};
                const auto midpoint_y = (start.y() + end.y()) / 2.0;
                QPainterPath path{start};
                path.cubicTo(QPointF{start.x(), midpoint_y},
                             QPointF{end.x(), midpoint_y}, end);
                auto* edge = scene_.addPath(path, QPen{QColor{"#607080"}, 2.0});
                edge->setZValue(-1.0);
            }
            ++id;
        }
    }

    void draw_nodes() {
        for (int id = 0; const auto& node_data : data_) {
            const auto position = to_qpoint(layout_.at(id));

            auto* widget = new QWidget{};
            node_widgets_[id] = widget;
            widget->move(position);
            widget->resize(style_.width, style_.height);

            update_node_state(id);

            auto* vbox_layout = new QVBoxLayout{widget};

            auto add_label = [&](const auto& text) {
                auto label_style = QStringLiteral(
                    "color: #17212b; font-size: 14px; border: none;");
                auto* label = new QLabel{};
                label->setText(text);
                label->setStyleSheet(label_style);
                label->setAlignment(Qt::AlignCenter);

                vbox_layout->addWidget(label);
            };

            add_label(node_type_text(id, node_data));

            if (not node_data.name.empty()) {
                add_label(QString::fromStdString(node_data.name));
            }

            scene_.addWidget(widget);

            ++id;
        }
    }

    void update_node_state(int id) {
        node_widgets_.at(id)->setStyleSheet(
            QString("background-color: %1; border: 1px solid #000000")
                .arg(state_color(data_.at(id).state)));
    }

    static QString state_color(btcpp::State state) {
        switch (state) {
        case btcpp::success:
            return "#b7e4c7";
        case btcpp::failure:
            return "#f7b2ad";
        case btcpp::running:
            return "#ffe8a1";
        }
        return "#ffffff";
    }

    static QString node_type_text(int id, const btcpp::NodeData& node_data) {
        auto type = node_data.type;

        // Remove btcpp namespace from type (not needed for core types)
        constexpr auto prefix = "btcpp::"sv;
        if (type.starts_with(prefix)) {
            type = type.substr(prefix.size());
        }

        // remove possible module name at the end
        type = type.substr(0, type.rfind('@'));

        return QString::fromStdString(std::format("[{} {}]", id, type));
    }

    static QPointF to_qpointf(const auto& pos) {
        return {static_cast<double>(pos.x), static_cast<double>(pos.y)};
    };

    static QPoint to_qpoint(const auto& pos) {
        return {static_cast<int>(pos.x), static_cast<int>(pos.y)};
    };

    QGraphicsScene scene_;
    btcpp::GraphData data_;
    std::vector<QWidget*> node_widgets_;
    GraphLayout layout_;
    NodeStyle style_;
};

} // namespace btgui