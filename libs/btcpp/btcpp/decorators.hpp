#pragma once

#include <btcpp/node.hpp>

namespace btcpp {

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
    Decorator(std::string_view name);

    template <a_node T>
    Decorator(T&& child, std::string_view name) : InternalNode{1, name} {
        add_child<T>(std::forward<T>(child));
    }
};

class Invert final : public Decorator {
public:
    Invert(std::string_view name = {});

    template <a_node T>
    Invert(T&& child)
        : Decorator{std::forward<T>(child),
                    std::format("{} invert", child.name())} {
    }

    template <a_node T>
    Invert(T&& child, std::string_view name)
        : Decorator{std::forward<T>(child), name} {
    }

private:
    [[nodiscard]] State do_tick() final;
};

class Retry final : public Decorator {
public:
    Retry(int retries, std::string_view name = {});

    template <a_node T>
    Retry(T&& child, int retries)
        : Decorator{std::forward<T>(child),
                    std::format("{} retry", child.name())},
          retries_{retries} {
    }
    template <a_node T>
    Retry(T&& child, int retries, std::string_view name)
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
    [[nodiscard]] State do_tick() final;

    int retries_;
    int failures_count_{};
};

} // namespace btcpp