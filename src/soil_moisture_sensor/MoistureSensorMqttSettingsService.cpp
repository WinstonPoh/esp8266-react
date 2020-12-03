#include "soil_moisture_sensor/MoistureSensorMqttSettingsService.h"

MoistureSensorMqttSettingsService::MoistureSensorMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(MoistureSensorMqttSettings::read,
                  MoistureSensorMqttSettings::update,
                  this,
                  server,
                  MOISTURE_SENSOR_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(MoistureSensorMqttSettings::read, MoistureSensorMqttSettings::update, this, fs, MOISTURE_SENSOR_BROKER_SETTINGS_FILE) {
}

void MoistureSensorMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
