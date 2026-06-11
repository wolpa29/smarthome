# Smart Home Central

This repository is the heart of my smart home setup — it contains ESP32/ESP8266 device firmware and links to my custom Home Assistant automation tools.

---

## ESP Devices

| Device | Description |
|--------|-------------|
| [Door Reed Contact](devices/door_reed_contact/) | ESP32-C3 door sensor using a magnetic reed switch; triggers instant MQTT state changes for light automations |
| [Gate Relay Controller](devices/gate_relais_controller/) | ESP32-C3 dual-channel relay controller for a driveway gate with walk/car positions and auto-stop pulse |
| [Pool Flow Sensor](devices/pool_flow_sensor/) | ESP8266 hall-effect flow sensor monitoring the pool overflow return channel via MQTT |
| [Pool Tech Room Sensor](devices/pool_techroom_dht_sensor/) | ESP8266 + DHT22 temperature & humidity sensor for the pool technical room with OTA support |

---

## Featured Projects

<table>
  <tr>
    <td width="80">
      <a href="https://github.com/wolpay29/hass-ai-gateway">
        <img src="https://raw.githubusercontent.com/wolpay29/hass-ai-gateway/main/addon/icon.png" width="72" alt="hass-ai-gateway icon">
      </a>
    </td>
    <td>
      <b><a href="https://github.com/wolpay29/hass-ai-gateway">Home Assistant Local LLM Voice & Text Control Add-On</a></b><br>
      A Home Assistant add-on that routes voice and text commands to a locally running LLM — no cloud required.
    </td>
  </tr>
  <tr>
    <td width="80"></td>
    <td>
      <b><a href="https://github.com/wolpa29/local-wakeword-trainer">Local Wakeword Trainer for Voice Assistant</a></b><br>
      Tool for recording and training custom wakewords for local voice assistant setups.
    </td>
  </tr>
  <tr>
    <td width="80">
      <a href="https://github.com/wolpa29/homeassistant-pv-forecast-planner">
        <img src="https://raw.githubusercontent.com/wolpa29/homeassistant-pv-forecast-planner/main/assets/icon.png" width="72" alt="PV Forecast Planner icon">
      </a>
    </td>
    <td>
      <b><a href="https://github.com/wolpa29/homeassistant-pv-forecast-planner">Home Assistant PV Forecast Planner</a></b><br>
      ML pipeline for PV forecasting and planning -> data scraping, dataset building, ML Model training (XGBoost), forecasting, planning.
    </td>
  </tr>
  <tr>
    <td width="80">
      <a href="https://github.com/wolpa29/homeassistant-battery-forecast">
        <img src="https://raw.githubusercontent.com/wolpa29/homeassistant-battery-forecast/main/assets/icon.png" width="72" alt="Battery Forecast icon">
      </a>
    </td>
    <td>
      <b><a href="https://github.com/wolpa29/homeassistant-battery-forecast">Home Assistant Battery Forecast</a></b><br>
      Forecasts home battery charge levels based on PV production and consumption patterns.
    </td>
  </tr>
</table>
