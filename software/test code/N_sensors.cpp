#include"N_sensors.h"
#include"N_math.h"
#include"N_defines.h"

lp_filter filter1(BATT_VOLTAGE_READ_FILTER_CONSTANT);
lp_filter filter2(BATT_CURR_READ_FILTER_CONSTANT);

void sensors_init() {
  analogReadResolution(10);
  analogSetPinAttenuation(BATT_VOLTAGE_READ_PIN, ADC_0db);
}

float read_battery_voltage() {
  filter1.set_input(fmap(analogRead(BATT_VOLTAGE_READ_PIN), 0, 1023, 0, 12.2));
  return filter1.get_val();
}

float read_battery_current() {
  filter2.set_input(((analogRead(BATT_CURR_READ_PIN) * 0.0008625) - 2.49) * 10);
  return filter2.get_val();
}
