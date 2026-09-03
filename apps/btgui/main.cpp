#include <QtGui/QScreen>
#include <QtGui/QBrush>
#include <QtGui/QPainterPath>
#include <QtGui/QPen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsPathItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include "../btgui/graph_layout.hpp"

import std;
import btcpp;

using namespace std::literals;

constexpr auto bt_xml = R"(
<?xml version="1.0"?>
<BehaviorTree>
	<Node type="btcpp::Fallback@btcpp" state="success" name="">
		<Node type="btcpp::Sequence@btcpp" state="failure" name="open and pass 1st door">
			<Node type="btcpp::Fallback@btcpp" state="failure" name="open 1st door if">
				<Node type="btcpp::GenericCondition@btcpp" state="failure" name="1st door opened" />
				<Node type="btcpp::GenericAction@btcpp" state="failure" name="open 1st door" />
			</Node>
			<Node type="btcpp::GenericAction@btcpp" state="success" name="pass 1st door" />
		</Node>
		<Node type="btcpp::Sequence@btcpp" state="success" name="open and pass 2nd door">
			<Node type="btcpp::Fallback@btcpp" state="success" name="open 2nd door if">
				<Node type="btcpp::GenericCondition@btcpp" state="success" name="2nd door opened" />
				<Node type="btcpp::GenericAction@btcpp" state="running" name="open 2nd door" />
			</Node>
			<Node type="btcpp::GenericAction@btcpp" state="success" name="pass 2nd door" />
		</Node>
	</Node>
</BehaviorTree>

)";

namespace {

constexpr double node_width = 200.0;
constexpr double node_height = 80.0;

QString state_color(btcpp::State state) {
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

QColor state_qcolor(btcpp::State state) {
    return QColor{state_color(state)};
}

QString node_type_text(int id, const btcpp::NodeData& node_data) {
    auto type = node_data.type;

    // Remove btcpp namespace from type (not needed for core types)
    constexpr auto prefix = "btcpp::"sv;
    if (type.starts_with(prefix)) {
        type = type.substr(prefix.size());
    }

    // remove possible module name at the end
    type = type.substr(0, type.rfind('@'));

    return QStringLiteral("[%1] %2").arg(QString::number(id),
                                         QString::fromStdString(type));
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    const auto bt_data = btcpp::parse_xml(bt_xml);

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

        auto* widget = new QWidget{};
        widget->move(position);
        widget->resize(node_width, node_height);

        widget->setStyleSheet(
            QString("background-color: %1; border: 1px solid #000000")
                .arg(state_color(node_data.state)));

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

        scene.addWidget(widget);
    }

    QGraphicsView view{&scene};
    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowTitle(QStringLiteral("Behavior Tree"));
    view.resize(1200, 900);
    view.show();

    return QApplication::exec();
}