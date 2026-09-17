#pragma once

#include <btcpp/btcpp.hpp>

#include <zmq.hpp>

#include <atomic>
#include <cstddef>
#include <chrono>
#include <functional>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>

namespace btnet {

class BTPublisher {
public:
    BTPublisher(const std::string& endpoint = "tcp://localhost:8521");

    std::size_t send_graph(const btcpp::Node& root_node);

    std::size_t send_graph_sync(const btcpp::Node& root_node);

    std::size_t send_state(const btcpp::Node& root_node);

    std::size_t send_state_sync(const btcpp::Node& root_node);

private:
    std::size_t send_sync(std::string_view topic, const btcpp::Node& root_node);

    std::size_t send(const btcpp::Node& root_node, zmq::send_flags flags);

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

        reception_thread_ =
            std::thread{[this, on_graph_update, on_state_update] {
                reception_thread(on_graph_update, on_state_update);
            }};
    }

    ~BTSubscriber();

    BTSubscriber(const BTSubscriber&) = delete;
    BTSubscriber(BTSubscriber&&) = delete;

    BTSubscriber& operator=(const BTSubscriber&) = delete;
    BTSubscriber& operator=(BTSubscriber&&) = delete;

private:
    void reception_thread(
        std::function<void(const btcpp::GraphData&)> on_graph_update,
        std::function<void(const btcpp::GraphData&)> on_state_update);

    zmq::context_t ctx_;
    zmq::socket_t socket_;
    std::atomic<bool> stop_reception_{false};
    std::thread reception_thread_;
};

} // namespace btnet