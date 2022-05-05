/*
    Example of using the actuator object to create motor movement with either position control or force control
    Using gui elements panels and communication objects from the eagle sdk libraries
    @author Rebecca McWilliam <rmcwilliam@irisdynamics.com>
*/

#include <iriscontrols4.h>  //iris controls 4 library (GUI Handling)
#include <modbus_lib.h>  //modbus client library (UART communications with motors)
#include "client/device_applications/actuator.h" //actuator objects (abstracts communication to motor)
#include "Main_GUI.h"  //custom GUI interface


#define CYCLES_PER_MICRO F_BUS / 1000000L

/*  Initialize array of actuator objects use 1 up to 3
    The channel or Uart number corresponds to which port on the eagle the motor is plugged into.
*/
Actuator motors[2] {
      {1, "Orca 1", CYCLES_PER_MICRO}
    , {2, "Orca 2", CYCLES_PER_MICRO}
};

/*
    Pass the array of motors to the gui object to be used to update gui elements
*/
GUI gui(motors);
IrisControls4 *IC4_virtual = &gui;
//!< custom communication settings for baud rate and interframe delay to allow faster communication than modbus protocol
Actuator::ConnectionConfig connection_config;
/*
    @brief initialize and enable the motors
    Initializing the motor will set up the UART on the specified port with the default baud rate
    and appropriate interrupts
    Enabling the motor means that discovery pings will begin sending on the motor's port in
    order to handshake with the motor and establish a connection
*/
void setup() {
    gui.set_server_name("Basic Example");
    gui.set_device_id("Eagle");
    /// This section is optional, if not added will default to 625000 bps and 80us
    connection_config.target_baud_rate_bps      = 780000;  //625000 //780000
    connection_config.target_delay_us           = 50;
    motors[0].set_connection_config(connection_config);
    motors[1].set_connection_config(connection_config);
    ///
    motors[0].init();
    motors[0].enable();
    motors[1].init();
    motors[1].enable();
}

/*
    @brief Main loop that runs the gui, creates adn parses motor communication frames
*/
void loop() {
    gui.run();
    motors[0].set_force_mN(gui.motor_control[0].force_target);
    motors[1].set_force_mN(gui.motor_control[1].force_target);
    motors[0].set_position_um(gui.motor_control[0].position_target);
    motors[1].set_position_um(gui.motor_control[1].position_target);

    motors[0].run_in();
    motors[1].run_in();
    motors[0].run_out();
    motors[1].run_out();
}

/*
    Interrupts handling for uart 1
    The uart status interrupt is fired when a message byte is either sent or received
    The motor's isr function handles the sending and receiving of bytes
*/
void uart1_status_isr(void) {
    motors[0].isr();
}

/*
    There is also interrupt handling for uart 2
*/
void uart2_status_isr(void) {
    motors[1].isr();
}
