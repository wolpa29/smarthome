#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <TelnetStream.h>

//wlan data
const char* ssid = "WLAN_SSID";
const char* password = "WLAN_PASSWORD";

//mqtt data
const char* mqtt_server = "MQTT_SERVER_IP";
const uint16_t mqtt_port = 1883;
const char* mqtt_user = "MQTT_USER";
const char* mqtt_pass = "MQTT_PASSWORD";

//topics
const char* mqtt_topic_raw = "pool/flow/raw";   //raw pulses per second

//Optional: Online/Offline-Status (LWT)
const char* STATUS_TOPIC = "pool/flow/status";
const char* deviceName = "ESP8266-flow";

//GPIO
#define FLOW_PIN 4  // D2 on NodeMCU

volatile unsigned long pulseCount = 0;
bool statusPublishedOnline = false;

WiFiClient espClient;
PubSubClient mqtt(espClient);

//Interrupt service routine: Pulse count
void ICACHE_RAM_ATTR countPulse() {
  pulseCount++;
}

//wlan connect
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WLAN: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 60) { // 30 s
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WLAN connected, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WLAN connection failed, proceeding with OTA/MQTT retries...");
  }
}

//MQTT reconnect (non-blocking)
bool mqttReconnect() {
  Serial.print("Connecting to MQTT... ");

  String clientId = String(deviceName) + "-" + String(ESP.getChipId(), HEX);

  // Last-Will: offline retained
  const char* willTopic = STATUS_TOPIC;
  const char* willMessage = "offline";
  int willQos = 1;
  bool willRetain = true;

  bool ok = mqtt.connect(clientId.c_str(), mqtt_user, mqtt_pass, willTopic, willQos, willRetain, willMessage);

  if (ok) {
    Serial.println("Connected!");
    // Set to online after successful connect
    statusPublishedOnline = true;
    mqtt.publish(STATUS_TOPIC, "online", statusPublishedOnline);
  } else {
    Serial.print("Error, rc=");
    Serial.println(mqtt.state());
  }
  return ok;
}

//timecontrol
unsigned long lastMeasurement = 0;                  //time of the last 1s intervall measurement

void setup() {
  Serial.begin(115200);

  pinMode(FLOW_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), countPulse, FALLING);

  setup_wifi();
  mqtt.setServer(mqtt_server, mqtt_port);

  // OTA-Setup
  ArduinoOTA.setHostname(deviceName);
  ArduinoOTA.begin();
  Serial.println("OTA ready");

  // Telnet Setup
  TelnetStream.begin(2323);
  TelnetStream.println("Telnet ready");
}

void loop() {
  ArduinoOTA.handle();

  if (!mqtt.connected()) {
    mqttReconnect();
  }
  mqtt.loop();

  unsigned long now = millis();                     //act time

  //count and publish detected pulses every 1s
  if (now - lastMeasurement >= 1000) {              //if 1s intervall reached
    lastMeasurement = now;                          //save time

    //read pulses, deactivate interrupts in that time
    noInterrupts();
    unsigned long pulses = pulseCount;              //save
    pulseCount = 0;                                 //reset counter
    interrupts();

    float pulses_per_s = (float)pulses;             //to float

    //debug Serial
    Serial.print("Pulses/s: ");
    Serial.println(pulses_per_s);

    //debug Telnet
    TelnetStream.print("Pulses/s: ");
    TelnetStream.println(pulses_per_s);
    TelnetStream.flush();

    // MQTT: publish raw value (analysis happens in Home Assistant)
    char msgBuffer[16];
    dtostrf(pulses_per_s, 4, 2, msgBuffer);
    mqtt.publish(mqtt_topic_raw, msgBuffer);
  }
}
