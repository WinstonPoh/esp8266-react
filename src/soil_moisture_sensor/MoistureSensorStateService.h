#ifndef MoistureSensorStateService_h
#define MoistureSensorStateService_h

#include <soil_moisture_sensor/MoistureSensorMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define LED_PIN 2
#define MOISTURE_SENSOR_POWER_PIN 5 //D5
#define MOISTURE_SENSOR_SIG_PIN 0 // A0
#define PRINT_DELAY 5000

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"

#define DEFAULT_MOISTURE_SENSOR_STATE false

// moisturesensor power pin D5
#ifdef ESP32
#define MOISTURE_SENSOR_ON 0x1
#define MOISTURE_SENSOR_OFF 0x0
#elif defined(ESP8266)
#define MOISTURE_SENSOR_ON 0x0
#define MOISTURE_SENSOR_OFF 0x1
#endif

// #define LIGHT_SETTINGS_ENDPOINT_PATH "/rest/lightState"
// #define LIGHT_SETTINGS_SOCKET_PATH "/ws/lightState"

#define MOISTURE_SENSOR_SETTINGS_ENDPOINT_PATH "/rest/moistureSensorState"
#define MOISTURE_SENSOR_SETTINGS_SOCKET_PATH "/ws/moistureSensorState"

class MoistureSensorState {
    public:
        bool moisture_sensor_on;
        uint32_t moisture_sensor_value;

        static void read(MoistureSensorState& settings, JsonObject& root) {
            root["moisture_sensor_on"] = settings.moisture_sensor_on;
            root["moisture_sensor_value"] = settings.moisture_sensor_value;
        }
        static StateUpdateResult update(JsonObject& root, MoistureSensorState& moistureSensorState) {
            boolean newState = root["moisture_sensor_on"] | DEFAULT_LED_STATE;
            if (moistureSensorState.moisture_sensor_on != newState) {
                moistureSensorState.moisture_sensor_on = newState;
                return StateUpdateResult::CHANGED;
            }
            return StateUpdateResult::UNCHANGED;
        }
        static void haRead(MoistureSensorState& settings, JsonObject& root) {
            root["sensor_state"] = settings.moisture_sensor_on ? ON_STATE : OFF_STATE;
        }
        static StateUpdateResult haUpdate(JsonObject& root, MoistureSensorState& moistureSensorState) {
            String state = root["sensor_state"];
            // parse new led state 
            boolean newState = false;
            if (state.equals(ON_STATE)) {
                newState = true;
            } else if (!state.equals(OFF_STATE)) {
                return StateUpdateResult::ERROR;
            }
            // change the new state, if required
            if (moistureSensorState.moisture_sensor_on != newState) {
                moistureSensorState.moisture_sensor_on = newState;
                return StateUpdateResult::CHANGED;
            }
            return StateUpdateResult::UNCHANGED;
        }
};


class MoistureSensorStateService : public StatefulService<MoistureSensorState> {
    public:
        MoistureSensorStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    MoistureSensorMqttSettingsService* moistureSensorMqttSettingsService);
        void begin();
    
    private:
        HttpEndpoint<MoistureSensorState> _httpEndpoint;
        MqttPubSub<MoistureSensorState> _mqttPubSub;
        WebSocketTxRx<MoistureSensorState> _webSocket;
        AsyncMqttClient* _mqttClient;
        MoistureSensorMqttSettingsService* _moistureSensorMqttSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
