/************************************************************
  [Annikken Andee]
  ================
  Lesson 9c
  How to Send SMS
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <SPI.h>
#include <Andee.h>

// We'll use a button to send the message
AndeeHelper sendMessage;

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.setName("SMS Demo");//Set the name of the Andee. This name will appear in the app
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  sendMessage.setId(0);
  sendMessage.setType(BUTTON_IN);
  sendMessage.setLocation(0,0,FULL);
  sendMessage.setTitle("Send SMS");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( sendMessage.isPressed() ) // When user presses the send button on phone
  {
    sendMessage.ack(); // Acknowledge button press
    Andee.sendSMS("+6512341234", "Hello oo oo oo oo");
    //Put in your recipient's phone number in the first argument above, 
    //the text message in the second argument
  }

  sendMessage.update();
  
  delay(500); // Always leave a short delay for Bluetooth communication
}



