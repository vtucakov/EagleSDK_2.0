

/**
    @file ActuatorApp_GUI.h
    @author Kali Erickson, Rebecca McWilliam
    @brief Main application that populates the gui elements when iriscontrols is connected
    Any other behaviour that either needs to happen in or out of the gui, or during a connection or disconnection from iriscontrols can be done here

    @version 1.0
*/

#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include <iriscontrols4.h>
#include <device_drivers/k20/ic4_k20.h>
#include "client/device_applications/actuator.h"
#include "Motor_Plot_Panel.h"
#include "Motor_Control_Panel.h"
/**
    @file Main_GUI.h
    @class GUI
    @brief Class makes and updates elements in the GUI.
*/
class GUI : public IC4_k20 {

        uint16_t last_GUI_update;
        Actuator* motors;

    public:
        Motor_Plot motor_plot[2];
        Motor_Control motor_control[2];
        uint32_t tnow = millis();


        GUI(
            Actuator* _motors
        ):
            motors(_motors)
            , motor_plot    {{motors[0], "Motor 1", 1, 19}  , {motors[1], "Motor 2", 16, 19}}      //initialize gui panels with row and column anchor points
        , motor_control {{motors[0], 3, 43}             , {motors[1], 18, 43}}                 //initialize gui panels with row and column anchor points
        {
            set_server_name("Basic Example");
            set_device_id("Eagle");    
        }

        /*
            @brief Setup the gui panels
        */
        void init() {
            motor_control[0].setup();
            motor_control[1].setup();
            motor_plot[0].setup();
            motor_plot[1].setup();
        }

        /*
            @Brief Handles
            Runs the run function of the current panels
        */
        void run() {
            check();
            switch (gui_frame_state) {
                ////////////// RX STATE /////////////////////////////////
                case rx: {

                        /////////////////// IRIS CONTROLS IS TIMED OUT ////////////////////
                        if ( is_timed_out() ) {

                            // DO THINGS WHEN TIMED OUT...

                            set_disconnected();
                        }
                        break;
                    } // RX CASE

                ////////////// TX STATE /////////////////////////////////
                case tx: {

                        /////////////////// IRIS CONTROLS RECONNECT AFTER TIMEOUT /////////
                        if ( restored_connection()) {
                            print_l("\r\r==== Iris Controls Was Timed Out ====\r\r");

                            //DO THINGS...

                        }

                        /////////////////// IRIS CONTROLS HAS ESTABLISHED CONNECTION //////
                        if (new_connection()) {

                            gui_set_grid(30, 60);
                            init();
                            last_GUI_update = millis();

                        }

                        /////////////////// IRIS CONTROLS REGULAR UPDATES /////////////////
                        if (is_connected() ) {

                            tnow = millis();
                            static uint32_t gui_timer = 0;
                            static uint8_t gui_update_period = 20;
                            if ( (uint32_t)(tnow - gui_timer) > gui_update_period ) {
                                gui_timer = tnow;
                                motor_plot[0].run();
                                motor_plot[1].run();
                                motor_control[0].run();
                                motor_control[1].run();
                                // Reset last update time
                                last_GUI_update = millis();

                                // Transmit end of transmission message which signals IC4 to transmit in half duplex
                                end_of_frame();

                            }
                        }
                        send();
                        break;
                    } // TX CASE
            } // SWITCH
        } // run()

    protected:

        int parse_app(char * cmd, char * args) {
            std::string command_list;

#define SERIAL_COMMAND(_ARG_)                   \
    COMMAND_IS (  (command_list += (std::string)_ARG_ + (std::string)"\r\n").capacity()  ? _ARG_: _ARG_    ) THEN_DO

            START_PARSING

            COMMAND_IS "a" THEN_DO
            IC4_virtual->print_l("bc");

            COMMAND_IS "f" THEN_DO
            IC4_virtual->print_l("\rforce mode");
            unsigned int arg_index = 0;
            int16_t id     = IC4_virtual->parse_int(args, arg_index) -1;
            int32_t force  = IC4_virtual->parse_int(args, arg_index);
        
            motor_control[id].force_target = force;
            motor_control[id].target_force.update(force);
            motors[id].set_mode(Actuator::ForceMode);
            motor_control[id].sleep_button.disable(false);
            motor_control[id].position_button.disable(false);
            motor_control[id].force_button.disable(true);
            motor_control[id].target_force.disable(false);
            motor_control[id].target_position.disable(true);
            
            COMMAND_IS "s" THEN_DO
            IC4_virtual->print_l("\rsleep");
            motors[0].set_mode(Actuator::SleepMode);
            motors[1].set_mode(Actuator::SleepMode);

            COMMAND_IS "help" THEN_DO
            IC4_virtual->print_l("\rforce command : 'f motor# force_value'");
            IC4_virtual->print_l("\rsleep command : 's'");
            
            FINISH_PARSING
        }

};

#endif
