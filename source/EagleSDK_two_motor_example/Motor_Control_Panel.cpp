/** 0
    @file Motor_Control_Pane.cpp
    @author  Dan Beddoes <dbeddoes@irisdynamics.com>
    @version 1.0

    Created on July 21, 2021
*/

#include "Motor_Control_Panel.h"

void Motor_Control::setup() {
    //buttons
    enable_button   .add("Enable"       ,  1, anchor_row    , anchor_col     , 2, 4);
    zero_position   .add("Zero Pos"     , -1, anchor_row    , anchor_col + 5 , 2, 4);
    clear_errors    .add("Clear Errors" , -1, anchor_row    , anchor_col + 10, 2, 4);
    sleep_button    .add("Sleep"        ,  0, anchor_row + 3, anchor_col     , 2, 4);
    position_button .add("Position"     ,  0, anchor_row + 3, anchor_col + 5 , 2, 4);
    force_button    .add("Force"        ,  0, anchor_row + 3, anchor_col + 10, 2, 4);
    //sliders
    target_force    .add("   Force"     ,     anchor_row + 6, anchor_col     , 1, 16, -10000, 10000 , 0, 1, FlexSlider::ALLOW_INPUT);
    target_position .add("Position"     ,     anchor_row + 8, anchor_col     , 1, 16, 0     , 300000, 0, 1, FlexSlider::ALLOW_INPUT);

    
    sleep_button    .disable(true);
    position_button .disable(false);
    force_button    .disable(false);
    target_force    .disable(true);
    target_position .disable(true);
}


void Motor_Control::shutdown() {
    enable_button   .hide();
    zero_position   .hide();
    clear_errors    .hide();
    sleep_button    .hide();
    position_button .hide();
    force_button    .hide();
    //sliders
    target_force    .hide();
    target_position .hide();


}

void Motor_Control::run() {
    force_target = target_force.get();
    position_target = target_position.get();

    // Handle toggling on of buttons `zero position`, `clear errors`, and `get latched errors`
    if (zero_position.pressed()) {
        motor.zero_position();
    }
    if (clear_errors.pressed()) {
        motor.clear_errors();
    }

    if (enable_button.toggled()) {
        if (enable_button.get()) {
            motor.enable();
        }
        else {
            motor.disable();
        }
    }

    if (sleep_button.pressed()) {
        IC4_virtual->print_l(motor.get_name());
        IC4_virtual->print_l(" Sleep Mode\r");
        sleep_button.disable(true);
        position_button.disable(false);
        force_button.disable(false);
        target_force.disable(true);
        target_position.disable(true);
        motor.set_mode(Actuator::SleepMode);
    }

    if (position_button.pressed()) {
        IC4_virtual->print_l(motor.get_name());
        IC4_virtual->print_l(" Position Mode\r");
        sleep_button.disable(false);
        position_button.disable(true);
        force_button.disable(false);
        motor.set_mode(Actuator::PositionMode);
        motor.tune_position_controller(100, 2, 10, 65535);
        target_force.disable(true);
        target_position.disable(false);
        target_position.update(motor.get_position_um());
    }

    if (force_button.pressed()) {
        IC4_virtual->print_l(motor.get_name());
        IC4_virtual->print_l(" Force Mode\r");
        sleep_button.disable(false);
        position_button.disable(false);
        force_button.disable(true);
        target_force.disable(false);
        target_position.disable(true);
        motor.set_mode(Actuator::ForceMode);
        target_force.update(motor.get_force_mN());
    }
}
