/*
  WiFi 2ch Relay Timer

  -----------------------------------------------------------------------------------------------
  History

  Version: 1.0.0 - 2020-07-05
  First release

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "RTClib.h"

#include "helpers.h"
#include "global.h"

// HTML CSS JS
#include "ui/script.js.h"
#include "ui/style.css.h"
#include "ui/information.h"
#include "ui/settings.h"
#include "ui/404.h"

// AP congig
#define ACCESS_POINT_NAME  "WiFi Rele"
#define ACCESS_POINT_PASSWORD  "12345678"

void setup() {
  delay(1000);
  EEPROM.begin(512);
  Serial.begin(115200);

  Serial.println("Creating access point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ACCESS_POINT_NAME, ACCESS_POINT_PASSWORD);

  Serial.println("Running dns server...");
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  // Web pages
  server.on("/generate_204", []() {
    Serial.println("open: HTML_Information");
    server.send ( 200, "text/html", HTML_Information );
  });  //Android captive portal. Maybe not needed. Might be handled by notFound handler.

  server.on("/fwlink", []() {
    Serial.println("open: HTML_Information");
    server.send ( 200, "text/html", HTML_Information );
  });  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.

  // 404 page
  server.onNotFound (send_404_html ); 

  // favicon.ico
  server.on ( "/favicon.ico",   []() {
    Serial.println("favicon.ico");
    server.send ( 200, "text/html", "" );
  }  );

  // style.css
  server.on ( "/style.css", []() {
    Serial.println("style.css");
    server.send ( 200, "text/plain", HTML_Style_css );
  } );

  // microajax.js
  server.on ( "/microajax.js", []() {
    Serial.println("microajax.js");
    server.send ( 200, "text/plain", HTML_microajax_js );
  } );

  // Root page
  server.on ( "/", []() {
    server.send ( 200, "text/html", HTML_Information );
  }  );
  server.on ( "/informationvalues", send_information_values_html );

  // Settings page
  server.on ( "/settings", send_settings_html );
  server.on ( "/settingsvalues", send_settings_values_html );

  // Web server
  server.begin();
  Serial.println("Server started...");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    //abort();
  }

  // Default config
  if (!ReadConfig())
  {
    config.R1_hh = 99;
    config.R1_mm = 99;
    config.R1_dd = 99;
    config.R2_hh = 99;
    config.R2_mm = 99;
    config.R2_dd = 99;
    WriteConfig();
    Serial.println("Default config applied");
  }

  // Pins init
  pinMode(R1_pin, OUTPUT);
  pinMode(R2_pin, OUTPUT);

  // Turn off
  relay_off(R1_pin);
  relay_off(R2_pin);
}

void loop() {
  // DNS Server
  dnsServer.processNextRequest();

  // Timer functions
  DateTime now = rtc.now();
  if (AutoMode)
  {
    // Relay 1 timer
    if (TimeToMinute(now.hour(), now.minute(), 0) >= TimeToMinute(config.R1_hh, config.R1_mm, 0) && TimeToMinute(now.hour(), now.minute(), 0) < TimeToMinute(config.R1_hh, config.R1_mm, config.R1_dd)) {
      relay_on(R1_pin);
      R1_state = LOW;
    }
    else
    {
      relay_off(R1_pin);
      R1_state = HIGH;
    }
    // Relay 2 timer
    if (TimeToMinute(now.hour(), now.minute(), 0) >= TimeToMinute(config.R2_hh, config.R2_mm, 0) && TimeToMinute(now.hour(), now.minute(), 0) < TimeToMinute(config.R2_hh, config.R2_mm, config.R2_dd)) {
      relay_on(R2_pin);
      R2_state = LOW;
    }
    else
    {
      relay_off(R2_pin);
      R2_state = HIGH;
    }
  } else {
    // do noting
  }

  // Handle web clients
  server.handleClient();
}
