export module btcpp:decorators;

import std;
import :node;

export namespace btcpp {

class Decorator : public InternalNode {
public:
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

protected:
    Decorator(std::string_view name) : InternalNode{1, name} {
    }

    template <a_node T>
    Decorator(T&& child, std::string_view name) : InternalNode{1, name} {
        add_child<T>(std::forward<T>(child));
    }
};

class Invert final : public Decorator {
public:
    Invert(std::string_view name = {}) : Decorator{name} {
    }

private:
    [[nodiscard]] State do_tick() final {
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
    Retry(int retries, std::string_view name = {})
        : Decorator{name}, retries_{retries} {
    }

    template <a_node T>
    Retry(T&& child, int retries, std::string_view name = {})
        : Decorator{std::forward<T>(child), name}, retries_{retries} {
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

private:
    [[nodiscard]] State do_tick() final {
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

    int retries_;
    int failures_count_{};
};

} // namespace btcpp