export module test_utils;

import btcpp;

export namespace testing {

template <btcpp::State S>
class ActionResult final : public btcpp::ExecutionNode {
public:
    using btcpp::ExecutionNode::ExecutionNode;

    [[nodiscard]] btcpp::State tick() final {
        ticked_ = true;
        return S;
    }

    [[nodiscard]] bool ticked() const {
        return ticked_;
    }

private:
    bool ticked_{};
};

using SuccessAction = ActionResult<btcpp::success>;
using FailureAction = ActionResult<btcpp::failure>;
using RunningAction = ActionResult<btcpp::running>;

} // namespace testing