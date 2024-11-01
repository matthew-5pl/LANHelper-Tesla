#include <iostream>
#include <cstdint>

#include <switch.h>
#include "ipglobal.hpp"

namespace xlink {
    std::string getIPString(ipglobal::mac_addr hwaddr);
    Result setIPAddress(ipglobal::mac_addr hwaddr);
}