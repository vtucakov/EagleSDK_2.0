/**
* @file gui.h
* @author Kali Erickson <kerickson@irisdynamics.com>
* @version 1.0
* 
* trademark stuff
* 
* @brief Control flow for IrisControls4 Example GUI
*/

/**
@mainpage Introduction

This is intended to demonstrate how to create and interact with GUI elements on the IrisControls4 application. No actuator control or communications are present here.
Parsing serial commands from the GUI is also demonstrated.

## Versions
1.0.0 - June 2021 - KE
*/

#ifndef GUI_H_
#define GUI_H_

#include "labelsandbuttons.h"
#include "dataandsliders.h"
#include "graphs.h"

/**
* @class GUI
* @brief Contains all graphic elements of the GUI, and the overridden parseApp function for this program
*/
class GUI : public IC4_k20 {

    //classes that represent certain pages of the GUI
    LabelsAndButtons page1;
    DataAndSliders page2;
    Graphs page3;

    // The following are one type of GUI element supported by the SDK: FlexButtons. For initialization syntax and other options, see labelsandbuttons.h
    FlexButton labels_buttons, data_sliders, graphs;

    //to track which page of the gui is requested to be running
    int open_page;

  public:

    /**
    * @brief adds/initializes the FlexElements for the GUI with the desired confugrations and effects
    */
    void initiate() {
      // Here is where the initial state of the GUI is set up
      
      gui_set_grid(33, 61);   //sets the size of the iriscontrols4 window
      
      int x_anchor = 0;
      int y_anchor = 24;

      labels_buttons.add("Labels and Buttons", 1, y_anchor, x_anchor, 2, 5);  //see the labelsandbuttons.h file for add() syntax
      data_sliders.add("Data_and_Sliders", 0, y_anchor, x_anchor + 7, 2, 5);  //note that spaces in element names can also be represented by underscores
      graphs.add("Graphs", 0, y_anchor, x_anchor + 14, 2, 5);

      //note that based on the element type, configuration, and name length, the row and column spans may need to be increased
      
      page3.initiate();
      page3.shutdown();
      page2.initiate();
      page2.shutdown();
      page1.initiate(); //leaves page1/labelsandbuttons page open as the main/entry page

      //example of printing to the console
      print_l("\rWelcome to The IrisControls4 Example GUI!\r\r");
      print_l("Press the buttons below this console to switch pages and explore the different GUI elements.\r\r");
      print_l("Or test this GUI's custom commands by typing any of the following commands below:\r");
      print_l("- test_0\r- test_1 \'an intiger\' \'a decimal\'\r\r");
    }

    /**
    * @brief maintains and updates the states of the GUI elements
    */
    void run() {

      //check if the page has been changed
      change_page();

      //will run only if the respective page is open
      page1.run();
      page2.run();
      page3.run();

    }

    /**
     * @brief this parser contains special text commands for this application
     *
     * Custom serial commands can be added here by following the existing format.
     */
    int parse_app(char* cmd, char* args) override {
      START_PARSING

      COMMAND_IS "test_0" THEN_DO
        print_l("Test Passed!\r");

      COMMAND_IS "test_1" THEN_DO
        unsigned int index = 0;
        int a = IrisControls4::parse_int(args, index);
        double b = IrisControls4::parse_double(args, index);
        print_l("test passed: arg 1 is ");
        print_d(a);
        print_l("; arg 2 is ");
        print_f(b);

      FINISH_PARSING

      return 1;
    }

    /**
    * @brief  Runs the setup/shutdown sequences for the three pages according to the value of open_page.
    */
    void change_page(){
      int page = navigation();
      switch(page){
        case 1:
          if(!page1.isrunning){
            page3.shutdown();
            page2.shutdown();
            page1.setup();
          }
          break;
        case 2:
          if(!page2.isrunning){
            page3.shutdown();
            page1.shutdown();
            page2.setup();
          }
          break;
        case 3:
          if(!page3.isrunning){
            page2.shutdown();
            page1.shutdown();
            page3.setup();
          }
          break;
      }
    }

    /**
    * @brief Determines the current page number based on button toggle logic.
    * @return An integer representing the current page. 0 for main 'Labels and Buttons' page, 1 for 'Data and Sliders' page, 2 for 'Graphs' page
    */
    int navigation(){
      
      if(labels_buttons.toggled()){
        if(labels_buttons.get()){
          open_page = 1;
          data_sliders.set_toggle(0);
          graphs.set_toggle(0);
        } else if(open_page == 1){
          labels_buttons.set_toggle(1);
        }
      }

      if(data_sliders.toggled()){
        if(data_sliders.get()){
          open_page = 2;
          labels_buttons.set_toggle(0);
          graphs.set_toggle(0);
        } else if(open_page == 2){
          data_sliders.set_toggle(1);
        }
      }

      if(graphs.toggled()){
        if(graphs.get()){
          open_page = 3;
          labels_buttons.set_toggle(0);
          data_sliders.set_toggle(0);
        } else if(open_page == 3){
          graphs.set_toggle(1);
        }
      }
      return open_page;
    }

};

#endif
