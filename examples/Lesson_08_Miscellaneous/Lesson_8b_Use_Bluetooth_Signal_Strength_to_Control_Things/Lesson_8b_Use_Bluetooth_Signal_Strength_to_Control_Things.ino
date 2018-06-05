/************************************************************
  [Annikken Andee]
  ================
  Lesson 8b
  Use Bluetooth Signal Strength to Control Things
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/

/* You can use this to control relays in your home/office! */

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <SPI.h>
#include <Andee.h>

// We'll have a displaybox to show you the Bluetooth signal strength
AndeeHelper displaybox;

// We'll just light up an LED to demonstrate this
// LED connected to Pin 2
const int outputPin = 2;
signed char signalStrength = 0;

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
  
  pinMode(outputPin, OUTPUT); // Configures outputPin for output.
  digitalWrite(outputPin, LOW);
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  displaybox.setId(0);
  displaybox.setType(DATA_OUT);
  displaybox.setLocation(0,0,FULL);
  displaybox.setTitle("Signal Strength");
  displaybox.setUnit("dB");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( Andee.isConnected() ) // Run only when connected
  {
    // Retrieve Bluetooth information from the Andee and store it in strBuffer
    signalStrength = Andee.getRSSI();    

    // We're gonna use a double threshold line to prevent light flickering.
    // If you use a single threshold, when you stand at the edge of the threshold
    // line, the signal will fluctuate around that value, causing the LED to
    // flicker. This is very undesirable.
    
    if(signalStrength > -60) // The nearer to zero, the closer you are
    {
      digitalWrite(outputPin, HIGH);
    }

    // Andee will not do anything when you are within the -70db to -60db range.

    if(signalStrength < -70) // When you're very far away
    {
      digitalWrite(outputPin, LOW);
    }    
    
    displaybox.setData(signalStrength);
    displaybox.update();
    delay(300);    
  }
  else
  {
    digitalWrite(outputPin, LOW); // Keep the LED off
  }
}



