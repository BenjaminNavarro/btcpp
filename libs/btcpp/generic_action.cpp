#include <btcpp/generic_action.hpp>

namespace btcpp {

State GenericAction::do_tick() {
    return action_();
}

} // namespace btcpp