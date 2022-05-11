

/**
    @file Main_GUI.h
    @author Rebecca McWilliam
    @brief Main application that populates the gui elements when iriscontrols is connected
    Any other behaviour that either needs to happen in or out of the gui, or during a connection or disconnection from iriscontrols can be done here

    @version 1.0
*/

#ifndef GEARS_H_
#define GEARS_H_

#include <iriscontrols4.h>
#include <device_drivers/k20/ic4_k20.h>
#include "client/device_applications/actuator.h"
#include "Spring.h"
#include "Motion.h"
#include "Damping.h"
#include "Periodic.h"
/**
    @file gears.h
    @class Gears
    @brief Class makes and updates elements in the GUI.
*/
class Gears : public IC4_k20 {

        uint16_t last_GUI_update;
        Actuator* motors; 
        FlexButton sleep_button;
        SpringEffect spring;
        Derivative speed;
        DampingEffect damper;
        SineEffect sine;
        int32_t friction;

    public:
        //uint32_t tnow = millis();
        int32_t target_force[2];
        
        Gears(
            Actuator* _motor
        ):
                motors(_motor)
            ,   spring()
            ,   speed (0.02)
            ,   damper()
        {
            //This will change name that will show up in the com port drop down
           // set_device_id("Eagle");    
        }

    void update_effects(){
        spring.update(motors[0].get_position_um());
        speed.update(motors[0].get_position_um());
        damper.update(speed.get());
        sine.update();
        target_force[0] = spring.getForce() + damper.getForce() + sine.getForce() + friction;
    }
    
    
    protected:
        //parsing of serial messages
        int parse_app(char * cmd, char * args) {
            std::string command_list;

#define SERIAL_COMMAND(_ARG_)                   \
    COMMAND_IS (  (command_list += (std::string)_ARG_ + (std::string)"\r\n").capacity()  ? _ARG_: _ARG_    ) THEN_DO

            START_PARSING

            COMMAND_IS "iris" THEN_DO
            print_serial(" dynamics");


            ///message of "f 1000" will give a force of 1000 to the motor. Negative value will give force in the opposite direction.
            // shaft should start moving with a value around 400 depending on motor and shaft configuration.
            //this can be typed into the serial box under the console window
            COMMAND_IS "f" THEN_DO
            unsigned int arg_index = 0;
            target_force[0]  = IC4_virtual->parse_int(args, arg_index);  
            print_serial("testing");      
            motors[0].set_mode(Actuator::ForceMode);


            COMMAND_IS "x" THEN_DO
            bool act[3];
            uint8_t control[3];
            int i = 0;
            int j = 0;
            int values_array[100];
            i++;
            act[0] = args[i] & (1<<0);
            act[1] = args[i] & (1<<1);
            act[2] = args[i] & (1<<2);
            i++;
            print_c(FLAG);
            print_c(args[1]);
            if (act[0]){
                control[0] = args[i];
                if (control[0] & (1<<1)) motors[0].set_mode(Actuator::ForceMode);
                if (control[0] & (1<<0)) motors[0].set_mode(Actuator::SleepMode);
                if (control[0] & (1<<3)) motors[0].zero_position();
    
                i++;
                spring.moveCenter ((int)((args[i]<<8) + args[i+1]) * 1000);
                i+=2;
                spring.adjustK (-((args[i]<<8) + args[i+1])/1000.);
                i+=2;
                damper.adjustK (-((args[i]<<8) + args[i+1])/1000.);
                i+=2;
                sine.amplitude ( (args[i]<<8)+args[i+1]);
                i+=2;
                sine.frequency ( (args[i]<<8) + args[i+1]);
                i+=2;
                values_array[j] = ( (args[i]<<8)+args[i+1]);
                i+=2;
                values_array[j] = ( (args[i]<<8) + args[i+1]); 
                i+=2;
                friction = ( (args[i]<<8) + args[i+1]); 
                i+=2;
                data_return_frame(motors[0]);           
            }
            if (act[1]){
                control[1] = args[i];
                if (control[1] & (1<<1)) motors[1].set_mode(Actuator::ForceMode);
                if (control[1] & (1<<0)) motors[1].set_mode(Actuator::SleepMode);
                if (control[1] & (1<<3)) motors[1].zero_position();
    
                i++;
                spring.moveCenter ((int)((args[i]<<8) + args[i+1]) * 1000);
                i+=2;
                spring.adjustK (-((args[i]<<8) + args[i+1])/1000.);
                i+=2;
                damper.adjustK (-((args[i]<<8) + args[i+1])/1000.);
                i+=2;
                sine.amplitude ( (args[i]<<8)+args[i+1]);
                i+=2;
                sine.frequency ( (args[i]<<8) + args[i+1]);
                i+=2;
                values_array[j] = ( (args[i]<<8)+args[i+1]);
                i+=2;
                values_array[j] = ( (args[i]<<8) + args[i+1]); 
                i+=2;
                friction = ( (args[i]<<8) + args[i+1]); 
                i+=2;
                data_return_frame(motors[1]);
            }            
            print_c(TRAILER);
            send();
            FINISH_PARSING
        }


        void data_return_frame(Actuator& motor){
                print_c((uint8_t)(motor.get_position_um()>>24));
                print_c((uint8_t)(motor.get_position_um()>>16));
                print_c((uint8_t)(motor.get_position_um()>>8));
                print_c((uint8_t)(motor.get_position_um()));
                print_c((uint8_t)(motor.get_force_mN()>>24));
                print_c((uint8_t)(motor.get_force_mN()>>16));
                print_c((uint8_t)(motor.get_force_mN()>>8));
                print_c((uint8_t)(motor.get_force_mN()));
                print_c((uint8_t)(motor.get_power_W()>>8));
                print_c((uint8_t)(motor.get_power_W()));
                print_c((uint8_t)motor.get_temperature_C());
                print_c((uint8_t)(motor.get_voltage_mV()>>8));
                print_c((uint8_t)(motor.get_voltage_mV()));
                print_c((uint8_t)(motor.get_errors()>>8));
                print_c((uint8_t)(motor.get_errors()));
                print_c((uint8_t)(9>>8));
                print_c((uint8_t)(5));
        }

};

#endif
