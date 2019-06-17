/*
 * This file stores user specific configurations.
 * Add this file to .gitignore
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

#define WIFI_HOME
#ifdef WIFI_HOME
    #define RDM_SSID "house4_IOT"
    #define RDM_SSID_PW "SuppenIOT!Net"
    #define NTP_SERVER_IS "pool.ntp.org"
#endif

//#define WIFI_THM
#ifdef WIFI_THM
    #define RDM_SSID "THMgast"
    #define RDM_SSID_PW "xi7nPKs8Nx"
    #define NTP_SERVER_IS "ntp.fh-friedberg.de"
#endif

#define RDM_SSID_USR "autoiem"
#define RDM_SSID_IDNT "autoiem"

