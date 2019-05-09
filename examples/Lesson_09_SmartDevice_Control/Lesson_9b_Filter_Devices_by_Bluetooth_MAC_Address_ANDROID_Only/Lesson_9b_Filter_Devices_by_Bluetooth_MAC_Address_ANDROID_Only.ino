/************************************************************
  [Annikken Andee]
  ================
  Lesson 9b
  Filter Devices by Bluetooth MAC Address

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

// We'll need a display box to display the connected MAC address
AndeeHelper displaybox;

// This list will contain all the authorised MAC addresses
char *authorisedList[] = { "00:11:22:33:44:55",
                           "aa:bb:cc:dd:ee:ff",
                           "aa:11:bb:22:cc:33"
                         };  // Don't forget to add yours!
int noOfAuthorisedUsers = 3;

const char* MACAddress;//Used to store the MAC address
bool authorised; // Flag to check if user is authorised or not


// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
  authorised = false;
}

// This is the function meant to define the types and the appearance of
// all the objects on your smartphone
void setInitialData()
{
  displaybox.setId(0);
  displaybox.setType(DATA_OUT);
  displaybox.setLocation(0, 0, FULL);
  displaybox.setTitle("Connected MAC Address");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if ( Andee.isConnected() ) // When user presses the send button on phone
  {
    // Get MAC Address and store it in a char*
    MACAddress = Andee.getMACAddress();

    for (int i = 0; i < noOfAuthorisedUsers; i++)
    {
      if ( strcmp(MACAddress, authorisedList[i]) == 0 )
      {
        authorised = true; // Mark flag as true if connected device is in authorised list
      }
    }

    if (authorised == true) // If user is authorised
    {
      // This is where you'll show the authorised user the controls that you want to show
      displaybox.setData("You are authorised!");
      displaybox.update();
    }
    else // If intruder
    {
      // Show a message before you disconnect the intruder
      displaybox.setData("Intruder! You will be disconnected!");
      displaybox.update();
      delay(2000); // Give intruder time to read the message
      Andee.disconnect(); // Disconnect intruder
    }

  }
  else // If not connected
  {
    authorised = false; // Reset flag to false when authorised users disconnect
  }

  delay(500); // Always leave a short delay for Bluetooth communication
}



