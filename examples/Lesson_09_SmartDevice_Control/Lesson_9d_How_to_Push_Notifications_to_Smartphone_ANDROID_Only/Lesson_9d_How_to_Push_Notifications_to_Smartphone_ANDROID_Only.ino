/************************************************************
  [Annikken Andee]
  ================
  Lesson 9d
  How to Push Notifications to Smartphone
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  http://resources.annikken.com

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <SPI.h>
#include <Andee.h>

// We'll use a button to send the message
AndeeHelper btnNotify;

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.setName("Notification Demo");//Set the name of the Andee. This name will appear in the app
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  btnNotify.setId(0);
  btnNotify.setType(BUTTON_IN);
  btnNotify.setLocation(0,0,FULL);
  btnNotify.setTitle("Push Notification"); 
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( btnNotify.isPressed() ) // When user presses the send button on phone
  {
    btnNotify.ack(); // Acknowledge button press
    Andee.notification("Notification Title", "Notification Message", "Text that appears at status bar when notification is pushed"); // Sends the SMS to the recipient
  }

  btnNotify.update();
  
  delay(500); // Always leave a short delay for Bluetooth communication
}



