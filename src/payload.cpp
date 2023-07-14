#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

#include "common.h"
#include "mqtt_credential.h"
#if defined USE_BME && USE_BME == 1
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;
#endif
#if defined USE_BMP && USE_BMP == 1
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

#endif
float temperature, humidity, pressure, altitude;

#define SEALEVELPRESSURE_HPA (1050)

void bm_setup() {
  // put your setup code here, to run once:
  delay(100);
#if defined USE_BME && USE_BME == 1
  bme.begin(0x76);
#endif
#if defined USE_BMP && USE_BMP == 1
  bmp.begin(0x76);
#endif
}

void bme_loop() {
// put your main code here, to run repeatedly:
#if defined USE_BME && USE_BME == 1
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
#endif

#if defined USE_BMP && USE_BMP == 1
  temperature = bmp.readTemperature();
  // humidity = bmp.readHumidity();
  pressure = bmp.readPressure() / 100.0F;
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
#endif
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");
  #if defined USE_BME && USE_BME == 1


  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
#endif
  Serial.println();
  delay(1000);
}

String get_BM_payload() {
  String message = "";
#if defined USE_BME && USE_BME == 1
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
#endif

#if defined USE_BMP && USE_BMP == 1
  temperature = bmp.readTemperature();
  // humidity = bme.readHumidity();
  pressure = bmp.readPressure() / 100.0F;
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
#endif



  // Encoder function
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["dev_id"] = mqtt_client_id;
  JsonObject& iotasync_data = root.createNestedObject("iotasync_data");
  iotasync_data["temp"] = String(temperature);
  iotasync_data["alt"] = String(altitude);

  #if defined USE_BME && USE_BME == 1
  iotasync_data["humidity"] = String(humidity);
  #endif
  iotasync_data["pressure"] = String(pressure);

  char JSONmessageBuffer[200];

  root.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  message += String(JSONmessageBuffer);

  // StaticJsonBuffer<200> jsonBuffer2;
  // JsonObject &root_out = jsonBuffer2.createObject();

  // char JSONmessageBuffer2[200];

  // root_out.printTo(JSONmessageBuffer2, sizeof(JSONmessageBuffer2));
  // message_outer += String(JSONmessageBuffer2);
  Serial.println(message);
  return message;
}