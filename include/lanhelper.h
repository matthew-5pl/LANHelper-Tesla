// This code war originally written for:
// https://github.com/matthew-5pl/LANHelper

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <switch.h>

#ifdef VSCODE
// shut vscode up, since it thinks
// time() and sprintf() don't exist
// even though it can find the files
// that contain them
#define time(x) x
#define sprintf(a, b, ...)
#endif

PadState pad;

void genIp(char* out, int* outip) {
    srand(time(0));
    int ip_part1 = 10;
    int ip_part2 = 13;
    int ip_part3;
    int ip_part4;
    do {
        ip_part3 = rand() % 255;
        ip_part4 = rand() % 254;
    } while(ip_part3 == 37 && ip_part4 == 1);

    outip[0] = ip_part1;
    outip[1] = ip_part2;
    outip[2] = ip_part3;
    outip[3] = ip_part4;
    sprintf(out, "%d.%d.%d.%d", ip_part1, ip_part2, ip_part3, ip_part4);
}

void defaultIP() {
    NifmNetworkProfileData profile;
    nifmGetCurrentNetworkProfile(&profile);

    // IP Auto ON
    profile.ip_setting_data.ip_address_setting.is_automatic = (u8)(1);

    // DNS Auto OFF
    profile.ip_setting_data.dns_setting.is_automatic = (u8)(0);
    
    //DNS 1 163.172.141.219
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[0] = (u8)(163);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[1] = (u8)(172);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[2] = (u8)(141);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[3] = (u8)(219);

    //DNS 2 207.246.121.77
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[0] = (u8)(207);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[1] = (u8)(246);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[2] = (u8)(121);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[3] = (u8)(77);

    nifmSetNetworkProfile(&profile, &profile.uuid);
    nifmSetWirelessCommunicationEnabled(true);
}

Result setIp(const int* ip) {
    NifmNetworkProfileData profile;
    nifmGetCurrentNetworkProfile(&profile);

    // IP Auto OFF
    profile.ip_setting_data.ip_address_setting.is_automatic = (u8)(0);

    // IP
    for(int i = 0; i < 4; i++) {
        profile.ip_setting_data.ip_address_setting.current_addr.addr[i] = (u8)ip[i];
    }
    
    //Subnet Mask 255.255.0.0
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[0] = (u8)(255);
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[1] = (u8)(255);
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[2] = (u8)(0);
    profile.ip_setting_data.ip_address_setting.subnet_mask.addr[3] = (u8)(0);

    // Gateway 10.13.37.1
    profile.ip_setting_data.ip_address_setting.gateway.addr[0] = (u8)(10);
    profile.ip_setting_data.ip_address_setting.gateway.addr[1] = (u8)(13);
    profile.ip_setting_data.ip_address_setting.gateway.addr[2] = (u8)(37);
    profile.ip_setting_data.ip_address_setting.gateway.addr[3] = (u8)(1);

    // DNS Auto OFF
    profile.ip_setting_data.dns_setting.is_automatic = (u8)(0);
    
    //DNS 1 163.172.141.219
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[0] = (u8)(163);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[1] = (u8)(172);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[2] = (u8)(141);
    profile.ip_setting_data.dns_setting.primary_dns_server.addr[3] = (u8)(219);

    //DNS 2 207.246.121.77
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[0] = (u8)(207);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[1] = (u8)(246);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[2] = (u8)(121);
    profile.ip_setting_data.dns_setting.secondary_dns_server.addr[3] = (u8)(77);

    Result res = nifmSetNetworkProfile(&profile, &profile.uuid);
    nifmSetWirelessCommunicationEnabled(true);

    return res;
}

//void menu1() {
//    int* ip_int = malloc(sizeof(int)*5);
//    int state = 0;
//    char* menu[] = 
//    {
//        "Welcome to LANHelper.",
//        "Please select an option:",
//        "[L] Init LAN Play",
//        "[+] Quit"
//    };
//    char* ip = malloc(sizeof(char)*100);
//    while(appletMainLoop()) {
//        padUpdate(&pad);
//        u64 kDown = padGetButtonsDown(&pad);
//        consoleClear();
//        if(state == 0) {
//            for(int i = 0; i < 4; i++) {
//                printf("%s\n", menu[i]);
//            }
//            if(kDown & HidNpadButton_L) {
//            genIp(ip, ip_int);
//            state = 1;
//        }
//        }
//        else if(state == 1) {
//            printf("Accept IP address %s? ([A] yes, [+] Exit and reset)\n", ip);
//            if(kDown & HidNpadButton_Plus) {
//                defaultIP();
//                break;
//            }
//            if(kDown & HidNpadButton_A) {
//                setIp(ip_int);
//                printf("Set network settings.\n");
//                break;
//            }
//        }
//        if(kDown & HidNpadButton_Plus) { break; }
//        consoleUpdate(NULL);
//    }   
//}

// int main() {
//     consoleInit(NULL);
//     padConfigureInput(1, HidNpadStyleSet_NpadStandard);
//     padInitializeDefault(&pad);
//     menu1();
//     consoleExit(NULL);
//     return 0;
// }