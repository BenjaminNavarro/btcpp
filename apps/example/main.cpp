import std;
import btcpp;

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
                std::println("Passing 1st door");
                return btcpp::success;
            },
            "pass 1st door");

        open_if.add_child<btcpp::GenericCondition>(
            [&] {
                std::println("1st door is {}",
                             door_state_str(first_door_state));
                return first_door_state;
            },
            "1st door opened");

        open_if.add_child<btcpp::GenericAction>(
            [] {
                std::println("Impossible to open 1st door");
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
                std::println("Passing 2nd door");
                return btcpp::success;
            },
            "pass 2nd door");

        open_if.add_child<btcpp::GenericCondition>(
            [&] {
                std::println("2nd door is {}",
                             door_state_str(second_door_state));
                return second_door_state;
            },
            "2nd door opened");

        open_if.add_child<btcpp::GenericAction>(
            [&, tries = 0] mutable {
                ++tries;
                if (tries >= 2) {
                    second_door_state = true;
                }
                std::println("Trying to open the 2nd door (try = {})", tries);
                return btcpp::running;
            },
            "open 2nd door");
    }

    std::println("Generated XML:\n{}", btcpp::to_xml(tree));

    int cycle{0};
    do {
        std::println("Ticking the BT (cycle = {})", cycle);
        if (cycle > 10) {
            std::println(std::cerr, "BT failed to execute");
            std::exit(1);
        }
        ++cycle;
    } while (tree.tick() != btcpp::success);

    std::println("Generated XML:\n{}", btcpp::to_xml(tree));

    const auto bt_data = btcpp::parse_xml(btcpp::to_xml(tree));
    for (const auto& [id, node_data] : bt_data) {
        std::println("Node ID: {}, Type: {}, State: {}, Children: {}", id,
                     node_data.type, node_data.state, node_data.children);
    }
}