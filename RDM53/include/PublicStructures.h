/*
 * This file contains public structures
 * 
 * Date: 2019 06 0
 * Author: Pascal Pfeiffer
 */

#ifndef PUB_STRUCT
#define PUB_STRUCT

/*
 * This struct is used for the device states and modes
 * mode:
 * ----------------------------------------------------
 * PROTOCOL     PAYLOAD (last payload byte)
 * 2rd Byte     START     END
 * ----------------------------------------------------
 * 0x00 00 00 + 0x00      0xFF : Autonomous
 * 0x01 00 00 + 0x00      0x0F : Remote Control (static)
 * 0x01 00 00 + 0x10      0x1F : Remote Control (dynamic)
 * 0x02 00 00 + 0x00      0x00 : Pause
 * 0x03 00 00 + 0x00      0x00 : Reset (Reboot device)
 * 
 * 
 * 2    : Remote Control (dynamic)
 * 
 * 101-n: different autonomous versions / implementations
 * 
 * debugPort:
 * 0    : Websockets and Serial
 * 1    : Websockets
 * 2    : Serial
 * 
 */
struct deviceConfig {
    unsigned int mode = 0x020000;
    short debugPort = 0;
    bool remoteControlDynamicUpdate = false;

    bool wiFiNotificationSent = false;
    /*
     * wiFiNotificationsender
     * TRUE: send sensor data cyclic to webinterface
     * FALSE: no effect (default)
     */
    bool wiFiNotificationSender = false;
};

#endif