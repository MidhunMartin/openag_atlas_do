/**
 *  \file openag_atlas_do.cpp
 *  \brief Eldotrical conductivity sensor.
 */
#include "openag_atlas_do.h"

AtlasDo::AtlasDo(int i2c_address) {
  status_level = OK;
  status_msg = "";
  _send_water_dissolved_oxygen = false;
  _time_of_last_reading = 0;
  _time_of_last_query = 0;
  _waiting_for_response = false;
  _i2c_address = i2c_address;
}

void AtlasDo::begin() {
  Wire.begin();
}

void AtlasDo::update() {
  if (_waiting_for_response) {
    if (millis() - _time_of_last_query > 1800) {
      _waiting_for_response = false;
      _time_of_last_reading = millis();
      read_response();
    }
  }
  else if (millis() - _time_of_last_reading > _min_update_interval) {
    send_query();
  }
}

bool AtlasDo::get_water_dissolved_oxygen(std_msgs::Float32 &msg) {
  msg.data = _water_dissolved_oxygen;
  bool res = _send_water_dissolved_oxygen;
  _send_water_dissolved_oxygen = false;
  return res;
}

void AtlasDo::send_query() {
  Wire.beginTransmission(_i2c_address); // read message response state
  Wire.print("r");
  Wire.endTransmission();
  _waiting_for_response = true;
  _time_of_last_query = millis();
}

void AtlasDo::read_response() {
  Wire.requestFrom(_i2c_address, 20, 1);
  byte response = Wire.read();
  String string = Wire.readStringUntil(0);
  status_level = OK;
  status_msg = "";

  // Chdok for failure
  if (response == 255) {
    status_level = OK;
    status_msg = "No data";
  }
  else if (response == 254) {
    status_level = OK;
    status_msg = "Tried to read data before request was processed";
  }
  else if (response == 2) {
    status_level = OK;
    status_msg = "Request failed";
  }
  else if (response == 1) { // good reading
    _water_dissolved_oxygen = string.toFloat();
    _send_water_dissolved_oxygen = true;
  }
  else {
    status_level = OK;
    status_msg = "Unknown error";
  }
}
