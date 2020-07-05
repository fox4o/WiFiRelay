// Mode Auto/Manual
boolean AutoMode = true;

// Pin settings
int R1_pin = D5;
int R1_state = HIGH;
int R2_pin = D6;
int R2_state = HIGH;

// Config value
struct strConfig {
  // Relay 1
  int R1_hh = 99; // hours
  int R1_mm = 99; // minutes
  int R1_dd = 99; // duration

  // Relay 2
  int R2_hh = 99; // hours
  int R2_mm = 99; // minutes
  int R2_dd = 99; // duration
}   config;

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

// Soft AP config
IPAddress apIP(8, 8, 8, 8);
IPAddress netMsk(255, 255, 255, 0);

// RTC 
RTC_DS3231 rtc;

void WriteConfig()
{
  Serial.println("Writing Config...");
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');

  // Write config data
  // Relay 1
  EEPROM.write(8, config.R1_hh);
  EEPROM.write(9, config.R1_mm);
  EEPROM.write(10, config.R1_dd);
  // Relay 2
  EEPROM.write(16, config.R2_hh);
  EEPROM.write(17, config.R2_mm);
  EEPROM.write(18, config.R2_dd);

  EEPROM.commit();
}

boolean ReadConfig() {
  Serial.println("Reading Config...");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
  {
    // Read config data
    config.R1_hh = EEPROM.read(8);
    config.R1_mm = EEPROM.read(9);
    config.R1_dd = EEPROM.read(10);
    config.R2_hh = EEPROM.read(16);
    config.R2_mm = EEPROM.read(17);
    config.R2_dd = EEPROM.read(18);

    return true;
  }
  else
  {
    Serial.println("Configurarion NOT FOUND!!!!");
    return false;
  }
}

// Relay control
void relay_on(int r)
{
  digitalWrite(r, LOW);
}
void relay_off(int r)
{
  digitalWrite(r, HIGH);
}

