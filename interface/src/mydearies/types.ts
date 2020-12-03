export interface LightState {
  led_on: boolean;
  led_brightness: string;
  max_brightness: string;
}

export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}

export interface MoistureSensorState {
  moisture_sensor_on: boolean;
  moisture_sensor_value: number;
}

export interface MoistureSensorMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}
