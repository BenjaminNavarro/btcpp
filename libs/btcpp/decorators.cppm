export module btcpp:decorators;

import std;
import :node;

export namespace btcpp {

class Decorator : public InternalNode {
public:
    Decorator() : InternalNode(1) {
    }

    template <a_node T>
    Decorator(T&& child) : InternalNode(1) {
        add_child<T>(std::forward<T>(child));
    }

    Decorator(const Decorator&) = delete;

    virtual ~Decorator() noexcept = default;

    Decorator& operator=(const Decorator&) = delete;
    Decorator& operator=(Decorator&&) noexcept = default;

    [[nodiscard]] const Node* child() const {
        return children().front().get();
    }

    [[nodiscard]] Node* child() {
        return children().front().get();
    }
};

class Invert final : public Decorator {
public:
    using Decorator::Decorator;

    [[nodiscard]] State tick() final {
        switch (child()->tick()) {
        case success:
            return failure;
        case failure:
            return success;
        case running:
            return running;
        }
    }
};

class Retry final : public Decorator {
public:
    Retry(int retries) : retries_{retries} {
    }

    template <a_node T>
    Retry(T&& child, int retries)
        : Decorator{std::forward<T>(child)}, retries_{retries} {
    }

    [[nodiscard]] int retries() const {
        return retries_;
    }

    [[nodiscard]] int failures_count() const {
        return failures_count_;
    }

    void reset() {
        failures_count_ = 0;
    }

    [[nodiscard]] State tick() final {
        switch (child()->tick()) {
        case success:
            reset();
            return success;
        case failure:
            ++failures_count_;
            if (failures_count_ <= retries_) {
                return running;
            } else {
                return failure;
            }
        case running:
            return running;
        }
    }

private:
    int retries_;
    int failures_count_{};
};

} // namespace btcpp