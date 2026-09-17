#include <btcpp/btcpp.hpp>
#include <btgui/btgui.hpp>
#include <btnet/btnet.hpp>

#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QThread>

using namespace std::literals;

class QBTSubscriber : public QObject {
    Q_OBJECT
public:
    QBTSubscriber(btgui::BTGui* bt_gui)
        : sub_{[this](const auto& data) { emit graph_update(data); },
               [this](const auto& data) { emit state_update(data); }} {

        connect(this, &QBTSubscriber::graph_update, bt_gui,
                &btgui::BTGui::set_graph);

        connect(this, &QBTSubscriber::state_update, bt_gui,
                &btgui::BTGui::update_graph_state);
    }

signals:
    void graph_update(const btcpp::GraphData& data);
    void state_update(const btcpp::GraphData& data);

private:
    btnet::BTSubscriber sub_;
};

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    auto bt_gui = btgui::BTGui{};

    auto sub = QBTSubscriber{&bt_gui};

    QGraphicsView view{bt_gui.scene()};
    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowTitle(QStringLiteral("Behavior Tree"));
    view.resize(1200, 900);
    view.show();

    return QApplication::exec();
}

#include "main.moc"