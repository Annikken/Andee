#include <Andee.h>
#include <SPI.h>

AndeeHelper displaybox;
AndeeHelper button;

void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  Andee.setName("TTS Demo");//Set the name of the Andee. This name will appear in the app
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the appearance of
// all the objects on your smartphone
void setInitialData()
{
  displaybox.setId(0);
  displaybox.setType(DATA_OUT);
  displaybox.setTitle("Text to Speech");
  displaybox.setData("Be sure to unmute your phone to hear your phone talk!");  
  
  button.setId(1); // Don't forget to assign a unique ID number
  button.setType(BUTTON_IN); // Defines object as a button
  button.setLocation(1,0,FULL);
  button.setTitle("Say something!");
  button.setColor(THEME_RED_DARK);
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( button.isPressed() )
  {
    button.ack();
    Andee.textToSpeech("Hello oo oo oo", 1.0, 1.0, GREAT_BRITON);  
  }
  
  displaybox.update();
  button.update();
  
  delay(500); // Always leave a short delay for Bluetooth communication
}
