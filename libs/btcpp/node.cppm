export module btcpp:node;

import std;

export namespace btcpp {

enum class State : std::uint8_t { Success, Failure, Running };

constexpr State success = State::Success;
constexpr State failure = State::Failure;
constexpr State running = State::Running;

class Node {
public:
    Node() = default;

    Node(const Node&) = delete;

    virtual ~Node() noexcept = default;

    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) noexcept = default;

    [[nodiscard]] const Node* parent() const {
        return parent_;
    }

    void set_parent(Node* parent) {
        parent_ = parent;
    }

    [[nodiscard]] State state() const {
        return state_;
    }

    [[nodiscard]] virtual State tick() = 0;

private:
    Node* parent_{nullptr};
    State state_{State::Success};
};

} // namespace btcpp
