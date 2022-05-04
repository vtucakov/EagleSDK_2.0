# Introduction
To clone this repo with all included submodules use git bash command
<code> git clone https://github.com/irisdynamics/EagleSDK_2.0 --recurse-submodules </code>

Note: If you do a regular clone on this repo the submodules folder will be empty, you can populate them by running the following command in the git bash window
git submodule update --init 

## WARNING!! 
Be aware that the shaft or motor will move during operation, and position your hands and other sensitive stuff accordingly. 
This firmware will cause the motor to create forces and motion. Be very careful at all times. Ensure the shaft and motor are mounted in a safe location and not in danger of hitting people or property.

## Description
The SDK firmware runs on an Eagle hardware development board and is meant to provide basic handshaking and communication Iris Orca motor. It also includes libraries to allow development of custom GUI interfaces 
The package contains some example with source files and builds 

# Quickstart
IrisControls4.exe, located in the IrisControls4 folder of this repo, is a Windows-based user-interface application used by Iris Dynamics devices to present controls and data over USB.
Running the .exe will open an IrisControls app window, GUI elements will be populated by the firmware on the Eagle Controller once it is connected via USB. 

Other prebuilt examples can be loaded by following the instructions in "Using Pre-Built Firmware" 
For modifying the firmware, follow the instructions in "Environment Setup", and find examples projects in the source folder. 

## Connecting to an Eagle
### The USB connectors 
Currently, only USB Serial is enabled for PC-Eagle communications.  Examples of receiving and sending serial messages are throughout the SDK.  The IrisControls4 applications also uses the USB Serial link to provide the Eagle's GUI. 

To connect from the Eagle to a PC you will need to use either a USB micro or a USB B cable. Ensure that they are data cables and not charging charging cables.
USB micro: powers & grounds Eagle
USB B	 : does not power & ground Eagle, must be powered using the "Power Lugs"

The PC and Eagle ground must similar for USB communications to work. Only one connection should be used at once. Both route to the same USB port on the Eagle. 

### Power Lugs
The power lugs on the Eagle Controller can take an input voltage of 12 - 24 V. The supply can be an AC-DC converter or a battery system.
Once the Eagle has been programmed with appropriate firmware it may be operated without a usb connection to a PC/IrisControls if it is powered throught the through the power lugs.
### Back-power 
The motors in some circumstances will act like generators, this can result in current being fed backwards to  power lugs. Typically this power is low and power supplies or batteries can accommodate this. In situations where shaft or stator speeds get very high, some power supplies may go into protection. 

## Connecting to a Motor

### The RJ45 Connectors (Motor to Eagle)
Connect the port on the Eagle marked "1" and/or "2" on the Eagle case to the motor(s) (These numbers signify the UART channel being used)
If using a power supply of 24V, the fused screw terminals on the Eagle can be used to provide power to the motors(s).
The motors can also be powered directly using a 24V-48V power supply, preferrably through a fuse.
!Important! Do not use a 48V to power a motor through the Eagle Controller.

## Building and Deploying

### Using Pre-Built Firmware 
Precompiled image(s) of firmware examples are available in the /builds directory. 
1. Open the Teensy.exe application.
2. Drag the desired firmware image (a .hex file) from the \builds folder onto the Teensy.exe. The filename should show up in the window. 
3. Press the "Auto" button on Teensy.exe so that it is bright green.
4. Install a bootloader board (small board with button, connects to headers near usb ports) onto the Eagle that is to be programmed.
5. Plug the Eagle into the PC by USB (and power if using the isolated USB port).
6. Press the button on the bootloader board to start programming. 

### Environment Setup 
To compile and build the firmware for the Eagle, the Arduino environment must be set up like so.
Install Arduino (version 1.8.19) :  https://www.arduino.cc/en/software

Ensure that arduino is installed through the arduino website and not through Windows app store.

Install Teensyduino (version 1.56)  : https://www.pjrc.com/teensy/td_download.html  
Navigate to the basic example found in source/EagleSDK_basic_example/EagleSDK_basic_example.ino 

In the IDE:
Change the sketchbook location to this repo's location <code>  File -> preferences  </code>
Change the target to  <code> Tools->board-> select "Teensy 3.2 / 3.1" </code>
From here, if everything is just right, the code can be verified (compiled) with CTRL+R or the check mark button
With Teensyduino installed, verifying the code opens the Teensy.exe app and loads in the hex that was just build. 

Press the button on the Eagle Bootloader to flash the code onto the board. 

Note: The hex file will save to C:\Users\user_name\AppData\Local\Temp\arduino_build_xxxxx
This can be manually loaded to the Teensy.exe app by either dragging the file onto the window or using File -> Open HEX file and navigating to the file location.

## Creating Custom Applications
The entire SDK source code can be modified and used as desired, however the best place to start is in the "source" folder where examples are available. 

EagleSDK_basic_example: motor position and force control example with up to 2 motors, feedback from the motor is provided through plots and data gui elements, there is also use of motor and gui control via serial commands

EagleSDK_GUI_example: extensive example of gui library capabilities semihosted on the IrisControls4.exe windows application showing some of this system's real-time data, and providing examples of creating/using GUI elements.

## Talking with Motors (Orca API)
The Actuator object is used to abstract communication to the motors, detailed information about the use of this object is found in the Orca API User Guide

## Good Luck!
We look forward to your feedback and hope this gets you on track to success with our technology :)
