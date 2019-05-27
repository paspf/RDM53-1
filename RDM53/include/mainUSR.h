/*
 * This file stores user specific configurations.
 * Add this file to .gitignore
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

//#define WIFI_HOME
#ifdef WIFI_HOME
    #define RDM_SSID "house4_IOT"
    #define RDM_SSID_PW ""
#endif

#define WIFI_THM
#ifdef WIFI_THM
    #define RDM_SSID "THMnet"
    #define RDM_SSID_PW ""
#endif

#define RDM_SSID_USR ""
#define RDM_SSID_IDNT ""

