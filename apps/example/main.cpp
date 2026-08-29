import std;
import btcpp;

int main() {
    std::srand(std::time(nullptr));

    auto is_even = btcpp::GenericCondition{[] {
        if (std::rand() % 2) {
            return btcpp::State::Success;
        } else {
            return btcpp::State::Failure;
        }
    }};

    auto root = btcpp::Sequence{};
    auto fallback = btcpp::Fallback{&root};
    auto sequence = btcpp::Sequence{};
    root.add_child(&is_even);

    std::cout << std::format("{}\n", root.tick());
}