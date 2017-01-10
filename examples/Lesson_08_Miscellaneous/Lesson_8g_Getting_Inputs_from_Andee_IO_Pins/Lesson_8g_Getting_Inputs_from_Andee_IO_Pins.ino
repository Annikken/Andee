/************************************************************
  [Annikken Andee]
  ================
  Lesson 8g
  Getting inputs from Andee IO Pins!
  
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

// Every object that appears on your smartphone's screen
// needs to be declared like this:
AndeeHelper disp0;
AndeeHelper disp1;
AndeeHelper disp2;
AndeeHelper disp3;
AndeeHelper disp4;
AndeeHelper disp5;
AndeeHelper disp6;
AndeeHelper disp7;
//Here we are creating 8 UI boxes to display whether 
//the IO is at a HIGH or LOW state


int value0 = 0;
int value1 = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;
int value5 = 0;
int value6 = 0;
int value7 = 0;
//8 variables are made to store the state of each IO pin

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup() 
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays  
  setInitialData(); // Define object types and their appearance

  for(int i = 0; i<8;i++)
  {
    Andee.AIO_pinMode(i,INPUT);
  }// Set pins 0 to 7 to INPUT
}

void setInitialData()
{
////Drawing 8 UI objects//////
  disp0.setId(0);// Each object must have a unique ID number
  disp0.setType(DATA_OUT);// This defines your object as a display box
  disp0.setLocation(0,0,ONE_QUART);// Sets the location and size of your object
  disp0.setData("");//This is where you want the result to show so leave it empty for now
  disp0.setTitle("Pin 0");//This is the title of the box to identify it

  disp1.setId(1);
  disp1.setType(DATA_OUT);
  disp1.setLocation(0,1,ONE_QUART);
  disp1.setData("");
  disp1.setTitle("Pin 1");
  
  disp2.setId(2);  
  disp2.setType(DATA_OUT);
  disp2.setLocation(0,2,ONE_QUART);
  disp2.setData("");
  disp2.setTitle("Pin 2");
  
  disp3.setId(3);  
  disp3.setType(DATA_OUT);
  disp3.setLocation(0,3,ONE_QUART);
  disp3.setData("");
  disp3.setTitle("Pin 3");
  
  disp4.setId(4);  
  disp4.setType(DATA_OUT);
  disp4.setLocation(1,0,ONE_QUART);
  disp4.setData("");
  disp4.setTitle("Pin 4");
  
  disp5.setId(5);  
  disp5.setType(DATA_OUT);
  disp5.setLocation(1,1,ONE_QUART);
  disp5.setData("");
  disp5.setTitle("Pin 5");
  
  disp6.setId(6);  
  disp6.setType(DATA_OUT);
  disp6.setLocation(1,2,ONE_QUART);
  disp6.setData("");
  disp6.setTitle("Pin 6");

  disp7.setId(7);
  disp7.setType(DATA_OUT);
  disp7.setLocation(1,3,ONE_QUART);
  disp7.setData("");
  disp7.setTitle("Pin 7");
}

// Arduino will run instructions here repeatedly until you power it off.
void loop() {
  
  value0 = Andee.AIO_digitalRead(0);
  value1 = Andee.AIO_digitalRead(1);
  value2 = Andee.AIO_digitalRead(2);
  value3 = Andee.AIO_digitalRead(3);
  value4 = Andee.AIO_digitalRead(4);
  value5 = Andee.AIO_digitalRead(5);
  value6 = Andee.AIO_digitalRead(6);
  value7 = Andee.AIO_digitalRead(7);  
//each IO is read and stored to its respective variable
  
  disp0.setData(value0);
  disp1.setData(value1);
  disp2.setData(value2);
  disp3.setData(value3);
  disp4.setData(value4);
  disp5.setData(value5);
  disp6.setData(value6);
  disp7.setData(value7);
//Once the value is stored, setData is set to display the value on the UI
  
  disp0.update();
  disp1.update();
  disp2.update();
  disp3.update();
  disp4.update();
  disp5.update();
  disp6.update();
  disp7.update();
// Call updates to all UI objects or else the object will not appear on the smart device
  
  delay(300);
// A short delay is necessary to give Andee time to communicate with the smart device
  
}
