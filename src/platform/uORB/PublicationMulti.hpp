#include "uORB.h"

namespace uORB {

template<typename T>
class PublicationMulti {
public:
    PublicationMulti(orb_id_t id) {}
    PublicationMulti(orb_id_t id, int instance) {}
    bool advertise() { return true; }
    bool publish(const T &data) { return true; }
};

}
