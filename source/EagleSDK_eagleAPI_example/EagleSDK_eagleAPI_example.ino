/*
    Simple starting point for custom programing.
    Serial commands can be used to give force targets to the motor
    GUI button to put the motor into sleep mode (electrical brake)
    @author Rebecca McWilliam <rmcwilliam@irisdynamics.com>
*/

#include <iriscontrols4.h>  //iris controls 4 library (GUI Handling)
#include <modbus_lib.h>  //modbus client library (UART communications with motors)
#include "client/device_applications/actuator.h" //actuator objects (abstracts communication to motor)
#include "eagleAPI.h"  //custom GUI interface


#define CYCLES_PER_MICRO F_BUS / 1000000L

//Motor connected on port 1 of the Eagle board
Actuator motors[2] {
    {1, "Orca 1", CYCLES_PER_MICRO}
    ,{2, "orca 2", CYCLES_PER_MICRO}
};

//Pass reference of the motor object to the GUI
Gears eagleAPI(motors);
IrisControls4 *IC4_virtual = &eagleAPI;
Actuator::ConnectionConfig connection_config;

void setup() {
    // put your setup code here, to run once:
    //intialize the UART port on the Eagle
   // motors[0].init();

    motors[0].init();
    connection_config.target_baud_rate_bps      = 1040000;
    connection_config.target_delay_us           = 10;
    motors[0].set_connection_config(connection_config);
    //Begin communications with the motor
   // motors[0].enable();
    motors[0].enable();
}

void loop() {
    // put your main code here, to run repeatedly:
    //main code that will set the target position or force of the motor can be run here
    eagleAPI.update_effects();
    motors[0].set_force_mN(eagleAPI.target_force[0]);
    //Parse incoming motor frames
    
    motors[0].run_in();
    //Send out motor frames
    
    motors[0].run_out();
    //Run IrisControls connectivity, GUI element interaction, serial parsing found in Main_GUI.h
    eagleAPI.check();
}

/*
    Interrupts handling for uart 2
    The uart status interrupt is fired when a message byte is either sent or received
    The motor's isr function handles the sending and receiving of bytes
*/
void uart2_status_isr(void) {
    motors[1].isr();
}

/*
    There is also interrupt handling for uart 1
*/
void uart1_status_isr(void) {
    motors[0].isr();
}
