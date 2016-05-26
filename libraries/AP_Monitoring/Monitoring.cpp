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

#include "Monitoring.h"
#include "AP_Monitoring_analog.h"








// table of user settable parameters
const AP_Param::GroupInfo Monitoring::var_info[] = {
    // @Param: _TYPE
    // @DisplayName: Rangefinder type
    // @Description: What type of rangefinder device that is connected
    // @Values: 0:None,1:Analog,2:APM2-MaxbotixI2C,3:APM2-PulsedLightI2C,4:PX4-I2C,5:PX4-PWM,6:BBB-PRU,7:LightWareI2C,8:LightWareSerial
    // @User: Standard
    AP_GROUPINFO("_TYPE",    0, Monitoring, _type[0], 1),

    // @Param: _PIN
    // @DisplayName: Rangefinder pin
    // @Description: Analog pin that rangefinder is connected to. Set this to 0..9 for the APM2 analog pins. Set to 64 on an APM1 for the dedicated 'airspeed' port on the end of the board. Set to 11 on PX4 for the analog 'airspeed' port. Set to 15 on the Pixhawk for the analog 'airspeed' port.
    // @Values: -1:Not Used, 0:APM2-A0, 1:APM2-A1, 2:APM2-A2, 3:APM2-A3, 4:APM2-A4, 5:APM2-A5, 6:APM2-A6, 7:APM2-A7, 8:APM2-A8, 9:APM2-A9, 11:PX4-airspeed port, 15:Pixhawk-airspeed port, 64:APM1-airspeed port
    // @User: Standard
    AP_GROUPINFO("_PIN",     1, Monitoring, _pin[0], 1),

    // @Param: _SCALING
    // @DisplayName: Rangefinder scaling
    // @Description: Scaling factor between rangefinder reading and distance. For the linear and inverted functions this is in meters per volt. For the hyperbolic function the units are meterVolts.
    // @Units: meters/Volt
    // @Increment: 0.001
    // @User: Standard
    AP_GROUPINFO("_SCALING", 2, Monitoring, _scaling[0], 1.0f),

    // @Param: _OFFSET
    // @DisplayName: rangefinder offset
    // @Description: Offset in volts for zero distance for analog rangefinders. Offset added to distance in centimeters for PWM and I2C Lidars
    // @Units: Volts
    // @Increment: 0.001
    // @User: Standard
    AP_GROUPINFO("_OFFSET",  3, Monitoring, _offset[0], 0.0f),

    // @Param: _FUNCTION
    // @DisplayName: Rangefinder function
    // @Description: Control over what function is used to calculate distance. For a linear function, the distance is (voltage-offset)*scaling. For a inverted function the distance is (offset-voltage)*scaling. For a hyperbolic function the distance is scaling/(voltage-offset). The functions return the distance in meters.
    // @Values: 0:Linear,1:Inverted,2:Hyperbolic
    // @User: Standard
    AP_GROUPINFO("_FUNCTION", 4, Monitoring, _function[0], 0),

    // @Param: _MIN_CM
    // @DisplayName: Rangefinder minimum distance
    // @Description: Minimum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    // @User: Standard
    AP_GROUPINFO("_MIN_VAL",  5, Monitoring, _min_data_value[0], 0),

    // @Param: _MAX_CM
    // @DisplayName: Rangefinder maximum distance
    // @Description: Maximum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    // @User: Standard
    AP_GROUPINFO("_MAX_VAL",  6, Monitoring, _max_data_value[0], 330),

    // @Param: _STOP_PIN
    // @DisplayName: Rangefinder stop pin
    // @Description: Digital pin that enables/disables rangefinder measurement for an analog rangefinder. A value of -1 means no pin. If this is set, then the pin is set to 1 to enable the rangefinder and set to 0 to disable it. This can be used to ensure that multiple sonar rangefinders don't interfere with each other.
    // @Values: -1:Not Used,50:Pixhawk AUXOUT1,51:Pixhawk AUXOUT2,52:Pixhawk AUXOUT3,53:Pixhawk AUXOUT4,54:Pixhawk AUXOUT5,55:Pixhawk AUXOUT6,111:PX4 FMU Relay1,112:PX4 FMU Relay2,113:PX4IO Relay1,114:PX4IO Relay2,115:PX4IO ACC1,116:PX4IO ACC2
    // @User: Standard
    AP_GROUPINFO("_STOP_PIN", 7, Monitoring, _stop_pin[0], -1),

    // @Param: _SETTLE
    // @DisplayName: Rangefinder settle time
    // @Description: The time in milliseconds that the rangefinder reading takes to settle. This is only used when a STOP_PIN is specified. It determines how long we have to wait for the rangefinder to give a reading after we set the STOP_PIN high. For a sonar rangefinder with a range of around 7m this would need to be around 50 milliseconds to allow for the sonar pulse to travel to the target and back again.
    // @Units: milliseconds
    // @Increment: 1
    // @User: Standard
    AP_GROUPINFO("_SETTLE", 8, Monitoring, _settle_time_ms[0], 0),

    // @Param: _RMETRIC
    // @DisplayName: Ratiometric
    // @Description: This parameter sets whether an analog rangefinder is ratiometric. Most analog rangefinders are ratiometric, meaning that their output voltage is influenced by the supply voltage. Some analog rangefinders (such as the SF/02) have their own internal voltage regulators so they are not ratiometric.
    // @Values: 0:No,1:Yes
    // @User: Standard
    AP_GROUPINFO("_RMETRIC", 9, Monitoring, _ratiometric[0], 1),




















#if MONITORING_MAX_INSTANCES > 1
    // @Param: 2_TYPE
    // @DisplayName: Second Rangefinder type
    // @Description: What type of rangefinder device that is connected
    // @Values: 0:None,1:Analog,2:APM2-MaxbotixI2C,3:APM2-PulsedLightI2C,4:PX4-I2C,5:PX4-PWM,6:BBB-PRU,7:LightWareI2C,8:LightWareSerial
    // @User: Advanced
    AP_GROUPINFO("2_TYPE",    10, Monitoring, _type[1], 1),

    // @Param: 2_PIN
    // @DisplayName: Rangefinder pin
    // @Description: Analog pin that rangefinder is connected to. Set this to 0..9 for the APM2 analog pins. Set to 64 on an APM1 for the dedicated 'airspeed' port on the end of the board. Set to 11 on PX4 for the analog 'airspeed' port. Set to 15 on the Pixhawk for the analog 'airspeed' port.
    // @Values: -1:Not Used, 0:APM2-A0, 1:APM2-A1, 2:APM2-A2, 3:APM2-A3, 4:APM2-A4, 5:APM2-A5, 6:APM2-A6, 7:APM2-A7, 8:APM2-A8, 9:APM2-A9, 11:PX4-airspeed port, 15:Pixhawk-airspeed port, 64:APM1-airspeed port
    // @User: Advanced
    AP_GROUPINFO("2_PIN",     11, Monitoring, _pin[1], 2),

    // @Param: 2_SCALING
    // @DisplayName: Rangefinder scaling
    // @Description: Scaling factor between rangefinder reading and distance. For the linear and inverted functions this is in meters per volt. For the hyperbolic function the units are meterVolts.
    // @Units: meters/Volt
    // @Increment: 0.001
    // @User: Advanced
    AP_GROUPINFO("2_SCALING", 12, Monitoring, _scaling[1], 1.0f),

    // @Param: 2_OFFSET
    // @DisplayName: rangefinder offset
    // @Description: Offset in volts for zero distance
    // @Units: Volts
    // @Increment: 0.001
    // @User: Advanced
    AP_GROUPINFO("2_OFFSET",  13, Monitoring, _offset[1], 0.0f),

    // @Param: 2_FUNCTION
    // @DisplayName: Rangefinder function
    // @Description: Control over what function is used to calculate distance. For a linear function, the distance is (voltage-offset)*scaling. For a inverted function the distance is (offset-voltage)*scaling. For a hyperbolic function the distance is scaling/(voltage-offset). The functions return the distance in meters.
    // @Values: 0:Linear,1:Inverted,2:Hyperbolic
    // @User: Advanced
    AP_GROUPINFO("2_FUNCTION",  14, Monitoring, _function[1], 0),

    // @Param: 2_MIN_CM
    // @DisplayName: Rangefinder minimum distance
    // @Description: Minimum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    // @User: Advanced
    AP_GROUPINFO("2_MIN_CM",  15, Monitoring, _min_data_value[1], 0),

    // @Param: 2_MAX_CM
    // @DisplayName: Rangefinder maximum distance
    // @Description: Maximum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    // @User: Advanced
    AP_GROUPINFO("2_MAX_CM",  16, Monitoring, _max_data_value[1], 330),

    // @Param: 2_STOP_PIN
    // @DisplayName: Rangefinder stop pin
    // @Description: Digital pin that enables/disables rangefinder measurement for an analog rangefinder. A value of -1 means no pin. If this is set, then the pin is set to 1 to enable the rangefinder and set to 0 to disable it. This can be used to ensure that multiple sonar rangefinders don't interfere with each other.
    // @Values: -1:Not Used,50:Pixhawk AUXOUT1,51:Pixhawk AUXOUT2,52:Pixhawk AUXOUT3,53:Pixhawk AUXOUT4,54:Pixhawk AUXOUT5,55:Pixhawk AUXOUT6,111:PX4 FMU Relay1,112:PX4 FMU Relay2,113:PX4IO Relay1,114:PX4IO Relay2,115:PX4IO ACC1,116:PX4IO ACC2
    // @User: Advanced
    AP_GROUPINFO("2_STOP_PIN", 17, Monitoring, _stop_pin[1], -1),

    // @Param: 2_SETTLE
    // @DisplayName: Sonar settle time
    // @Description: The time in milliseconds that the rangefinder reading takes to settle. This is only used when a STOP_PIN is specified. It determines how long we have to wait for the rangefinder to give a reading after we set the STOP_PIN high. For a sonar rangefinder with a range of around 7m this would need to be around 50 milliseconds to allow for the sonar pulse to travel to the target and back again.
    // @Units: milliseconds
    // @Increment: 1
    // @User: Advanced
    AP_GROUPINFO("2_SETTLE", 18, Monitoring, _settle_time_ms[1], 0),

    // @Param: 2_RMETRIC
    // @DisplayName: Ratiometric
    // @Description: This parameter sets whether an analog rangefinder is ratiometric. Most analog rangefinders are ratiometric, meaning that their output voltage is influenced by the supply voltage. Some analog rangefinders (such as the SF/02) have their own internal voltage regulators so they are not ratiometric.
    // @Values: 0:No,1:Yes
    // @User: Advanced
    AP_GROUPINFO("2_RMETRIC", 19, Monitoring, _ratiometric[1], 1),









#endif



















#if MONITORING_MAX_INSTANCES > 2
    // @Param: 3_TYPE
    // @DisplayName: Second Rangefinder type
    // @Description: What type of rangefinder device that is connected
    // @Values: 0:None,1:Analog,2:APM2-MaxbotixI2C,3:APM2-PulsedLightI2C,4:PX4-I2C,5:PX4-PWM,6:BBB-PRU,7:LightWareI2C,8:LightWareSerial
    AP_GROUPINFO("3_TYPE",    20, Monitoring, _type[2], 1),

    // @Param: 3_PIN
    // @DisplayName: Rangefinder pin
    // @Description: Analog pin that rangefinder is connected to. Set this to 0..9 for the APM2 analog pins. Set to 64 on an APM1 for the dedicated 'airspeed' port on the end of the board. Set to 11 on PX4 for the analog 'airspeed' port. Set to 15 on the Pixhawk for the analog 'airspeed' port.
    // @Values: -1:Not Used, 0:APM2-A0, 1:APM2-A1, 2:APM2-A2, 3:APM2-A3, 4:APM2-A4, 5:APM2-A5, 6:APM2-A6, 7:APM2-A7, 8:APM2-A8, 9:APM2-A9, 11:PX4-airspeed port, 15:Pixhawk-airspeed port, 64:APM1-airspeed port
    AP_GROUPINFO("3_PIN",     21, Monitoring, _pin[2], 3),

    // @Param: 3_SCALING
    // @DisplayName: Rangefinder scaling
    // @Description: Scaling factor between rangefinder reading and distance. For the linear and inverted functions this is in meters per volt. For the hyperbolic function the units are meterVolts.
    // @Units: meters/Volt
    // @Increment: 0.001
    AP_GROUPINFO("3_SCALING", 22, Monitoring, _scaling[2], 1.0f),

    // @Param: 3_OFFSET
    // @DisplayName: rangefinder offset
    // @Description: Offset in volts for zero distance
    // @Units: Volts
    // @Increment: 0.001
    AP_GROUPINFO("3_OFFSET",  23, Monitoring, _offset[2], 0.0f),

    // @Param: 3_FUNCTION
    // @DisplayName: Rangefinder function
    // @Description: Control over what function is used to calculate distance. For a linear function, the distance is (voltage-offset)*scaling. For a inverted function the distance is (offset-voltage)*scaling. For a hyperbolic function the distance is scaling/(voltage-offset). The functions return the distance in meters.
    // @Values: 0:Linear,1:Inverted,2:Hyperbolic
    AP_GROUPINFO("3_FUNCTION",  24, Monitoring, _function[2], 0),

    // @Param: 3_MIN_CM
    // @DisplayName: Rangefinder minimum distance
    // @Description: Minimum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    AP_GROUPINFO("3_MIN_CM",  25, Monitoring, _min_data_value[2], 0),

    // @Param: 3_MAX_CM
    // @DisplayName: Rangefinder maximum distance
    // @Description: Maximum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    AP_GROUPINFO("3_MAX_CM",  26, Monitoring, _max_data_value[2], 330),

    // @Param: 3_STOP_PIN
    // @DisplayName: Rangefinder stop pin
    // @Description: Digital pin that enables/disables rangefinder measurement for an analog rangefinder. A value of -1 means no pin. If this is set, then the pin is set to 1 to enable the rangefinder and set to 0 to disable it. This can be used to ensure that multiple sonar rangefinders don't interfere with each other.
    // @Values: -1:Not Used,50:Pixhawk AUXOUT1,51:Pixhawk AUXOUT2,52:Pixhawk AUXOUT3,53:Pixhawk AUXOUT4,54:Pixhawk AUXOUT5,55:Pixhawk AUXOUT6,111:PX4 FMU Relay1,112:PX4 FMU Relay2,113:PX4IO Relay1,114:PX4IO Relay2,115:PX4IO ACC1,116:PX4IO ACC2
    AP_GROUPINFO("3_STOP_PIN", 27, Monitoring, _stop_pin[2], -1),

    // @Param: 3_SETTLE
    // @DisplayName: Sonar settle time
    // @Description: The time in milliseconds that the rangefinder reading takes to settle. This is only used when a STOP_PIN is specified. It determines how long we have to wait for the rangefinder to give a reading after we set the STOP_PIN high. For a sonar rangefinder with a range of around 7m this would need to be around 50 milliseconds to allow for the sonar pulse to travel to the target and back again.
    // @Units: milliseconds
    // @Increment: 1
    AP_GROUPINFO("3_SETTLE", 28, Monitoring, _settle_time_ms[2], 0),

    // @Param: 3_RMETRIC
    // @DisplayName: Ratiometric
    // @Description: This parameter sets whether an analog rangefinder is ratiometric. Most analog rangefinders are ratiometric, meaning that their output voltage is influenced by the supply voltage. Some analog rangefinders (such as the SF/02) have their own internal voltage regulators so they are not ratiometric.
    // @Values: 0:No,1:Yes
    AP_GROUPINFO("3_RMETRIC", 29, Monitoring, _ratiometric[2], 1),









#endif

#if MONITORING_MAX_INSTANCES > 3








    // @Param: 4_TYPE
    // @DisplayName: Second Rangefinder type
    // @Description: What type of rangefinder device that is connected
    // @Values: 0:None,1:Analog,2:APM2-MaxbotixI2C,3:APM2-PulsedLightI2C,4:PX4-I2C,5:PX4-PWM,6:BBB-PRU,7:LightWareI2C,8:LightWareSerial
    AP_GROUPINFO("4_TYPE",    30, Monitoring, _type[3], 0),

    // @Param: 4_PIN
    // @DisplayName: Rangefinder pin
    // @Description: Analog pin that rangefinder is connected to. Set this to 0..9 for the APM2 analog pins. Set to 64 on an APM1 for the dedicated 'airspeed' port on the end of the board. Set to 11 on PX4 for the analog 'airspeed' port. Set to 15 on the Pixhawk for the analog 'airspeed' port.
    // @Values: -1:Not Used, 0:APM2-A0, 1:APM2-A1, 2:APM2-A2, 3:APM2-A3, 4:APM2-A4, 5:APM2-A5, 6:APM2-A6, 7:APM2-A7, 8:APM2-A8, 9:APM2-A9, 11:PX4-airspeed port, 15:Pixhawk-airspeed port, 64:APM1-airspeed port
    AP_GROUPINFO("4_PIN",     31, Monitoring, _pin[3], -1),

    // @Param: 4_SCALING
    // @DisplayName: Rangefinder scaling
    // @Description: Scaling factor between rangefinder reading and distance. For the linear and inverted functions this is in meters per volt. For the hyperbolic function the units are meterVolts.
    // @Units: meters/Volt
    // @Increment: 0.001
    AP_GROUPINFO("4_SCALING", 32, Monitoring, _scaling[3], 1.0f),

    // @Param: 4_OFFSET
    // @DisplayName: rangefinder offset
    // @Description: Offset in volts for zero distance
    // @Units: Volts
    // @Increment: 0.001
    AP_GROUPINFO("4_OFFSET",  33, Monitoring, _offset[3], 0.0f),

    // @Param: 4_FUNCTION
    // @DisplayName: Rangefinder function
    // @Description: Control over what function is used to calculate distance. For a linear function, the distance is (voltage-offset)*scaling. For a inverted function the distance is (offset-voltage)*scaling. For a hyperbolic function the distance is scaling/(voltage-offset). The functions return the distance in meters.
    // @Values: 0:Linear,1:Inverted,2:Hyperbolic
    AP_GROUPINFO("4_FUNCTION",  34, Monitoring, _function[3], 0),

    // @Param: 4_MIN_CM
    // @DisplayName: Rangefinder minimum distance
    // @Description: Minimum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    AP_GROUPINFO("4_MIN_CM",  35, Monitoring, _min_data_value[3], 0),

    // @Param: 4_MAX_CM
    // @DisplayName: Rangefinder maximum distance
    // @Description: Maximum distance in centimeters that rangefinder can reliably read
	// @Units: centimeters
    // @Increment: 1
    AP_GROUPINFO("4_MAX_CM",  36, Monitoring, _max_data_value[3], 330),

    // @Param: 4_STOP_PIN
    // @DisplayName: Rangefinder stop pin
    // @Description: Digital pin that enables/disables rangefinder measurement for an analog rangefinder. A value of -1 means no pin. If this is set, then the pin is set to 1 to enable the rangefinder and set to 0 to disable it. This can be used to ensure that multiple sonar rangefinders don't interfere with each other.
    // @Values: -1:Not Used,50:Pixhawk AUXOUT1,51:Pixhawk AUXOUT2,52:Pixhawk AUXOUT3,53:Pixhawk AUXOUT4,54:Pixhawk AUXOUT5,55:Pixhawk AUXOUT6,111:PX4 FMU Relay1,112:PX4 FMU Relay2,113:PX4IO Relay1,114:PX4IO Relay2,115:PX4IO ACC1,116:PX4IO ACC2
    AP_GROUPINFO("4_STOP_PIN", 37, Monitoring, _stop_pin[3], -1),

    // @Param: 4_SETTLE
    // @DisplayName: Sonar settle time
    // @Description: The time in milliseconds that the rangefinder reading takes to settle. This is only used when a STOP_PIN is specified. It determines how long we have to wait for the rangefinder to give a reading after we set the STOP_PIN high. For a sonar rangefinder with a range of around 7m this would need to be around 50 milliseconds to allow for the sonar pulse to travel to the target and back again.
    // @Units: milliseconds
    // @Increment: 1
    AP_GROUPINFO("4_SETTLE", 38, Monitoring, _settle_time_ms[3], 0),

    // @Param: 4_RMETRIC
    // @DisplayName: Ratiometric
    // @Description: This parameter sets whether an analog rangefinder is ratiometric. Most analog rangefinders are ratiometric, meaning that their output voltage is influenced by the supply voltage. Some analog rangefinders (such as the SF/02) have their own internal voltage regulators so they are not ratiometric.
    // @Values: 0:No,1:Yes
    AP_GROUPINFO("4_RMETRIC", 39, Monitoring, _ratiometric[3], 1),

















#endif
    
    AP_GROUPEND
};

Monitoring::Monitoring(AP_SerialManager &_serial_manager) :
    primary_instance(0),
    num_instances(0),

    serial_manager(_serial_manager)
{
    AP_Param::setup_object_defaults(this, var_info);

    // init state and drivers
    memset(state,0,sizeof(state));
    memset(drivers,0,sizeof(drivers));
}

/*
  initialise the Monitoring class. We do detection of attached range
  finders here. For now we won't allow for hot-plugging of
  rangefinders.
 */
void Monitoring::init(void)
{
    if (num_instances != 0) {
        // init called a 2nd time?
        return;
    }
    for (uint8_t i=0; i<MONITORING_MAX_INSTANCES; i++) {
        detect_instance(i);
        if (drivers[i] != NULL) {
            // we loaded a driver for this instance, so it must be
            // present (although it may not be healthy)
            num_instances = i+1;
        }





        // initialise status
        state[i].status = Monitoring_NotConnected;
        state[i].range_valid_count = 0;
    }
}

/*
  update Monitoring state for all instances. This should be called at
  around 10Hz by main loop
 */
void Monitoring::update(void)
{
    for (uint8_t i=0; i<num_instances; i++) {
        if (drivers[i] != NULL) {
            if (_type[i] == Monitoring_TYPE_NONE) {
                // allow user to disable a rangefinder at runtime
                state[i].status = Monitoring_NotConnected;
                state[i].range_valid_count = 0;
                continue;
            }
            drivers[i]->update();

        }
    }

    // work out primary instance - first sensor returning good data
    for (int8_t i=num_instances-1; i>=0; i--) {
        if (drivers[i] != NULL && (state[i].status == Monitoring_Good)) {
            primary_instance = i;
        }
    }
}
    
/*
  detect if an instance of a rangefinder is connected. 
 */
void Monitoring::detect_instance(uint8_t instance)
{
    uint8_t type = _type[instance];




























































    if (type == Monitoring_TYPE_ANALOG) {
        // note that analog must be the last to be checked, as it will
        // always come back as present if the pin is valid
        if (AP_Monitoring_analog::detect(*this, instance)) {
            state[instance].instance = instance;
            drivers[instance] = new AP_Monitoring_analog(*this, instance, state[instance]);
            return;
        }
    }
}

// query status
Monitoring::Monitoring_Status Monitoring::status(uint8_t instance) const
{
    // sanity check instance
    if (instance >= MONITORING_MAX_INSTANCES) {
        return Monitoring_NotConnected;
    }

    if (drivers[instance] == NULL || _type[instance] == Monitoring_TYPE_NONE) {
        return Monitoring_NotConnected;
    }

    return state[instance].status;
}

// true if sensor is returning data
bool Monitoring::has_data(uint8_t instance) const
{
    // sanity check instance
    if (instance >= MONITORING_MAX_INSTANCES) {
        return Monitoring_NotConnected;
    }
    return ((state[instance].status != Monitoring_NotConnected) && (state[instance].status != Monitoring_NoData));
}










































