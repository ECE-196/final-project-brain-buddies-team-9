// Wifi Time Update

// SimpleTime Example
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"

 char *ssid = "VWLife";
 char *password = "Wolfman1!";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const char *time_zone = "PST8PDT,M3.2.0,M11.1.0";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)
int current24hrTime = 0;

char currentTimeString[9] = {};
const int asciiOffset = 48;
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  current24hrTime = timeinfo.tm_hour * 100 + timeinfo.tm_min;
  //Serial.print("timinfo.tm_hour = "); Serial.println(timeinfo.tm_hour);
  if(9 < timeinfo.tm_hour && 13 > timeinfo.tm_hour || 21 < timeinfo.tm_hour){
    currentTimeString[0] = '1';
  }
  else {
    currentTimeString[0] = ' ';
  }
  if(13 > timeinfo.tm_hour){
      currentTimeString[1] = (timeinfo.tm_hour % 10) + asciiOffset;
  }
  else{
      currentTimeString[1] = ((timeinfo.tm_hour - 2) % 10) + asciiOffset;
  }
  if(0 == timeinfo.tm_hour){
    currentTimeString[0] = '1';
    currentTimeString[1] = '2';
  }
  currentTimeString[2] = ':';
  currentTimeString[3] = (timeinfo.tm_min / 10) + asciiOffset;
  currentTimeString[4] = (timeinfo.tm_min % 10) + asciiOffset;
  currentTimeString[5] = ' ';
  if(12 < timeinfo.tm_hour){
    currentTimeString[6] = 'P';
    currentTimeString[7] = 'M';
  }
  else{
    currentTimeString[6] = 'A';
    currentTimeString[7] = 'M';
  }
  currentTimeString[8] = '\0';
  lv_label_set_text(ui_clock, currentTimeString);
}// ascii: 0 = 48, 9 = 57;


// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
  WiFi.disconnect();
}
// END: SimpleTime Example
/*
struct tm
{
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
*/

// SimpleTime begin
void setupClockTime(char* ssid, char* password){
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    esp_sntp_servermode_dhcp(1);
    int WifiAttemptCount = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      WifiAttemptCount++;
      if(5 < WifiAttemptCount){
        Serial.println(" CONNECTION FAILED");
        return;
      }
    }
    Serial.println(" CONNECTED");

    // set notification call-back function
    sntp_set_time_sync_notification_cb(timeavailable);
    // #define TZ_America_Los_Angeles	PSTR("PST8PDT,M3.2.0,M11.1.0")
    configTzTime(PSTR("PST8PDT,M3.2.0,M11.1.0"), ntpServer1, ntpServer2);
  // Simple time end
}
