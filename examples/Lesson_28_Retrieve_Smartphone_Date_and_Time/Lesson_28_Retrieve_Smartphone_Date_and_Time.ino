/************************************************************
  [Annikken Andee]
  ================
  Lesson 28
  Retrieve Smartphone Date and Time
  by Jonathan Sim <jonathan@annikken.com>

  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  http://resources.annikken.com
  
  Check out our Blog for regular updates and fun!
  http://annikken.com/blog
************************************************************/

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <SPI.h>
#include <Andee.h>

// Dis[;ay the date and time from your smartphone on the Annikken Andee
// We'll create an object to demonstrate this.
AndeeHelper dateTime;

char dateTimeBuffer[30];
int day;
int month;
int year;
int hour; 
int minute;
int second;

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  dateTime.setId(0);
  dateTime.setType(DATA_OUT);
  dateTime.setLocation(0,0,FULL);
  dateTime.setTitle("Date & Time");  
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{  
  // Useful if you need to check the current time to execute a scheduled task
  // Retrieve date and store in variables: day, month, and year
  Andee.getDeviceDate(&day, &month, &year);
  // Retrieve time and store in variables: hour, minute, second
  Andee.getDeviceTime(&hour, &minute, &second);
  sprintf(dateTimeBuffer, "%d/%d/%d, %02d:%02d:%02d", day, month, year, hour, minute, second);
  dateTime.setData(dateTimeBuffer);
  dateTime.update();
  
  delay(500); // Always leave a short delay for Bluetooth communication
}



