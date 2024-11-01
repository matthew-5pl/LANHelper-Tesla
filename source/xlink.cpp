#include "xlink.hpp"

std::string xlink::getIPString(ipglobal::mac_addr hwaddr) {
    std::string out = "10.254.";

    out += std::to_string((uint8_t)hwaddr.p5);
    out += ".";
    out += std::to_string((uint8_t)hwaddr.p6);

    return out;
}

Result xlink::setIPAddress(ipglobal::mac_addr hwaddr) {
    NifmNetworkProfileData profile;
    nifmGetCurrentNetworkProfile(&profile);

    // IP Auto OFF
    profile.ip_setting_data.ip_address_setting.is_automatic = (u8)(0);

    // IP Address 10.254.{MAC byte 5}.{MAC byte 6}
    profile.ip_setting_data.ip_address_setting.current_addr.addr[0] = (u8)(10);
    profile.ip_setting_data.ip_address_setting.current_addr.addr[1] = (u8)(254);
    profile.ip_setting_data.ip_address_setting.current_addr.addr[2] = (u8)(hwaddr.p5);
    profile.ip_setting_data.ip_address_setting.current_addr.addr[3] = (u8)(hwaddr.p6);
    
    // Subnet Mask 255.255.0.0
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[0] = (u8)(255);
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[1] = (u8)(255);
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[2] = (u8)(0);
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[3] = (u8)(0);

    // Gateway 10.254.0.1
    profile.ip_setting_data.ip_address_setting.gateway.addr[0] = (u8)(10);
    profile.ip_setting_data.ip_address_setting.gateway.addr[1] = (u8)(254);
    profile.ip_setting_data.ip_address_setting.gateway.addr[2] = (u8)(0);
    profile.ip_setting_data.ip_address_setting.gateway.addr[3] = (u8)(1);

    // DNS Auto OFF
    profile.ip_setting_data.dns_setting.is_automatic = (u8)(0);
    
    //DNS 1 10.254.0.1
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[0] = (u8)(10);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[1] = (u8)(254);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[2] = (u8)(0);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[3] = (u8)(1);

    //DNS 2 0.0.0.0
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[0] = (u8)(0);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[1] = (u8)(0);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[2] = (u8)(0);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[3] = (u8)(0);

    Result res = nifmSetNetworkProfile(&profile, &profile.uuid);
    nifmSetWirelessCommunicationEnabled(true);

    return res;
}