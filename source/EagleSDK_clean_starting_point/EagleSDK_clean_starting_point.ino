
#include <iriscontrols4.h>  //iris controls 4 library (GUI Handling)
#include <modbus_lib.h>  //modbus client library (UART communications with motors)
#include "client/device_applications/actuator.h" //actuator objects (abstracts communication to motor)
#include "Main_GUI.h"  //custom GUI interface


#define CYCLES_PER_MICRO F_BUS / 1000000L

//Motor connected on port 1 of the Eagle board
Actuator motor(1, "Motor 1", CYCLES_PER_MICRO);

//Pass reference of the motor object to the GUI
GUI gui(motor);
IrisControls4 *IC4_virtual = &gui;

void setup() {
    // put your setup code here, to run once:
    //intialize the UART port on the Eagle
    motor.init();
    //Begin communications with the motor
    motor.enable();
}

void loop() {
    // put your main code here, to run repeatedly:
    //main code that will set the target position or force of the motor can be run here
    motor.set_force_mN(gui.target_force);       //set the motor force to the value set in the gui object
                                                //this value is being set in the parse_app function
    //Parse incoming motor frames
    motor.run_in();
    //Send out motor frames
    motor.run_out();
    //Run IrisControls connectivity, GUI element interaction, serial parsing found in Main_GUI.h 
    gui.run();
}

/*
    Interrupts handling for uart 1
    The uart status interrupt is fired when a message byte is either sent or received
    The motor's isr function handles the sending and receiving of bytes
*/
void uart1_status_isr(void) {
    motor.isr();
}
