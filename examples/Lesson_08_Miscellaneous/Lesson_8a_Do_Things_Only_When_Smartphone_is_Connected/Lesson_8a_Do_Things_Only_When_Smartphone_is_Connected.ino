/************************************************************
  [Annikken Andee]
  ================
  Lesson 8a
  Do Things Only When Smartphone is Connected
  
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

// We'll have a button to allow the user to disconnect
AndeeHelper disconnectButton;

// We'll just light up an LED to demonstrate this
// LED connected to Pin 2
const int outputPin = 2;

bool runOnce; // Flag to initiate certain actions to run only once

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.setName("Connected Demo");//Set the name of the Andee. This name will appear in the app
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
  
  runOnce = true;
  
  pinMode(outputPin, OUTPUT); // Configures outputPin for output.
  digitalWrite(outputPin, LOW);
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  // We're gonna create two objects. The first object, displayState, will
  // read the current state of the pin and report it to your smartphone.
  // The second object, togglebutton, will be used to turn the LED/relay
  // on or off.
  
  disconnectButton.setId(0);// Don't forget to assign a unique ID number
  disconnectButton.setType(BUTTON_IN);
  disconnectButton.setLocation(0,0,FULL);
  disconnectButton.setTitle("Disconnect");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  // If you need the Andee to run some things only once upon connection,
  // you'll need to do something like this:
  if( Andee.isConnected() && runOnce == true )
  {
    // We'll blink an LED to demonstrate the run once procedure
    for(int i = 0; i < 10; i++)
    {
      digitalWrite(outputPin, HIGH);
      delay(300);
      digitalWrite(outputPin, LOW);
      delay(300);
    }
    
    runOnce = false;
  }
  
  // All code that's in here will only run when smartphone is connected  
  if( Andee.isConnected() )
  {
    if( disconnectButton.isPressed() )
    {
      disconnectButton.ack();
      Andee.disconnect(); // Disconnect user
    }
    
    digitalWrite(outputPin, HIGH);
    
    disconnectButton.update();
  }

  // Arduino will only do these things when Andee is disconnected
  if( !Andee.isConnected() ) 
  {
    digitalWrite(outputPin, LOW);
    
    // Reset flag so that Andee will run some actions at the next connection
    runOnce = true; 
  }

  delay(500); // Always leave a short delay for Bluetooth communication
}



