module;

#include <zmq.hpp>
#include <zmq_addon.hpp>

export module btnet;

import std;
import btcpp;

export namespace btnet {

class BTPublisher {
public:
    BTPublisher(const std::string& endpoint = "tcp://localhost:8521")
        : socket_{ctx_, zmq::socket_type::pub} {
        using namespace std::literals;
        socket_.bind(endpoint);
        bind_time_ = std::chrono::steady_clock::now();
    }

    std::size_t send_graph(const btcpp::Node& root_node) {
        socket_.send(zmq::str_buffer("G"), zmq::send_flags::sndmore);
        return send(root_node, zmq::send_flags::dontwait);
    }

    std::size_t send_graph_sync(const btcpp::Node& root_node) {
        return send_sync("G", root_node);
    }

    std::size_t send_state(const btcpp::Node& root_node) {
        socket_.send(zmq::str_buffer("S"), zmq::send_flags::sndmore);
        return send(root_node, zmq::send_flags::dontwait);
    }

    std::size_t send_state_sync(const btcpp::Node& root_node) {
        return send_sync("S", root_node);
    }

private:
    std::size_t send_sync(std::string_view topic,
                          const btcpp::Node& root_node) {
        // Sending data too soon after a bind may fail silently, so wait a bit
        // before sending the first message
        std::this_thread::sleep_until(bind_time_ +
                                      std::chrono::milliseconds{500});
        socket_.send(zmq::const_buffer{topic.data(), topic.size()},
                     zmq::send_flags::sndmore);
        return send(root_node, zmq::send_flags::none);
    }

    std::size_t send(const btcpp::Node& root_node, zmq::send_flags flags) {
        const auto as_xml = btcpp::to_xml(root_node, btcpp::unformatted);
        const auto res =
            socket_.send(zmq::message_t{as_xml}, zmq::send_flags::dontwait);
        return res.value_or(0);
    }

    zmq::context_t ctx_;
    zmq::socket_t socket_;
    std::chrono::steady_clock::time_point bind_time_;
};

template <typename F>
concept bt_subscriber_callback =
    std::is_invocable_r_v<void, F, const btcpp::GraphData&>;

class BTSubscriber {
public:
    template <bt_subscriber_callback OnGraphUpdate,
              bt_subscriber_callback OnStateUpdate>
    BTSubscriber(OnGraphUpdate&& on_graph_update, OnStateUpdate on_state_update,
                 const std::string& endpoint = "tcp://localhost:8521")
        : socket_{ctx_, zmq::socket_type::sub} {
        socket_.connect(endpoint);

        // Full graphs
        socket_.set(zmq::sockopt::subscribe, "G");
        // Graph state update
        socket_.set(zmq::sockopt::subscribe, "S");

        reception_thread_ = std::thread{[this, on_graph_update,
                                         on_state_update] {
            while (true) {
                std::vector<zmq::message_t> recv_msgs;
                const auto ret =
                    zmq::recv_multipart(socket_, std::back_inserter(recv_msgs),
                                        zmq::recv_flags::none);
                if (stop_reception_) {
                    break;
                }
                if (ret) {
                    if (recv_msgs.size() != 2) {
                        std::println(
                            stderr,
                            "Incorrect number of messages received. Expecting "
                            "2, got {}",
                            recv_msgs.size());
                        continue;
                    }

                    const auto& topic = recv_msgs[0].to_string_view();
                    if (topic.size() != 1) {
                        std::println(
                            stderr,
                            "Incorrect topic length. Expecting 1, got {}",
                            topic.size());
                        continue;
                    }

                    const auto& data_xml = recv_msgs[1].to_string_view();
                    const auto data = btcpp::parse_xml(data_xml);

                    switch (topic[0]) {
                    case 'G':
                        on_graph_update(data);
                        break;
                    case 'S':
                        on_state_update(data);
                        break;
                    default:
                        std::println(
                            stderr,
                            "Incorrect topic. Expecting 'G' or 'S', got {}",
                            topic.size());
                        continue;
                    }
                }
            }
        }};
    }

    ~BTSubscriber() {
        stop_reception_ = true;
        socket_.close();
        reception_thread_.join();
    }

    BTSubscriber(const BTSubscriber&) = delete;
    BTSubscriber(BTSubscriber&&) = delete;

    BTSubscriber& operator=(const BTSubscriber&) = delete;
    BTSubscriber& operator=(BTSubscriber&&) = delete;

private:
    zmq::context_t ctx_;
    zmq::socket_t socket_;
    std::atomic<bool> stop_reception_{false};
    std::thread reception_thread_;
};

} // namespace btnet