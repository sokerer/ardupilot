/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef __AP_MONITORING_ANALOG_H__
#define __AP_MONITORING_ANALOG_H__

#include "Monitoring.h"
#include "Monitoring_Backend.h"

class AP_Monitoring_analog : public AP_Monitoring_Backend
{
public:
    // constructor
    AP_Monitoring_analog(Monitoring &ranger, uint8_t instance, Monitoring::Monitoring_State &_state);

    // static detection function
    static bool detect(Monitoring &monitor, uint8_t instance);

    // update state
    void update(void);

private:
    // update raw voltage
    void update_voltage(void);

    AP_HAL::AnalogSource *source;
};
#endif // __AP_MONITORING_ANALOG_H__

