

/**
    @file Main_GUI.h
    @author Rebecca McWilliam
    @brief Main application that populates the gui elements when iriscontrols is connected
    Any other behaviour that either needs to happen in or out of the gui, or during a connection or disconnection from iriscontrols can be done here

    @version 1.0
*/

#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include <iriscontrols4.h>
#include <device_drivers/k20/ic4_k20.h>
#include "client/device_applications/actuator.h"
/**
    @file Main_GUI.h
    @class GUI
    @brief Class makes and updates elements in the GUI.
*/
class GUI : public IC4_k20 {

        uint16_t last_GUI_update;
        Actuator& motor; 
        FlexButton sleep_button;

    public:
        uint32_t tnow = millis();
        int32_t target_force;
        
        GUI(
            Actuator& _motor
        ):
            motor(_motor)
        {
            //This will change name that will show up in the com port drop down
            set_device_id("Eagle");    
        }

        /*
            @brief Setup the gui panels
        */
        void init() {
            //label on the button: "Sleep" 
            //Toggle state : -1 (not toggleable), 0 (toggled off), 1 (toggled on)
            //Row location : 15 
            //Column location: 25
            //Row span : 2
            //Column span: 4
            sleep_button   .add("Sleep"         ,  -1, 15    , 25     , 2, 4);
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
                                // Reset last update time
                                last_GUI_update = millis();

                                if (sleep_button.pressed()){
                                    motor.set_mode(Actuator::SleepMode);
                                    IC4_virtual->print_l("Motor sleeping");
                                }
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
        //parsing of serial messages
        int parse_app(char * cmd, char * args) {
            std::string command_list;

#define SERIAL_COMMAND(_ARG_)                   \
    COMMAND_IS (  (command_list += (std::string)_ARG_ + (std::string)"\r\n").capacity()  ? _ARG_: _ARG_    ) THEN_DO

            START_PARSING

            COMMAND_IS "hello" THEN_DO
            IC4_virtual->print_l("world");


            ///message of "f 1000" will give a force of 1000 to the motor. Negative value will give force in the opposite direction.
            // shaft should start moving with a value around 400 depending on motor and shaft configuration.
            //this can be typed into the serial box under the console window
            COMMAND_IS "f" THEN_DO
            unsigned int arg_index = 0;
            target_force  = IC4_virtual->parse_int(args, arg_index);        
            motor.set_mode(Actuator::ForceMode);
            
            FINISH_PARSING
        }

};

#endif
