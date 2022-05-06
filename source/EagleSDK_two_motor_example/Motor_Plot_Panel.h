
/**
   @file Motor_Plot_Panel.h
   @author  Dan Beddoes <dbeddoes@irisdynamics.com>
 * @version 1.0
 *
 * Created on July 21, 2021
*/


#pragma once

#include "Arduino.h"
#include "../ic4_library/device_drivers/k20/IC4_k20.h"
#include "../ic4_library/ic_app.h"
#include "../modbus_client/client/device_applications/actuator.h"
#include <string>


#define MOTOR_GUI_PANE_WIDTH        21
#define MOTOR_GUI_PANE_HEIGHT       13


/**
 * @brief Each pane object represents one actuator and contains plots and other i/o to inform the user of actuator status
 *
 */
class Motor_Plot {


public:

    Motor_Plot
    (
            Actuator & _motor
        ,   const  char * _name
        ,   uint16_t _anchor_row
        ,   uint16_t _anchor_col
    )
        :
            motor(_motor)
        ,   name(_name)
        //gui anchor
        ,   anchor_row(_anchor_row)
        ,   anchor_col(_anchor_col)

        //title label
        ,   label()

        //plots
        ,   plot()

        //datas
        ,   temp()
        ,   volt()
        ,   power()

        //sliders
        ,   output()

        //Datasets
        ,   temp_data()
        ,   volt_data()
        ,   power_data()
        ,   position_data()
        ,   output_data()

        ,   plot_toggle_name(plot_toggle_name_base)

    {

    }


    void setup();
    void shutdown();
    void run();
    void update_fps();
    void handle_error_codes();

protected:

    Actuator & motor;
    
    const  char * name;
    
    uint8_t index;

    uint16_t anchor_row, anchor_col;

    FlexLabel label;

    FlexPlot plot;

    FlexData temp;
    FlexData volt;
    FlexData power;
    FlexData errors;
    FlexData fps;

    FlexSlider output;

    Dataset temp_data;
    Dataset volt_data;
    Dataset power_data;
    Dataset position_data;
    Dataset output_data;

    uint32_t prev_status = -1;
    uint32_t prev_errors = -1;
    uint32_t last_data_rate_check = 0;

    const  char * plot_toggle_name;
    const  char * plot_toggle_name_base = "Plot: ";

    // Used to track the motor data rates
    uint16_t last_num_success_msgs; 
 
    enum PARAMETER_STATUS{
        OK      = 0,
        CAUTION = 1,
        WARNING = 2,
    };

private:
    int current_errors = -1;
};
