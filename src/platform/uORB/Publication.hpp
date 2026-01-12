#include "uORB.h"

namespace uORB {

template<typename T>
class Publication {
public:
    Publication(orb_id_t id) {}
    Publication(orb_id_t id, int instance) {}
    bool advertise() { return true; }
    bool publish(const T &data) { return true; }
};

}
