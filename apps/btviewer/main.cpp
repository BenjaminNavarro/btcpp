#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>

import std;
import btcpp;
import btgui;

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

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    auto bt_data = btcpp::parse_xml(bt_xml);

    auto bt_gui = btgui::BTGui{};
    bt_gui.set_graph(bt_data);

    QGraphicsView view{bt_gui.scene()};
    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowTitle(QStringLiteral("Behavior Tree"));
    view.resize(1200, 900);
    view.show();

    return QApplication::exec();
}