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
        auto& open_and_pass = tree.add_child<btcpp::Sequence>();
        auto& open_if = open_and_pass.add_child<btcpp::Fallback>();
        open_and_pass.add_child<btcpp::GenericAction>([] {
            std::println("Passing 1st door");
            return btcpp::success;
        });

        open_if.add_child<btcpp::GenericCondition>([&] {
            std::println("1st door is {}", door_state_str(first_door_state));
            return first_door_state;
        });

        open_if.add_child<btcpp::GenericAction>([] {
            std::println("Impossible to open 1st door");
            return btcpp::failure;
        });
    }

    {
        auto& open_and_pass = tree.add_child<btcpp::Sequence>();
        auto& open_if = open_and_pass.add_child<btcpp::Fallback>();
        open_and_pass.add_child<btcpp::GenericAction>([] {
            std::println("Passing 2nd door");
            return btcpp::success;
        });

        open_if.add_child<btcpp::GenericCondition>([&] {
            std::println("2nd door is {}", door_state_str(second_door_state));
            return second_door_state;
        });

        open_if.add_child<btcpp::GenericAction>([&, tries = 0] mutable {
            ++tries;
            if (tries >= 2) {
                second_door_state = true;
            }
            std::println("Trying to open the 2nd door (try = {})", tries);
            return btcpp::running;
        });
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
}