import std;
import btcpp;

int main() {
    std::srand(std::time(nullptr));

    auto door_state_str = [](bool state) {
        return state ? "opened" : "closed";
    };

    bool first_door_state{false};
    bool second_door_state{false};
    auto is_1st_door_open = btcpp::GenericCondition{[&] {
        std::println("1st door is {}", door_state_str(first_door_state));
        return first_door_state;
    }};

    auto is_2nd_door_open = btcpp::GenericCondition{[&] {
        std::println("2nd door is {}", door_state_str(second_door_state));
        return second_door_state;
    }};

    auto open_1st_door = btcpp::GenericAction{[] {
        std::println("Impossible to open 1st door");
        return btcpp::failure;
    }};

    auto open_2nd_door = btcpp::GenericAction{[&, tries = 0] mutable {
        ++tries;
        if (tries >= 3) {
            second_door_state = true;
        }
        std::println("Trying to open the 2nd door (try = {})", tries);
        return btcpp::running;
    }};

    auto pass_1st_door = btcpp::GenericAction{[] {
        std::println("Passing 1st door");
        return btcpp::success;
    }};

    auto pass_2nd_door = btcpp::GenericAction{[] {
        std::println("Passing 2nd door");
        return btcpp::success;
    }};

    auto open_1st_door_if = btcpp::Fallback{};
    open_1st_door_if.add_child(&is_1st_door_open);
    open_1st_door_if.add_child(&open_1st_door);

    auto open_and_pass_1st_door = btcpp::Sequence{};
    open_and_pass_1st_door.add_child(&open_1st_door_if);
    open_and_pass_1st_door.add_child(&pass_1st_door);

    auto open_2nd_door_if = btcpp::Fallback{};
    open_2nd_door_if.add_child(&is_2nd_door_open);
    open_2nd_door_if.add_child(&open_2nd_door);

    auto open_and_pass_2nd_door = btcpp::Sequence{};
    open_and_pass_2nd_door.add_child(&open_2nd_door_if);
    open_and_pass_2nd_door.add_child(&pass_2nd_door);

    auto bt = btcpp::Fallback{};
    bt.add_child(&open_and_pass_1st_door);
    bt.add_child(&open_and_pass_2nd_door);

    int cycle{0};
    do {
        std::println("Ticking the BT (cycle = {})", cycle);
        if (cycle > 10) {
            std::println(std::cerr, "BT failed to execute");
            std::exit(1);
        }
        ++cycle;
    } while (bt.tick() != btcpp::success);
}