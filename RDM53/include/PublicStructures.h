/*
 * This file contains all header public structures
 * 
 * Date: 2019 06 0
 * Author: Pascal Pfeiffer
 */

/*
 * This struct is used for the device states and modes
 * mode:
 * ----------------------------------------------------
 * PROTOCOL        PAYLOAD
 * 3rd B     START     END
 * ----------------------------------------------------
 * 0x00 00 + payload        : Autonomous
 * 0x01 00 + 0x00      0x0F : Remote Control (static)
 * 0x01 00 + 0x10      0x1F : Remote Control (dynamic)
 * 0x02 00 + 0x00      0x00 : Pause
 * 0x03 00 + 0x00      0x00 : Reset
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
 */
typedef struct deviceConfig {
    unsigned int mode = 0x0200;
    short debugPort = 0;
};