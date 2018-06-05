/************************************************************
  [Annikken Andee]
  ================
  Lesson 2c
  Programming Single- and Double-Press Buttons
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// In this lesson, you'll learn how to create a button with two
// separate actions that you can execute depending on whether you
// do a single press or a double-press (i.e. double-click).

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <SPI.h>
#include <Andee.h>

// We'll create a display and a button to show you how to
// program a button to do something
AndeeHelper displaybox;
AndeeHelper button;

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.setName("MultiplePress Demo");//Set the name of the Andee. This name will appear in the app
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  // Let's draw a display box!
  displaybox.setId(0);  // Each object must have a unique ID number
  displaybox.setType(DATA_OUT);  // This defines your object as a display box
  displaybox.setLocation(0, 0, FULL); 
  displaybox.setTitle("Single and Double Press Buttons"); 
  displaybox.setData("Waiting for button press"); 
  
  button.setId(1); // Don't forget to assign a unique ID number
  button.setType(BUTTON_IN); // Defines object as a button
  button.setLocation(1,0,FULL);
  button.setTitle("Press me!");
  button.requireAck(false); // You need this line to allow for multiple button presses
  // You can't use setData() and setUnit() for buttons.
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{  
  int buttonPressed = button.isPressed();//isPressed() can also return the number of times the button is pressed
  
  // Here's how you code with a multi-press button:
  if( buttonPressed == 1 )
  {
    // Add single press action here!
    displaybox.setData("Single Press");
    displaybox.update();
    delay(500);
    displaybox.setData("Waiting for button press");
    displaybox.update();
  }
  else if( buttonPressed > 1 )
  {
    // Add double press action here!
    displaybox.setData("Double Press");
    displaybox.update();
    delay(500);
    displaybox.setData("Waiting for button press");
    displaybox.update();    
  }
  // You can potentially code actions for triple press and quadruple press.
  
  displaybox.update(); // Don't forget to call update() or else your objects won't show up
  button.update();
  
  delay(500); // Always leave a short delay for Bluetooth communication
}



