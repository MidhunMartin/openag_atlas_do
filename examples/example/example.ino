#include "openag_atlas_do.h"

AtlasDo atlas_do_1(97);

void setup() {
  Serial.begin(9600);
  atlas_do_1.begin();
}

std_msgs::Float32 do_msg;

void loop() {
  atlas_do_1.update();

  if (atlas_do_1.get_water_dissolved_oxygen(do_msg)) {
    Serial.print("Dissolved Oxygen: ");
    Serial.println(do_msg.data);
  }
  if (atlas_do_1.has_error) {
    Serial.print("Error: ");
    Serial.println(atlas_do_1.error_msg);
    atlas_do_1.has_error = false;
  }
}
