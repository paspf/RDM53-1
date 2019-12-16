/*
 * This file stores user specific configurations.
 * recommend: add this file to .gitignore
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

#ifndef MAIN_USR_H
#define MAIN_USR_H

//  use the option for a standard home wifi network
#define WIFI_HOME
#ifdef WIFI_HOME
    #define RDM_SSID "RDM53"
    #define RDM_SSID_PW "ChangeMe"
    #define NTP_SERVER_IS "pool.ntp.org"
#endif

// use this option for a enterprise / eduroam wifi network
//#define WIFI_ENTERPRISE
#ifdef WIFI_ENTERPRISE
    #define RDM_SSID "YourCompanySSID"
    #define RDM_SSID_PW "YourCompanyKey"
    #define NTP_SERVER_IS "ntp.fh-friedberg.de"
#endif

#define RDM_SSID_USR "YourCompanyUserName"
#define RDM_SSID_IDNT "YourCompanyIdentity"

/*
 * Room and parcours profiles
 * the profiles are used to switch between
 * different lighting and color conditions
 */
//#define PARCOUR_A3_306 // PARCOUR_0 DT Lab own colors
//#define PARCOUR_A2_007 // PARCOUR_1 con lab colors
#define PARCOUR_2_OG_146_C // PARCOUR_2 RNG Lab own Cols

#endif