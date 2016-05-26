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

#ifndef __MONITORING_H__
#define __MONITORING_H__

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_SerialManager/AP_SerialManager.h>

// Maximum number of range finder instances available on this platform
#define MONITORING_MAX_INSTANCES 4

class AP_Monitoring_Backend;
 
class Monitoring
{
public:
    friend class AP_Monitoring_Backend;

    Monitoring(AP_SerialManager &_serial_manager);

    // Monitoring driver types
    enum Monitoring_Type {
        Monitoring_TYPE_NONE   = 0,
        Monitoring_TYPE_ANALOG = 1







    };

    enum Monitoring_Function {
        FUNCTION_LINEAR    = 0,
        FUNCTION_INVERTED  = 1,
        FUNCTION_HYPERBOLA = 2
    };

    enum Monitoring_Status {
        Monitoring_NotConnected = 0,
        Monitoring_NoData,
        Monitoring_OutOfRangeLow,
        Monitoring_OutOfRangeHigh,
        Monitoring_Good
    };

    // The Monitoring_State structure is filled in by the backend driver
    struct Monitoring_State {
        uint8_t                instance;    // the instance number of this Monitoring
        uint16_t               data_value; // value of data monitored
        uint16_t               voltage_mv;  // voltage in millivolts,
                                            // if applicable, otherwise 0
        enum Monitoring_Status status;     // sensor status
        uint8_t                range_valid_count;   // number of consecutive valid readings (maxes out at 10)
//        bool                   pre_arm_check;   // true if sensor has passed pre-arm checks
//        uint16_t               pre_arm_distance_min;    // min distance captured during pre-arm checks
//        uint16_t               pre_arm_distance_max;    // max distance captured during pre-arm checks
    };

    // parameters for each instance
    AP_Int8  _type[MONITORING_MAX_INSTANCES];
    AP_Int8  _pin[MONITORING_MAX_INSTANCES];
    AP_Int8  _ratiometric[MONITORING_MAX_INSTANCES];
    AP_Int8  _stop_pin[MONITORING_MAX_INSTANCES];
    AP_Int16 _settle_time_ms[MONITORING_MAX_INSTANCES];
    AP_Float _scaling[MONITORING_MAX_INSTANCES];
    AP_Float _offset[MONITORING_MAX_INSTANCES];
    AP_Int8  _function[MONITORING_MAX_INSTANCES];
    AP_Int16 _min_data_value[MONITORING_MAX_INSTANCES];
    AP_Int16 _max_data_value[MONITORING_MAX_INSTANCES];

    static const struct AP_Param::GroupInfo var_info[];
    
    // Return the number of range finder instances
    uint8_t num_sensors(void) const {
        return num_instances;
    }

    // detect and initialise any available rangefinders
    void init(void);

    // update state of all rangefinders. Should be called at around
    // 10Hz from main loop
    void update(void);
    
#define _Monitoring_STATE(instance) state[instance]

    uint16_t data_value(uint8_t instance) const {
        return (instance<num_instances? _Monitoring_STATE(instance).data_value : 0);
    }
    uint16_t data_value() const {
        return data_value(primary_instance);
    }

    uint16_t voltage_mv(uint8_t instance) const {
        return _Monitoring_STATE(instance).voltage_mv;
    }
    uint16_t voltage_mv() const {
        return voltage_mv(primary_instance);
    }

    int16_t max_data_value(uint8_t instance) const {
        return _max_data_value[instance];
    }
    int16_t max_data_value() const {
        return max_data_value(primary_instance);
    }

    int16_t min_data_value(uint8_t instance) const {
        return _min_data_value[instance];
    }
    int16_t min_data_value() const {
        return min_data_value(primary_instance);
    }







    // query status
    Monitoring_Status status(uint8_t instance) const;
    Monitoring_Status status(void) const {
        return status(primary_instance);
    }

    // true if sensor is returning data
    bool has_data(uint8_t instance) const;
    bool has_data() const {
        return has_data(primary_instance);
    }

    // returns count of consecutive good readings
    uint8_t range_valid_count() const {
        return range_valid_count(primary_instance);
    }
    uint8_t range_valid_count(uint8_t instance) const {
        return _Monitoring_STATE(instance).range_valid_count;
    }
















private:
    Monitoring_State state[MONITORING_MAX_INSTANCES];
    AP_Monitoring_Backend *drivers[MONITORING_MAX_INSTANCES];
    uint8_t primary_instance:3;
    uint8_t num_instances:3;

    AP_SerialManager &serial_manager;

    void detect_instance(uint8_t instance);
    void update_instance(uint8_t instance);  


};
#endif // __MONITORING_H__
