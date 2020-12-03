#ifndef MoistureSensorMqttSettingsService_h
#define MoistureSensorMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPUtils.h>

#define MOISTURE_SENSOR_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define MOISTURE_SENSOR_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class MoistureSensorMqttSettings {
    public:
        String mqttPath;
        String name;
        String uniqueId;

        static void read(MoistureSensorMqttSettings& settings, JsonObject& root) {
            root["mqtt_path"] = settings.mqttPath;
            root["name"] = settings.name;
            root["unique_id"] = settings.uniqueId;
        }

        static StateUpdateResult update(JsonObject& root, MoistureSensorMqttSettings& settings) {
            settings.mqttPath = root["mqtt_path"] | ESPUtils::defaultDeviceValue("homeassistant/moisture_sensor/");
            settings.name = root["name"] | ESPUtils::defaultDeviceValue("moisture_sensor-");
            settings.uniqueId = root["unique_id"] | ESPUtils::defaultDeviceValue("moisture_sensor-");
            return StateUpdateResult::CHANGED;
        }
};

class MoistureSensorMqttSettingsService : public StatefulService<MoistureSensorMqttSettings> {
    public:
        MoistureSensorMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
        void begin();

    private:
        HttpEndpoint<MoistureSensorMqttSettings> _httpEndpoint;
        FSPersistence<MoistureSensorMqttSettings> _fsPersistence;
};

#endif  // end MoistureSensorMqttSettingsService_h
