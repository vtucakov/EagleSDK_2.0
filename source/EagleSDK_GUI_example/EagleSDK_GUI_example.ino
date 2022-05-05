/**
* @file New_GUI_example.ino
* @brief Application entry point.
* @author Kali Erickson <kerickson@irisdynamics.com>
* @version 1.0
*/

#include <Arduino.h>
#include "iriscontrols4.h"
#include "device_drivers/k20/ic4_k20.h"
#include "gui.h"  //First a class must be defined for your GUI that extends the device driver class (IC4_k20) and therefore the IrisControls4 class aswell

//Now create an instance of this class for the Eagle and IrisControls4 to interact with
GUI gui;
IrisControls4 *IC4_virtual = &gui;  //The ic4_library will access this object through the IC4_virtual pointer, so it must be connected to the class instance that was just declared

//Variable for controlling polling rate
unsigned int last_iriscontrols4_update;

/**
 * @brief Arduino setup function.
 */
void setup() {
  gui.set_server_name("GUI_Example"); // Feel free to change this line... it will dictate what shows up on the IrisControls window
}

/**
 * @brief Arduino loop function
 */
void loop() {
  // parses any incomming serial data from the USB
  gui.check();  
  gui.send();

  // Example of detecting a new conntection to the IrisControls4 app
  if (gui.new_connection()) {
    gui.initiate();           //calls the gui function that will add/make visible all the desired elements
  }

  // Example of checking and updating the IrisControls4 elements peridocially
  if (gui.is_connected() && (millis() - last_iriscontrols4_update > 16)) {
    last_iriscontrols4_update = millis();
    gui.run();                //calls the gui function that will maintain/update all the GUI elements
    gui.end_of_frame();       //Used to track the framerate of the GUI, MUST be included to avoid disconnection
  }

}
