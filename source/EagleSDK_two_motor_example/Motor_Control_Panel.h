
/**
    @file Motor_Control_Panel.h
    @author  Rebecca McWi,liam <rmcwilliam@irisdynamics.com>
    @version 1.0

    Created on Feb 20, 2022

    @brief Basic sliders to set target positions or forces and switch between modes (Sleep, Position, Force)
*/


#pragma once

#include "Arduino.h"
#include "../ic4_library/device_drivers/k20/IC4_k20.h"
#include "../ic4_library/ic_app.h"
#include "../modbus_client/client/device_applications/actuator.h"
#include <string>


/**
    @brief Each pane object represents one actuator and contains plots and other i/o to inform the user of actuator status

*/
class Motor_Control {

        Actuator & motor;
        
        uint16_t anchor_row, anchor_col;

    public:
        FlexSlider target_force, target_position;
        int32_t force_target = 0, position_target = 0;
        FlexButton enable_button, zero_position, clear_errors, sleep_button, position_button, force_button;
        
        Motor_Control
        (
            Actuator & _motor
            ,   uint16_t _anchor_row
            ,   uint16_t _anchor_col
        )
            :
            motor(_motor)
            ,   anchor_row(_anchor_row)
            ,   anchor_col(_anchor_col)
        {

        }


        void setup();
        void shutdown();
        void run();

};
