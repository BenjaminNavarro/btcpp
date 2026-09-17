#include <btcpp/generic_condition.hpp>

#include <stdexcept>

namespace btcpp {

State GenericCondition::do_tick() {
    const auto state = condition_();
    if (state == State::Running) {
        throw std::logic_error(
            "[GenericCondition] conditions cannot be in a 'Running' state");
    }
    return state;
}

} // namespace btcpp