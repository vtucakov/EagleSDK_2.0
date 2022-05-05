/** 0
    @file Motor_Plot_Panel.h
    @author  Dan Beddoes <dbeddoes@irisdynamics.com>
    @version 1.0

    Created on July 21, 2021
*/

#include "Motor_Plot_Panel.h"


#define PLOT_HEIGHT        10
#define MOTOR_GUI_UPDATE_PERIOD  0.1 * 1000000  // seconds * microseconds/second
#define FORCE_ERROR_MASK    (1<<6)
#define TEMP_ERROR_MASK     (1<<7)
#define POWER_ERROR_MASK    (1<<8)

void Motor_Plot::setup() {
    //flexelements
    label       .add(name,      anchor_row,                     anchor_col, 2, MOTOR_GUI_PANE_WIDTH,     FlexLabel::ALIGN_CENTER);

    plot        .add("",        anchor_row + 2,                 anchor_col, PLOT_HEIGHT, MOTOR_GUI_PANE_WIDTH, -10, 310,
                     FlexPlot::TIMEPLOT +
                     FlexPlot::WALKING);
                     
    volt        .add("",                anchor_row + PLOT_HEIGHT + 2,   anchor_col + 0  , 1, 4,     -1, 1000, "V"       ,   FlexData::UNITS + FlexData::DIGITS_4 + FlexData::PRECISION_1);
    temp        .add("",                anchor_row + PLOT_HEIGHT + 2,   anchor_col + 4  , 1, 4,     -1, 1   , "*degC*"  ,   FlexData::UNITS + FlexData::DIGITS_3);
    power       .add("",                anchor_row + PLOT_HEIGHT + 2,   anchor_col + 8  , 1, 4,     -1, 1   , "W"       ,   FlexData::UNITS + FlexData::DIGITS_4);
    errors      .add("",                anchor_row + PLOT_HEIGHT + 2,   anchor_col + 12 , 1, 4,     -1, 1   , "E"       ,   FlexData::UNITS + FlexData::DIGITS_4);
    fps         .add("",                anchor_row + PLOT_HEIGHT + 2,   anchor_col + 16 , 1, 5,     -1, 1   , "Fps"       ,   FlexData::UNITS + FlexData::DIGITS_4);
    output      .add(" Force",          anchor_row + PLOT_HEIGHT + 3,   anchor_col      , 1, MOTOR_GUI_PANE_WIDTH, -32000, 32000, 0, 1);

    //output element colours
    volt    .set_colour(FlexData::VALUE_TEXT    , OFF_WHITE);
    temp    .set_colour(FlexData::VALUE_TEXT    , OFF_WHITE);
    power   .set_colour(FlexData::VALUE_TEXT    , OFF_WHITE);
    errors  .set_colour(FlexData::VALUE_TEXT    , OFF_WHITE);
    fps     .set_colour(FlexData::VALUE_TEXT    , OFF_WHITE);
    
    label   .set_colour(FlexLabel::TEXT         , 169, 169, 169, 169);
    output  .set_colour(FlexSlider::VALUE_TEXT  , OFF_WHITE);
    output  .set_colour(FlexSlider::SLIDER_SUBPAGE  , TRANS_DARK_GREY);
    output  .set_colour(FlexSlider::SLIDER_ADDPAGE  , TRANS_DARK_GREY);
    output  .set_colour(FlexSlider::HANDLE_DISABLED , RED);

    IC4_virtual->flush();

    //datasets
    temp_data       .add(&plot, "Temp Data"     , "", "Temperature (C)" , Dataset::TIMEPLOT + Dataset::NONE);
    volt_data       .add(&plot, "Volt Data"     , "", "Voltage (mV)"    , Dataset::TIMEPLOT + Dataset::NONE);
    power_data      .add(&plot, "Power Data"    , "", "Power (W)"       , Dataset::TIMEPLOT + Dataset::NONE + Dataset::SECONDARY_Y_AXIS);
    position_data   .add(&plot, "Position Data" , "", "Position(mm)"    , Dataset::TIMEPLOT + Dataset::NONE);
    output_data     .add(&plot, "Force Data", "", "Force (N)"       , Dataset::TIMEPLOT + Dataset::NONE);

    temp_data       .set_max_data_points(25000);
    temp_data       .set_max_data_points(25000);
    volt_data       .set_max_data_points(25000);
    power_data      .set_max_data_points(25000);
    position_data   .set_max_data_points(25000);
    output_data     .set_max_data_points(25000);

    temp_data       .set_colour(ORANGE);
    volt_data       .set_colour(ORANGE);
    power_data      .set_colour(RED);
    position_data   .set_colour(WHITE);
    output_data     .set_colour(ORANGE);
    position_data   .show();
    power_data      .show();

    //plot config
    plot.set_axes_labels(&power_data);
    plot.set_domain(10);
    plot.set_range(0, 300);
    plot.set_secondary_range(0, 1250);
    plot.set_colour(FlexPlot::COLOUR_SET_FLAG::TEXT, 169, 169, 169, 169);

    label.font_size(18);
    last_data_rate_check = millis();
}

void Motor_Plot::shutdown() {

    label       .hide();
    plot        .hide();
    temp        .hide();
    volt        .hide();
    power       .hide();
    errors      .hide();
    fps         .hide();
    output      .hide();
}

void Motor_Plot::run() {
    //Update outputs
    update_fps();
    temp            .update(motor.get_temperature_C     ());
    volt            .update(motor.get_voltage_mV        ());
    power           .update(motor.get_power_W           ());
    errors          .update(motor.get_errors            ());

    output          .update(motor.get_force_mN          ());

    //Update Datasets
    u64 now = IC4_virtual->system_time();

    temp_data           .add_data(now, temp         .get());
    volt_data           .add_data(now, volt         .get());
    power_data          .add_data(now, power        .get());
    position_data       .add_data(now, (int)(motor.get_position_um() / 1000));

    output_data         .add_data(now, output       .get());

    handle_error_codes();
}

/**
    @brief Method that changes the plot title depending on the actuator's state

    Sets the plot title colour to red with an error present, light grey with no errors present

*/
void Motor_Plot::handle_error_codes() {

    current_errors = -1;

    uint16_t errors = motor.get_errors();

    if (current_errors == errors) return;

    if (motor.is_connected()) {
        if (errors)     label.set_colour(FlexLabel::TEXT, RED);
        else            label.set_colour(FlexLabel::TEXT, GREEN);
    }
    else {
        label.set_colour(FlexLabel::TEXT, 169, 169, 169, 169);
    }

    current_errors = errors;
}

/**
 * @brief Updates the datarate FlexDatas on the second row of the ribbon
 */
void Motor_Plot::update_fps(){

    uint32_t tnow = millis();

    uint32_t total_success_rate = 0;

    if ( tnow - last_data_rate_check >= 100 ) {
    
        total_success_rate = 0;
    
        uint16_t num_msgs = motor.get_num_successful_msgs();
        total_success_rate = (uint16_t)(num_msgs - last_num_success_msgs) * 1000 / (uint32_t)( tnow - last_data_rate_check );
        last_num_success_msgs = num_msgs;
    
        fps.update((int)total_success_rate);
        total_success_rate = 0;
        last_data_rate_check = tnow;
    }
}
