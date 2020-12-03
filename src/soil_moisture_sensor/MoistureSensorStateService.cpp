#include <LightStateService.h>
#include <soil_moisture_sensor/MoistureSensorStateService.h>

#define SAMPLING_PERIOD 10

MoistureSensorStateService::MoistureSensorStateService(AsyncWebServer* server,
                                                        SecurityManager* securityManager,
                                                        AsyncMqttClient* mqttClient,
                                                        MoistureSensorMqttSettingsService* moistureSensorMqttSettingsService) :
    _httpEndpoint(MoistureSensorState::read,
                  MoistureSensorState::update,
                  this,
                  server,
                  MOISTURE_SENSOR_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(MoistureSensorState::haRead, MoistureSensorState::haUpdate, this, mqttClient),
    _webSocket(MoistureSensorState::read,
               MoistureSensorState::update,
               this,
               server,
               MOISTURE_SENSOR_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient),
    _moistureSensorMqttSettingsService(moistureSensorMqttSettingsService) {
        // configure moisture sensor power pin (D5) as output
        pinMode(MOISTURE_SENSOR_POWER_PIN, OUTPUT);
        
        // configure MQTT callback
        _mqttClient->onConnect(std::bind(&MoistureSensorStateService::registerConfig, this));

        // configure update handler for when the moisture sensor settings change
        _moistureSensorMqttSettingsService->addUpdateHandler([&](const String& originId) { registerConfig(); }, false);

        // configure settings service update handler to update moisture sensor state
        addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void MoistureSensorStateService::begin() {
    _state.moisture_sensor_on = DEFAULT_MOISTURE_SENSOR_STATE;
    onConfigUpdated();
}

void MoistureSensorStateService::onConfigUpdated() {
    digitalWrite(MOISTURE_SENSOR_POWER_PIN, _state.moisture_sensor_on ? MOISTURE_SENSOR_ON : MOISTURE_SENSOR_OFF);
    long sensor_value;
    static unsigned long oldtime=millis();
    
    for (int i = 0; i <= 100; i++) { 
        // collect 100 read samples and average. read speed is once per 10 ms.
        if ((unsigned long)(millis() - oldtime) >  SAMPLING_PERIOD) {
            oldtime = millis();
            sensor_value += analogRead(MOISTURE_SENSOR_SIG_PIN); 
        } 
    }
    sensor_value /= 100.0; 
    _state.moisture_sensor_value = sensor_value;
}


void MoistureSensorStateService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String subTopic;
  String pubTopic;

  DynamicJsonDocument doc(256);
  _moistureSensorMqttSettingsService->read([&](MoistureSensorMqttSettings& settings) {
    configTopic = settings.mqttPath + "/config";
    subTopic = settings.mqttPath + "/set";
    pubTopic = settings.mqttPath + "/state";
    doc["~"] = settings.mqttPath;
    doc["name"] = settings.name;
    doc["unique_id"] = settings.uniqueId;
  });
  doc["cmd_t"] = "~/set";
  doc["stat_t"] = "~/state";
  doc["schema"] = "json";
  doc["brightness"] = false;

  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(pubTopic, subTopic);
}
