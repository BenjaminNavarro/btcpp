#include <btnet/btnet.hpp>

#include <zmq_addon.hpp>

#include <iostream>
#include <format>

namespace btnet {

BTPublisher::BTPublisher(const std::string& endpoint)
    : socket_{ctx_, zmq::socket_type::pub} {
    using namespace std::literals;
    socket_.bind(endpoint);
    bind_time_ = std::chrono::steady_clock::now();
}

std::size_t BTPublisher::send_graph(const btcpp::Node& root_node) {
    socket_.send(zmq::str_buffer("G"), zmq::send_flags::sndmore);
    return send(root_node, zmq::send_flags::dontwait);
}

std::size_t BTPublisher::send_graph_sync(const btcpp::Node& root_node) {
    return send_sync("G", root_node);
}

std::size_t BTPublisher::send_state(const btcpp::Node& root_node) {
    socket_.send(zmq::str_buffer("S"), zmq::send_flags::sndmore);
    return send(root_node, zmq::send_flags::dontwait);
}

std::size_t BTPublisher::send_state_sync(const btcpp::Node& root_node) {
    return send_sync("S", root_node);
}

std::size_t BTPublisher::send_sync(std::string_view topic,
                                   const btcpp::Node& root_node) {
    // Sending data too soon after a bind may fail silently, so wait a bit
    // before sending the first message
    std::this_thread::sleep_until(bind_time_ + std::chrono::milliseconds{500});
    socket_.send(zmq::const_buffer{topic.data(), topic.size()},
                 zmq::send_flags::sndmore);
    return send(root_node, zmq::send_flags::none);
}

std::size_t BTPublisher::send(const btcpp::Node& root_node,
                              zmq::send_flags flags) {
    const auto as_xml = btcpp::to_xml(root_node, btcpp::unformatted);
    const auto res =
        socket_.send(zmq::message_t{as_xml}, zmq::send_flags::dontwait);
    return res.value_or(0);
}

BTSubscriber::~BTSubscriber() {
    stop_reception_ = true;
    socket_.close();
    reception_thread_.join();
}

void BTSubscriber::reception_thread(
    std::function<void(const btcpp::GraphData&)> on_graph_update,
    std::function<void(const btcpp::GraphData&)> on_state_update) {
    while (true) {
        std::vector<zmq::message_t> recv_msgs;
        const auto ret = zmq::recv_multipart(
            socket_, std::back_inserter(recv_msgs), zmq::recv_flags::none);
        if (stop_reception_) {
            break;
        }
        if (ret) {
            if (recv_msgs.size() != 2) {
                std::cerr << std::format(
                    "Incorrect number of messages received. Expecting "
                    "2, got {}\n",
                    recv_msgs.size());
                continue;
            }

            const auto& topic = recv_msgs[0].to_string_view();
            if (topic.size() != 1) {
                std::cerr << std::format(
                    "Incorrect topic length. Expecting 1, got {}\n",
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
                std::cerr << std::format(
                    "Incorrect topic. Expecting 'G' or 'S', got {}\n",
                    topic.size());
                continue;
            }
        }
    }
}

} // namespace btnet