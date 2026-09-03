export module btcpp:node;

import std;

export namespace btcpp {

enum class State : std::uint8_t { Success, Failure, Running };

constexpr State success = State::Success;
constexpr State failure = State::Failure;
constexpr State running = State::Running;

State from_string(std::string_view state_str) {
    if (state_str == "success") {
        return success;
    } else if (state_str == "failure") {
        return failure;
    } else if (state_str == "running") {
        return running;
    } else {
        throw std::invalid_argument{
            std::format("Invalid state string: {}", state_str)};
    }
}

class Node {
public:
    Node() = default;

    Node(const Node&) = delete;
    Node(Node&&) noexcept = default;

    virtual ~Node() noexcept = default;

    Node& operator=(const Node&) = delete;
    Node& operator=(Node&&) noexcept = default;

    [[nodiscard]] State state() const {
        return state_;
    }

    [[nodiscard]] State tick() {
        state_ = do_tick();
        return state_;
    }

protected:
    [[nodiscard]] virtual State do_tick() = 0;

private:
    State state_{State::Success};
};

class LeafNode : public Node {};

template <typename T>
concept a_node = std::is_base_of_v<Node, std::remove_cvref_t<T>>;

class InternalNode : public Node {
public:
    template <a_node T, typename... Args>
    T& add_child(Args&&... args) {
        if (max_children_count_ > 0 and
            children_.size() == max_children_count_) {
            throw std::logic_error{
                std::format("Maximum number of children reached ({}), can't "
                            "add a new one",
                            max_children_count_)};
        }

        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        auto* child_ptr = child.get();
        children_.push_back(std::move(child));
        return *child_ptr;
    }

    template <a_node... Ts>
    auto& add_children(Ts&&... children) {
        (add_child<Ts>(std::forward<Ts>(children)), ...);
        return children_;
    }

    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& children() const {
        return children_;
    }

    [[nodiscard]] int max_children_count() const {
        return max_children_count_;
    }

protected:
    InternalNode() = default;

    InternalNode(int max_children_count)
        : max_children_count_{max_children_count} {
        if (max_children_count_ < 0) {
            throw std::logic_error{
                std::format("Maximum number of children ({}) can't be negative",
                            max_children_count_)};
        }
    }

private:
    //! \brief Maximum number of children allowed. Zero means infinite
    //!
    int max_children_count_{};
    std::vector<std::unique_ptr<Node>> children_;
};

} // namespace btcpp
