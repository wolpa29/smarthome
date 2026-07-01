# Pool Flow Sensor

Monitors water returning from the pool overflow channel with an ESP8266 and a hall effect flow sensor on GPIO4 / D2.

It measures pulses per second. There's no conversion to mass or volume flow: the overflow return isn't a steady stream, so that wouldn't be accurate anyway. The raw pulse rate is enough to see whether water is flowing and how much is going on - enough to detect pool usage and trigger the pump.

The device only publishes the **raw pulse rate once per second**. All analysis (average, stddev, trends) happens in Home Assistant. That keeps the firmware simple and lets me tune the logic (windows, thresholds) live without reflashing.

## Hardware
- Board: ESP8266 (NodeMCU)
- Sensor: Hall effect flow sensor on GPIO4 / D2

## Features
- Raw pulse rate published once per second (analysis offloaded to Home Assistant).
- Remote debugging output over Wi-Fi using TelnetStream (Port 2323).
- Non-blocking Wi-Fi and MQTT reconnect logic.
- MQTT Last Will and Testament (LWT) for online/offline device tracking.
- OTA update support.

## MQTT
Topics published by the device:
- `pool/flow/status` : Device status (`online` / `offline`) via LWT
- `pool/flow/raw` : Raw pulse rate (pulses/s), published every second

## Home Assistant
The device publishes only the raw value. Derived metrics (average, stddev,
trend) are computed in Home Assistant, so they can be tuned without reflashing.

### Base sensor
```yaml
mqtt:
  sensor:
    - name: "Pool Flow Raw"
      state_topic: "pool/flow/raw"
      unit_of_measurement: "p/s"
      value_template: "{{ value }}"
      availability_topic: "pool/flow/status"
```

### Derived sensors (average / standard deviation)
Use the [Statistics](https://www.home-assistant.io/integrations/statistics/)
integration on the raw sensor. Change `max_age` to change the window - no
firmware change needed.
```yaml
sensor:
  - platform: statistics
    name: "Pool Flow Average 90s"
    entity_id: sensor.pool_flow_raw
    state_characteristic: mean
    max_age:
      seconds: 90
  - platform: statistics
    name: "Pool Flow StdDev 90s"
    entity_id: sensor.pool_flow_raw
    state_characteristic: standard_deviation
    max_age:
      seconds: 90
  - platform: statistics
    name: "Pool Flow StdDev 10s"
    entity_id: sensor.pool_flow_raw
    state_characteristic: standard_deviation
    max_age:
      seconds: 10
```

## Requirements
- `ESP8266WiFi` (included in board package)
- `ArduinoOTA` (included in board package)
- `PubSubClient` (install via Library Manager)
- `TelnetStream` (install via Library Manager)

## Setup
Update Wi-Fi and MQTT credentials in the source code before flashing.

## Changelog
- **v2.0**: Firmware now publishes only the raw pulse rate once per second; all analysis (average, stddev, trends) moved to Home Assistant for flexibility.
- **v1.3**: Added TelnetStream for network debugging, implemented MQTT Last will testament LWT (online/offline status), improved reconnect logic.
- **v1.2**: Increased buffer to 90s, added trend calculations.
- **v1.1**: Switched to 60s buffer, added standard deviation calculations.
- **v1.0**: Initial version with basic 5-value moving average.