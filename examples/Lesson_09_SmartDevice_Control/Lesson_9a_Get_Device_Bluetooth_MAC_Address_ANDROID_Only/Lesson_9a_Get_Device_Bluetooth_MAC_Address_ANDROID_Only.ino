/************************************************************
  [Annikken Andee]
  ================
  Lesson 9a
  Get Device Bluetooth MAC Address
  
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

// We'll use a display box to show the current MAC address
AndeeHelper displaybox;

const char* MACAddress;//Used to store the MAC address

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the appearance of
// all the objects on your smartphone
void setInitialData()
{
  displaybox.setId(0);
  displaybox.setType(DATA_OUT);
  displaybox.setLocation(0,0,FULL);
  displaybox.setTitle("Connected MAC Address");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if( Andee.isConnected() ) // Do this only when Andee is connected
  {
    // Get MAC Address and store it in a char*
    MACAddress = Andee.getMACAddress();  

    //Display MAC Address in a display box
    displaybox.setData(MACAddress);
    displaybox.update();
  }
  
  delay(500); // Always leave a short delay for Bluetooth communication
}



