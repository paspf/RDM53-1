/*
 * This file contains all Init functions
 * for the ESP32
 * 
 * Date: 2019 05 21
 * Author: Pascal Pfeiffer
 */

#include "ESP32Init.h"


 // class ESP32Init {
    // public:
    /*
    * Initialize WiFi for the ESP 32
    */
    void RDMWiFiInit() {
        #ifdef WIFI_HOME
            Serial.println("Set up Home WiFi");
            setUpHomeWiFi();
        #else
            Serial.println("No home Wifi defined!");
        #endif

        #ifdef WIFI_THM
            Serial.println("Set up THM WiFi");
            setUpTHMWifi();
        #else
            Serial.println("No enterprise Wifi defined!");
        #endif

        #if defined(WIFI_HOME) && defined(WIFI_THM)
            Serial.println("Enterprise and Home WiFi defined!");
            return;
        #endif
        #ifndef WIFI_HOME
            #ifndef WIFI_THM
                Serial.println("No WiFi defined!");
            #endif
        #endif
        Serial.print("\n\rConnecting Wifi: ");
        Serial.println(RDM_SSID);
        int i = 0;
        while(WiFi.status() != WL_CONNECTED && i < 60) {                        // try to connect with WiFi network
            delay(300);
            Serial.print(".");
            i++;
        }
        if( i >= 60) {
            Serial.println("Unable to connect to WiFi Network, WiFi disabled!");
            WiFi.disconnect(true);                                                // reconnect to WiFi Network disabled
            WiFi.mode(WIFI_OFF);                                                  // turn off WiFi SW
            yield();                                                              // run background tasks
        }
        if(WiFi.status() == WL_CONNECTED) {
            Serial.println("WiFi connected");
            Serial.printf("Singal strength:");
            Serial.println(WiFi.RSSI());
            Serial.printf("IP address: ");
            Serial.println(WiFi.localIP());

            Serial.printf("Client MAC address: ");
            Serial.println(WiFi.macAddress());

            Serial.print("Host MAC address: ");
            Serial.println(WiFi.BSSIDstr());
            Serial.println("-----------------------");

            delay(10);
            Serial.println("Getting time from ntp server...");
            configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER_IS);
            delay(10);
            printLocalTime();
            Serial.println("-----------------------");
        }
    }

    /*  
 *   initialize over the air upgrade
 *   Based on ArduinoOTA
 */
void OTAirInit() {
    // Set port defaults
    // ArduinoOTA.setPort(3232);
    // Hostname defaults
    ArduinoOTA.setHostname("RDM53-OTA");
    // Set OTA password
    // ArduinoOTA.setPassword((const char *)"123");                         
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd OTA");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("OTA Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("OTA Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("OTA Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("OTA Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("OTA End Failed");
    });
    ArduinoOTA.begin();
}

    /*
     * Print the local Time
     * https://github.com/espressif/arduino-esp32/issues/1225
     */
    void printLocalTime() {
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)) {
             Serial.println("Failed to obtain time");
             return;
            }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    }

    // private:
    /*
     * Setup WiFi for home use
     */
    void setUpHomeWiFi() {
        WiFi.begin(RDM_SSID, RDM_SSID_PW);
    }
    /*
     * Set Up Wifi for use at THM
     * based on:
     * https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecureEnterprise/WiFiClientSecureEnterprise.ino
     */
    void setUpTHMWifi() {
        WiFi.mode(WIFI_STA); //init wifi mode
        esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)RDM_SSID_IDNT, strlen(RDM_SSID_IDNT)); //provide identity
        esp_wifi_sta_wpa2_ent_set_username((uint8_t *)RDM_SSID_USR, strlen(RDM_SSID_USR)); //provide username --> identity and username is same
        esp_wifi_sta_wpa2_ent_set_password((uint8_t *)RDM_SSID_PW, strlen(RDM_SSID_PW)); //provide password
        esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
        esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
        WiFi.begin(RDM_SSID); //connect to wifi
    }
 // };