#include "uORB.h"

namespace uORB {

class Subscription {
public:
    Subscription(orb_id_t id) {}
    Subscription(orb_id_t id, int instance) {}
    bool updated() { return false; }
    bool copy(void *dst) { return false; }
    bool subscribe() { return true; }
};

}
