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

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include "Monitoring.h"
#include "Monitoring_Backend.h"

/*
  base class constructor. 
  This incorporates initialisation as well.
*/
AP_Monitoring_Backend::AP_Monitoring_Backend(Monitoring &_monitor, uint8_t instance, Monitoring::Monitoring_State &_state) :
        monitor(_monitor),
        state(_state) 
{
}

// update status based on distance measurement
void AP_Monitoring_Backend::update_status()
{
    // check distance
    if ((int16_t)state.data_value > monitor._max_data_value[state.instance]) {
        set_status(Monitoring::Monitoring_OutOfRangeHigh);
    } else if ((int16_t)state.data_value < monitor._min_data_value[state.instance]) {
        set_status(Monitoring::Monitoring_OutOfRangeLow);
    } else {
        set_status(Monitoring::Monitoring_Good);
    }
}

// set status and update valid count
void AP_Monitoring_Backend::set_status(Monitoring::Monitoring_Status status)
{
    state.status = status;

    // update valid count
    if (status == Monitoring::Monitoring_Good) {
        if (state.range_valid_count < 10) {
            state.range_valid_count++;
        }
    } else {
        state.range_valid_count = 0;
    }
}
