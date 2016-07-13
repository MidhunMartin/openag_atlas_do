/**
 *  \file openag_atlas_do.h
 *  \brief Dissolved oxygen sensor.
 */
#ifndef OPENAG_ATLAS_do_H
#define OPENAG_ATLAS_do_H

#include "Arduino.h"
#include <Wire.h>
#include <std_msgs/Float32.h>

/**
 * \brief Eldotrical conductivity sensor.
 */
class AtlasDo {
  public:
    // Constructor
    AtlasDo(int i2c_address); // Default is 97

    // Public variables
    bool has_error;
    char* error_msg;

    // Public functions
    void begin();
    void update();
    bool get_water_dissolved_oxygen(std_msgs::Float32 &msg);

  private:
    // Private variables
    float _water_dissolved_oxygen;
    bool _send_water_dissolved_oxygen;
    uint32_t _time_of_last_reading;
    uint32_t _time_of_last_query;
    bool _waiting_for_response;
    const static uint32_t _min_update_interval = 3000;
    int _i2c_address;

    // Private functions
    void send_query();
    void read_response();
};

 #endif
