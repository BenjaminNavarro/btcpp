#include <btcpp/btcpp.hpp>
#include <btnet/btnet.hpp>

#include <cstdlib>
#include <ctime>
#include <format>
#include <iostream>
#include <utility>
#include <chrono>
#include <thread>

template <typename... Args>
void println(std::ostream& out, std::format_string<Args...> str,
             Args&&... args) {
    out << std::format(std::move(str), std::forward<Args>(args)...) << '\n';
}

template <typename... Args>
void println(std::format_string<Args...> str, Args&&... args) {
    println(std::cout, std::move(str), std::forward<Args>(args)...);
}

int main() {
    std::srand(std::time(nullptr));

    auto door_state_str = [](bool state) {
        return state ? "opened" : "closed";
    };

    bool first_door_state{false};
    bool second_door_state{false};

    auto tree = btcpp::Fallback{};

    {
        auto& open_and_pass =
            tree.add_child<btcpp::Sequence>("open and pass 1st door");
        auto& open_if =
            open_and_pass.add_child<btcpp::Fallback>("open 1st door if");
        open_and_pass.add_child<btcpp::GenericAction>(
            [] {
                println("Passing 1st door");
                return btcpp::success;
            },
            "pass 1st door");

        open_if.add_child<btcpp::GenericCondition>(
            [&] {
                println("1st door is {}", door_state_str(first_door_state));
                return first_door_state;
            },
            "1st door opened");

        open_if.add_child<btcpp::GenericAction>(
            [] {
                println("Impossible to open 1st door");
                return btcpp::failure;
            },
            "open 1st door");
    }

    {
        auto& open_and_pass =
            tree.add_child<btcpp::Sequence>("open and pass 2nd door");
        auto& open_if =
            open_and_pass.add_child<btcpp::Fallback>("open 2nd door if");
        open_and_pass.add_child<btcpp::GenericAction>(
            [] {
                println("Passing 2nd door");
                return btcpp::success;
            },
            "pass 2nd door");

        open_if.add_child<btcpp::GenericCondition>(
            [&] {
                println("2nd door is {}", door_state_str(second_door_state));
                return second_door_state;
            },
            "2nd door opened");

        open_if.add_child<btcpp::GenericAction>(
            [&, tries = 0]() mutable {
                ++tries;
                if (tries >= 2) {
                    second_door_state = true;
                }
                println("Trying to open the 2nd door (try = {})", tries);
                return btcpp::running;
            },
            "open 2nd door");
    }

    auto bt_pub = btnet::BTPublisher{};

    const auto bytes_sent = bt_pub.send_graph_sync(tree);
    println("{} bytes sent", bytes_sent);

    println("Generated XML:\n{}", btcpp::to_xml(tree));

    int cycle{0};
    do {
        println("Ticking the BT (cycle = {})", cycle);
        if (cycle > 10) {
            println(std::cerr, "BT failed to execute");
            std::exit(1);
        }
        ++cycle;
        bt_pub.send_state(tree);
        std::this_thread::sleep_for(std::chrono::seconds{1});
    } while (tree.tick() != btcpp::success);

    bt_pub.send_state(tree);
}