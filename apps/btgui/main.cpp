#include <QtGui/QScreen>
#include <QtGui/QBrush>
#include <QtGui/QPainterPath>
#include <QtGui/QPen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsPathItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsView>

#include "../btgui/graph_layout.hpp"

import std;
import btcpp;

constexpr auto bt_xml = R"(
<?xml version="1.0"?>
<BehaviorTree>
	<Node type="btcpp::Fallback@btcpp" state="success">
		<Node type="btcpp::Sequence@btcpp" state="failure">
			<Node type="btcpp::Fallback@btcpp" state="failure">
				<Node type="btcpp::GenericCondition@btcpp" state="failure" />
				<Node type="btcpp::GenericAction@btcpp" state="failure" />
			</Node>
			<Node type="btcpp::GenericAction@btcpp" state="success" />
		</Node>
		<Node type="btcpp::Sequence@btcpp" state="success">
			<Node type="btcpp::Fallback@btcpp" state="success">
				<Node type="btcpp::GenericCondition@btcpp" state="success" />
				<Node type="btcpp::GenericAction@btcpp" state="running" />
			</Node>
			<Node type="btcpp::GenericAction@btcpp" state="success" />
		</Node>
	</Node>
</BehaviorTree>
)";

namespace {

constexpr double node_width = 200.0;
constexpr double node_height = 80.0;

QColor state_color(btcpp::State state) {
    switch (state) {
    case btcpp::success:
        return QColor{"#b7e4c7"};
    case btcpp::failure:
        return QColor{"#f7b2ad"};
    case btcpp::running:
        return QColor{"#ffe8a1"};
    }
    return Qt::white;
}

QString state_text(btcpp::State state) {
    switch (state) {
    case btcpp::success:
        return QStringLiteral("success");
    case btcpp::failure:
        return QStringLiteral("failure");
    case btcpp::running:
        return QStringLiteral("running");
    }
    return {};
}

QString node_text(int id, const btcpp::NodeData& node_data) {
    return QStringLiteral("%1\nnode %2 | %3")
        .arg(QString::fromStdString(node_data.type), QString::number(id),
             state_text(node_data.state));
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    const auto bt_data = btcpp::parse_xml(bt_xml);
    for (const auto& [id, node] : bt_data) {
        std::println("  - Node {} ({}): {}", id, node.type, node.children);
    }

    const auto positions = graph_layout(bt_data);
    QGraphicsScene scene;
    scene.setBackgroundBrush(QColor{"#f4f6f8"});

    // Draw edges first so nodes remain visually in the foreground.
    for (const auto& [id, node_data] : bt_data) {
        const auto parent_position = positions.at(id);
        for (const int child_id : node_data.children) {
            const auto child_position = positions.at(child_id);
            const auto start =
                parent_position + QPointF{node_width / 2.0, node_height};
            const auto end = child_position + QPointF{node_width / 2.0, 0.0};
            const auto midpoint_y = (start.y() + end.y()) / 2.0;
            QPainterPath path{start};
            path.cubicTo(QPointF{start.x(), midpoint_y},
                         QPointF{end.x(), midpoint_y}, end);
            auto* edge = scene.addPath(path, QPen{QColor{"#607080"}, 2.0});
            edge->setZValue(-1.0);
        }
    }

    for (const auto& [id, node_data] : bt_data) {
        const auto position = positions.at(id);
        scene.addRect(QRectF{position, QSizeF{node_width, node_height}},
                      QPen{QColor{"#263238"}, 2.0},
                      QBrush{state_color(node_data.state)});

        auto* label = scene.addText(node_text(id, node_data));
        label->setDefaultTextColor(QColor{"#17212b"});
        label->setFont(QFont{"Sans Serif", 11});
        label->setTextWidth(node_width - 20.0);
        label->setPos(position + QPointF{10.0, 10.0});
    }

    QGraphicsView view{&scene};
    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowTitle(QStringLiteral("Behavior Tree"));
    view.resize(1200, 900);
    view.show();

    return QApplication::exec();
}