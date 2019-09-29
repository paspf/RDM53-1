/*
 * This file stores user specific configurations.
 * recommend: add this file to .gitignore
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

//  use the option for a standard home wifi network
#define WIFI_HOME
#ifdef WIFI_HOME
    #define RDM_SSID "RDM53"
    #define RDM_SSID_PW "ChangeMe"
    #define NTP_SERVER_IS "pool.ntp.org"
#endif

// use this option for a company / eduroam wifi network
//#define WIFI_THM
#ifdef WIFI_THM
    #define RDM_SSID "YourCompanySSID"
    #define RDM_SSID_PW "YourCompanyKey"
    #define NTP_SERVER_IS "ntp.fh-friedberg.de"
#endif

#define RDM_SSID_USR "YourCompanyUserName"
#define RDM_SSID_IDNT "YourCompanyIdentity"
