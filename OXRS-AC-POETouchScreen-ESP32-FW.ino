/**
  Ethernet POE touchscreen firmware for the Open eXtensible Rack System
  
  See https://oxrs.io/docs/firmware/poe-touchscreen-esp32.html for documentation.

  Compile options:
    ESP32
    
  External dependencies. Install using the Arduino library manager:
    "PubSubClient" by Nick O'Leary
    "OXRS-IO-MQTT-ESP32-LIB" by OXRS Core Team
    "OXRS-IO-API-ESP32-LIB" by OXRS Core Team
    "OXRS-AC-I2CSensors-ESP-LIB" by Austins Creations
    "ledPWM" by Austins Creations

  Compatible with WT32-SC01 Ethernet POE shield found here:
    https://github.com/austinscreations/WT32-SC01_Ethernet_POE

  GitHub repository:
    https://github.com/austinscreations/OXRS-AC-POETouchScreen-ESP32-FW
    
  Bugs/Features:
    See GitHub issues list

  Copyright 2022 Austins Creations
*/

/*--------------------------- Version ------------------------------------*/
#define FW_NAME       "OXRS-AC-POETouchScreen-ESP32-FW"
#define FW_SHORT_NAME "POE Touch Screen"
#define FW_MAKER      "Austin's Creations"
#define FW_VERSION    "0.0.2"

/*--------------------------- Libraries ----------------------------------*/
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <SPI.h>
#include <Ethernet.h>
#include <OXRS_MQTT.h>              // For MQTT
#include <OXRS_API.h>               // For REST API
#include <OXRS_SENSORS.h>           // For QUICC I2C sensors

/*--------------------------- Constants ----------------------------------*/
// Serial
#define       SERIAL_BAUD_RATE          115200

// I2C
#define       I2C_SCL                   19
#define       I2C_SDA                   18

// REST API
#define       REST_API_PORT             80

// Ethernet
#define       ETH_SCLK                  32
#define       ETH_MISO                  27
#define       ETH_MOSI                  33
#define       ETHERNET_CS_PIN           26
#define       WIZNET_RESET_PIN          25
#define       DHCP_TIMEOUT_MS           15000
#define       DHCP_RESPONSE_TIMEOUT_MS  4000

// Display
#define       DISPLAY_CS_PIN            15

/*--------------------------- Instantiate Global Objects -----------------*/
// Ethernet client
EthernetClient client;

// MQTT client
PubSubClient mqttClient(client);
OXRS_MQTT mqtt(mqttClient);

// REST API
EthernetServer server(REST_API_PORT);
OXRS_API api(mqtt);

// QUICC I2C sensors
OXRS_SENSORS sensors(mqtt);

/*--------------------------- Program ------------------------------------*/
void getFirmwareJson(JsonVariant json)
{
  JsonObject firmware = json.createNestedObject("firmware");

  firmware["name"] = FW_NAME;
  firmware["shortName"] = FW_SHORT_NAME;
  firmware["maker"] = FW_MAKER;
  firmware["version"] = FW_VERSION;
}

void getNetworkJson(JsonVariant json)
{
  byte mac[6];
  WiFi.macAddress(mac);
  
  char mac_display[18];
  sprintf_P(mac_display, PSTR("%02X:%02X:%02X:%02X:%02X:%02X"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  JsonObject network = json.createNestedObject("network");

  network["ip"] = Ethernet.localIP();
  network["mac"] = mac_display;
}

void getConfigSchemaJson(JsonVariant json)
{
  JsonObject configSchema = json.createNestedObject("configSchema");
  
  // Config schema metadata
  configSchema["$schema"] = "http://json-schema.org/draft-07/schema#";
  configSchema["title"] = FW_NAME;
  configSchema["type"] = "object";

  JsonObject properties = configSchema.createNestedObject("properties");

  // Add any sensor config
  sensors.setConfigSchema(properties);
}

void getCommandSchemaJson(JsonVariant json)
{
  JsonObject commandSchema = json.createNestedObject("commandSchema");
  
  // Command schema metadata
  commandSchema["$schema"] = "http://json-schema.org/draft-07/schema#";
  commandSchema["title"] = FW_NAME;
  commandSchema["type"] = "object";

  JsonObject properties = commandSchema.createNestedObject("properties");

  JsonObject restart = properties.createNestedObject("restart");
  restart["title"] = "Restart";
  restart["type"] = "boolean";

  // Add any sensor commands
  sensors.setCommandSchema(properties);
}

/**
  API callbacks
*/
void apiAdopt(JsonVariant json)
{
  // Build device adoption info
  getFirmwareJson(json);
  getNetworkJson(json);
  getConfigSchemaJson(json);
  getCommandSchemaJson(json);
}

/**
  MQTT callbacks
*/
void mqttConnected() 
{
  // Publish device adoption info
  DynamicJsonDocument json(JSON_ADOPT_MAX_SIZE);
  mqtt.publishAdopt(api.getAdopt(json.as<JsonVariant>()));

  // Log the fact we are now connected
  Serial.println("[wt32] mqtt connected");
}

void mqttDisconnected(int state) 
{
  // Log the disconnect reason
  // See https://github.com/knolleary/pubsubclient/blob/2d228f2f862a95846c65a8518c79f48dfc8f188c/src/PubSubClient.h#L44
  switch (state)
  {
    case MQTT_CONNECTION_TIMEOUT:
      Serial.println(F("[wt32] mqtt connection timeout"));
      break;
    case MQTT_CONNECTION_LOST:
      Serial.println(F("[wt32] mqtt connection lost"));
      break;
    case MQTT_CONNECT_FAILED:
      Serial.println(F("[wt32] mqtt connect failed"));
      break;
    case MQTT_DISCONNECTED:
      Serial.println(F("[wt32] mqtt disconnected"));
      break;
    case MQTT_CONNECT_BAD_PROTOCOL:
      Serial.println(F("[wt32] mqtt bad protocol"));
      break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
      Serial.println(F("[wt32] mqtt bad client id"));
      break;
    case MQTT_CONNECT_UNAVAILABLE:
      Serial.println(F("[wt32] mqtt unavailable"));
      break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
      Serial.println(F("[wt32] mqtt bad credentials"));
      break;      
    case MQTT_CONNECT_UNAUTHORIZED:
      Serial.println(F("[wt32] mqtt unauthorised"));
      break;      
  }
}

void mqttConfig(JsonVariant json)
{
  // Let the sensors handle any config
  sensors.conf(json);
}

void mqttCommand(JsonVariant json)
{  
  if (json.containsKey("restart") && json["restart"].as<bool>())
  {
    ESP.restart();
  }

  // Let the sensors handle any commands
  sensors.cmnd(json);
}

void mqttCallback(char * topic, uint8_t * payload, unsigned int length) 
{
  // Pass down to our MQTT handler and check it was processed ok
  int state = mqtt.receive(topic, payload, length);
  switch (state)
  {
    case MQTT_RECEIVE_ZERO_LENGTH:
      Serial.println(F("[wt32] empty mqtt payload received"));
      break;
    case MQTT_RECEIVE_JSON_ERROR:
      Serial.println(F("[wt32] failed to deserialise mqtt json payload"));
      break;
    case MQTT_RECEIVE_NO_CONFIG_HANDLER:
      Serial.println(F("[wt32] no mqtt config handler"));
      break;
    case MQTT_RECEIVE_NO_COMMAND_HANDLER:
      Serial.println(F("[wt32] no mqtt command handler"));
      break;
  }
}

/**
  Setup functions
*/
void initialiseEthernet(byte * mac)
{
  // Get ESP32 base MAC address
  WiFi.macAddress(mac);
  
  // Ethernet MAC address is base MAC + 3
  // See https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system.html#mac-address
  mac[5] += 3;

  // Display the MAC address on serial
  char mac_display[18];
  sprintf_P(mac_display, PSTR("%02X:%02X:%02X:%02X:%02X:%02X"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print(F("[wt32] mac address: "));
  Serial.println(mac_display);

  // Initialise ethernet library
  Ethernet.init(ETHERNET_CS_PIN);

  // Reset Wiznet W5500
  pinMode(WIZNET_RESET_PIN, OUTPUT);
  digitalWrite(WIZNET_RESET_PIN, HIGH);
  delay(250);
  digitalWrite(WIZNET_RESET_PIN, LOW);
  delay(50);
  digitalWrite(WIZNET_RESET_PIN, HIGH);
  delay(350);

  // WT32-SC01 uses a non-standard SPI bus
  SPI.begin(ETH_SCLK, ETH_MISO, ETH_MOSI, ETHERNET_CS_PIN);

  // Get an IP address via DHCP and display on serial
  Serial.print(F("[wt32] ip address: "));
  if (Ethernet.begin(mac, DHCP_TIMEOUT_MS, DHCP_RESPONSE_TIMEOUT_MS))
  {
    Serial.println(Ethernet.localIP());
  }
  else
  {
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println(F("ethernet shield not found"));
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println(F("ethernet cable not connected"));
    } else {
      Serial.println(F("unknown problem initialising ethernet"));
    }
  }
}

void initialiseMqtt(byte * mac)
{
  // NOTE: this must be called *before* initialising the REST API since
  //       that will load MQTT config from file, which has precendence

  // Set the default client id to the last 3 bytes of the MAC address
  char clientId[32];
  sprintf_P(clientId, PSTR("%02x%02x%02x"), mac[3], mac[4], mac[5]);  
  mqtt.setClientId(clientId);
  
  // Register our callbacks
  mqtt.onConnected(mqttConnected);
  mqtt.onDisconnected(mqttDisconnected);
  mqtt.onConfig(mqttConfig);
  mqtt.onCommand(mqttCommand);  

  // Start listening for MQTT messages
  mqttClient.setCallback(mqttCallback);  
}

void initialiseRestApi(void)
{
  // NOTE: this must be called *after* initialising MQTT since that sets
  //       the default client id, which has lower precendence than MQTT
  //       settings stored in file and loaded by the API

  // Set up the REST API
  api.begin();

  // Register our callbacks
  api.onAdopt(apiAdopt);

  // Start listening on our REST API port
  server.begin();
}

/**
  Main program
*/
void setup() 
{
  // Startup logging to serial
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  Serial.println();
  Serial.println(F("========================================"));
  Serial.print  (F("FIRMWARE: ")); Serial.println(FW_NAME);
  Serial.print  (F("MAKER:    ")); Serial.println(FW_MAKER);
  Serial.print  (F("VERSION:  ")); Serial.println(FW_VERSION);
  Serial.println(F("========================================"));

  // Start the I2C bus
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Set up the display
  pinMode(DISPLAY_CS_PIN, OUTPUT);
  digitalWrite(DISPLAY_CS_PIN, HIGH);

  // Set up ethernet and obtain an IP address
  byte mac[6];
  initialiseEthernet(mac);

  // Set up MQTT (don't attempt to connect yet)
  initialiseMqtt(mac);

  // Set up the REST API
  initialiseRestApi();

  // Initialise the I2C sensors
  sensors.begin();
}

void loop()
{
  // Maintain our DHCP lease
  Ethernet.maintain();
  
  // Handle any MQTT messages
  mqtt.loop();
  
  // Handle any REST API requests
  EthernetClient client = server.available();
  api.checkEthernet(&client);
  
  // Publish any sensor reports
  sensors.tele();
}
