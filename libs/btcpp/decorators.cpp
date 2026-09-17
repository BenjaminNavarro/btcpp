#include <btcpp/decorators.hpp>

namespace btcpp {
Decorator::Decorator(std::string_view name) : InternalNode{1, name} {
}

Invert::Invert(std::string_view name) : Decorator{name} {
}

State Invert::do_tick() {
    switch (child()->tick()) {
    case success:
        return failure;
    case failure:
        return success;
    case running:
        return running;
    }
}

Retry::Retry(int retries, std::string_view name)
    : Decorator{name}, retries_{retries} {
}

State Retry::do_tick() {
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

} // namespace btcpp