// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *   AP_Monitoring_analog.cpp - rangefinder for analog source
 *
 */

#include <AP_HAL/AP_HAL.h>
#include <AP_Common/AP_Common.h>
#include <AP_Math/AP_Math.h>
#include "Monitoring.h"
#include "AP_Monitoring_analog.h"

extern const AP_HAL::HAL& hal;

/* 
   The constructor also initialises the rangefinder. Note that this
   constructor is not called until detect() returns true, so we
   already know that we should setup the rangefinder
*/
AP_Monitoring_analog::AP_Monitoring_analog(Monitoring &_monitor, uint8_t instance, Monitoring::Monitoring_State &_state) :
    AP_Monitoring_Backend(_monitor, instance, _state)
{
    source = hal.analogin->channel(monitor._pin[instance]);
    if (source == NULL) {
        // failed to allocate a ADC channel? This shouldn't happen
        set_status(Monitoring::Monitoring_NotConnected);
        return;
    }
    source->set_stop_pin((uint8_t)monitor._stop_pin[instance]);
    source->set_settle_time((uint16_t)monitor._settle_time_ms[instance]);
    set_status(Monitoring::Monitoring_NoData);
}

/* 
   detect if an analog rangefinder is connected. The only thing we
   can do is check if the pin number is valid. If it is, then assume
   that the device is connected
*/
bool AP_Monitoring_analog::detect(Monitoring &_monitor, uint8_t instance)
{
    if (_monitor._pin[instance] != -1) {
        return true;
    }
    return false;
}


/*
  update raw voltage state
 */
void AP_Monitoring_analog::update_voltage(void)
{
   if (source == NULL) {
       state.voltage_mv = 0;
       return;
   }
   // cope with changed settings
   source->set_pin(monitor._pin[state.instance]);
   source->set_stop_pin((uint8_t)monitor._stop_pin[state.instance]);
   source->set_settle_time((uint16_t)monitor._settle_time_ms[state.instance]);
   if (monitor._ratiometric[state.instance]) {
       state.voltage_mv = source->voltage_average_ratiometric() * 1000U;
   } else {
       state.voltage_mv = source->voltage_average() * 1000U;
   }
}

/*
  update distance_cm 
 */
void AP_Monitoring_analog::update(void)
{
    update_voltage();
    float v = state.voltage_mv * 0.001f;
    float dist_m = 0;
    float scaling = monitor._scaling[state.instance];
    float offset  = monitor._offset[state.instance];
    Monitoring::Monitoring_Function function = (Monitoring::Monitoring_Function)monitor._function[state.instance].get();
    int16_t max_data_value = monitor._max_data_value[state.instance];

    switch (function) {
    case Monitoring::FUNCTION_LINEAR:
        dist_m = (v - offset) * scaling;
        break;
	  
    case Monitoring::FUNCTION_INVERTED:
        dist_m = (offset - v) * scaling;
        break;

    case Monitoring::FUNCTION_HYPERBOLA:
        if (v <= offset) {
            dist_m = 0;
        }
        dist_m = scaling / (v - offset);
        if (isinf(dist_m) || dist_m > max_data_value * 0.01f) {
            dist_m = max_data_value * 0.01f;
        }
        break;
    }
    if (dist_m < 0) {
        dist_m = 0;
    }
    state.data_value = dist_m * 100.0f;

    // update range_valid state based on distance measured
    update_status();
}

