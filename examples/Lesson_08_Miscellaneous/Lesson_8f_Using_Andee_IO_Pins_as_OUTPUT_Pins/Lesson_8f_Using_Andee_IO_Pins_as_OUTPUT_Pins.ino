/************************************************************
  [Annikken Andee]
  ================
  Lesson 8f
  Using Andee IO pins as OUTPUT pins!
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  https://annikken.gitbooks.io/annikken-andee/content/

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
// AIO stands for Andee Input/Output. The 8 AIO pins located on the Annikken Andee for Android AIO Ready board.
// You can use these AIO pins as a digital out or digital in pins.
// In this lesson, We will create 8 Buttons that control the 8 AIO pins on the Annikken Andee Board.

// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <SPI.h>
#include <Andee.h>

// Every object that appears on your smartphone's screen
// needs to be declared like this:
AndeeHelper pin0Button;
AndeeHelper pin1Button;
AndeeHelper pin2Button;
AndeeHelper pin3Button;
AndeeHelper pin4Button;
AndeeHelper pin5Button;
AndeeHelper pin6Button;
AndeeHelper pin7Button; 

int AIOpinState[8];

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.clear();  // Clear the screen of any previous displays
  setInitialData(); // Define object types and their appearance
  for (int i = 0; i < 8; i++)
  {
    AIOpinState[i] = 0;
    Andee.AIO_pinMode(i, OUTPUT);
  }//Set pins 0 to 7 to OUTPUT and set state to 0
}

// This is the function meant to define the types and the appearance of
// all the objects on your smartphone
void setInitialData()
{
  //// Let's draw the first object! //////////////////////////////////////////

  pin0Button.setId(0);  // Each object must have a unique ID number
  pin0Button.setType(BUTTON_IN);  // This defines your object as a button input box
  pin0Button.setLocation(0, 0, HALF); // Sets the location and size of your object
  pin0Button.setTitle("Pin 0");//Sets the text on this button
  pin0Button.setColor("FFFF0000");
  pin0Button.requireAck(false);

  pin1Button.setId(1);
  pin1Button.setType(BUTTON_IN);
  pin1Button.setLocation(0, 1, HALF);
  pin1Button.setTitle("Pin 1");
  pin1Button.setColor("FFFF0000");
  pin1Button.requireAck(false);

  pin2Button.setId(2);
  pin2Button.setType(BUTTON_IN);
  pin2Button.setLocation(1, 0, HALF);
  pin2Button.setTitle("Pin 2");
  pin2Button.setColor("FFFF0000");
  pin2Button.requireAck(false);

  pin3Button.setId(3);
  pin3Button.setType(BUTTON_IN);
  pin3Button.setLocation(1, 1, HALF);
  pin3Button.setTitle("Pin 3");
  pin3Button.setColor("FFFF0000");
  pin3Button.requireAck(false);

  pin4Button.setId(4);
  pin4Button.setType(BUTTON_IN);
  pin4Button.setLocation(2, 0, HALF);
  pin4Button.setTitle("Pin 4");
  pin4Button.setColor("FFFF0000");
  pin4Button.requireAck(false);

  pin5Button.setId(5);
  pin5Button.setType(BUTTON_IN);
  pin5Button.setLocation(2, 1, HALF);
  pin5Button.setTitle("Pin 5");
  pin5Button.setColor("FFFF0000");
  pin5Button.requireAck(false);

  pin6Button.setId(6);
  pin6Button.setType(BUTTON_IN);
  pin6Button.setLocation(3, 0, HALF);
  pin6Button.setTitle("Pin 6");
  pin6Button.setColor("FFFF0000");
  pin6Button.requireAck(false);

  pin7Button.setId(7);
  pin7Button.setType(BUTTON_IN);
  pin7Button.setLocation(3, 1, HALF);
  pin7Button.setTitle("Pin 7");
  pin7Button.setColor("FFFF0000");
  pin7Button.requireAck(false);
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{  
  if (pin0Button.isPressed()) //Check Button 0
  {
    if (AIOpinState[0] == 0)// If the state of the AIO pin 0 is LOW
    {
      AIOpinState[0] = 1;//Change the state of AIO pin 0 to HIGH
      Andee.AIO_digitalWrite(0, HIGH); //OUTPUT HIGH on AIO Pin 0
      pin0Button.setColor("FF00FF00");
    }
    else if (AIOpinState[0] == 1)//If the state of the AIO pin 0 is HIGH
    {
      AIOpinState[0] = 0;//Change the state of AIO pin 0 to LOW
      Andee.AIO_digitalWrite(0, LOW); //OUTPUT HIGH on AIO Pin 0
      pin0Button.setColor("FFFF0000");
    }
  }
  pin0Button.update();
  
  if (pin1Button.isPressed()) //Check Button 1
  {
    if (AIOpinState[1] == 0)
    {
      AIOpinState[1] = 1;
      Andee.AIO_digitalWrite(1, HIGH);
      pin1Button.setColor("FF00FF00");
    }
    else if (AIOpinState[1] == 1)
    {
      AIOpinState[1] = 0;
      Andee.AIO_digitalWrite(1, LOW);
      pin1Button.setColor("FFFF0000");
    }
  }
  pin1Button.update();
  
  if (pin2Button.isPressed()) //Check Button 2
  {
    if (AIOpinState[2] == 0)
    {
      AIOpinState[2] = 1;
      Andee.AIO_digitalWrite(2, HIGH);
      pin2Button.setColor("FF00FF00");
    }
    else if (AIOpinState[2] == 1)
    {
      AIOpinState[2] = 0;
      Andee.AIO_digitalWrite(2, LOW);
      pin2Button.setColor("FFFF0000");
    }
  }
  pin2Button.update();
  
  if (pin3Button.isPressed()) //Check Button 3
  {
    if (AIOpinState[3] == 0)
    {
      AIOpinState[3] = 1;
      Andee.AIO_digitalWrite(3, HIGH);
      pin3Button.setColor("FF00FF00");
    }
    else if (AIOpinState[3] == 1)
    {
      AIOpinState[3] = 0;
      Andee.AIO_digitalWrite(3, LOW);
      pin3Button.setColor("FFFF0000");
    }
  }
  pin3Button.update();
  
  if (pin4Button.isPressed()) //Check Button 4
  {
    if (AIOpinState[4] == 0)
    {
      AIOpinState[4] = 1;
      Andee.AIO_digitalWrite(4, HIGH);
      pin4Button.setColor("FF00FF00");
    }
    else if (AIOpinState[4] == 1) 
    {
      AIOpinState[4] = 0;
      Andee.AIO_digitalWrite(4, LOW);
      pin4Button.setColor("FFFF0000");
    }
  }
  pin4Button.update();
  
  if (pin5Button.isPressed()) //Check Button 5
  {
    if (AIOpinState[5] == 0)
    {
      AIOpinState[5] = 1;
      Andee.AIO_digitalWrite(5, HIGH);
      pin5Button.setColor("FF00FF00");
    }
    else if (AIOpinState[5] == 1)
    {
      AIOpinState[5] = 0;
      Andee.AIO_digitalWrite(5, LOW);
      pin5Button.setColor("FFFF0000");
    }
  }
  pin5Button.update();
  
  if (pin6Button.isPressed())  //Check Button 6
  {
    if (AIOpinState[6] == 0)
    {
      AIOpinState[6] = 1;
      Andee.AIO_digitalWrite(6, HIGH);
      pin6Button.setColor("FF00FF00");
    }
    else if (AIOpinState[6] == 1) 
    {
      AIOpinState[6] = 0;
      Andee.AIO_digitalWrite(6, LOW);
      pin6Button.setColor("FFFF0000");
    }
  }
  pin6Button.update();
  
  if (pin7Button.isPressed())  //Check Button 7
  {
    if (AIOpinState[7] == 0)
    {
      AIOpinState[7] = 1;
      Andee.AIO_digitalWrite(7, HIGH);
      pin7Button.setColor("FF00FF00");
    }
    else if (AIOpinState[7] == 1)//If the state of the AIO pin 0 is HIGH
    {
      AIOpinState[7] = 0;
      Andee.AIO_digitalWrite(7, LOW);
      pin7Button.setColor("FFFF0000");
    }
  }
  pin7Button.update(); 
}



