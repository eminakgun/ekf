#pragma once
#include "Subscription.hpp"

namespace uORB {

class SubscriptionCallbackWorkItem : public Subscription { // Hacky inheritance
public:
    SubscriptionCallbackWorkItem(void *work_item, int instance = 0) {}
    void registerCallback() {}
    void unregisterCallback() {}
};

}
