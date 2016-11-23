/************************************************************
  [Annikken Andee]
  ================
  Lesson 37
  Displaying Gyroscope, Linear Acceleration, Gravity and GPS Values from the Smartdevice!
  by Hasif <hasif@annikken.com>

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

// Every object that appears on your smartphone's screen
// needs to be declared like this:
AndeeHelper grav;
AndeeHelper gyro;
AndeeHelper lac;
AndeeHelper gps;
AndeeHelper xDisp;
AndeeHelper yDisp;
AndeeHelper zDisp;
// We're creating 7 objects

//We are initialising 4 variables to help keep track of states
//This will help to create toggle buttons
int gravState;
int gyroState;
int lacState;
int gpsState;

//Initialising 3 variables to store the values from the smartdevice
float x;
float y;
float z;

//Initialising 2 more variables to check if the UI has been updated yet
int refreshUI = 0;
int On;

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup() {
  Andee.begin();// Setup communication between Annikken Andee and Arduino
  Andee.clear();// Clear the screen of any previous displays
  setInitialData();// Define object types and their appearance
  //Serial.begin(9600);// Start Serial communication to view data in the Serial Terminal

  //Setting these variables to 0
  gravState = 0;
  gyroState = 0;
  lacState = 0;
  gpsState = 0;

}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  xDisp.setId(0); // Each object must have a unique ID number
  xDisp.setType(DATA_OUT);// Defines object as a display box
  xDisp.setLocation(0, 0, ONE_THIRD); // Sets the location and size of your object
  xDisp.setTitle("X-Axis");// Sets the title of the object
  xDisp.setData("");//Leave this empty since this will be filled up with data later on
  xDisp.setColor(LIGHT_BLUE);//Sets the colour of the UI object

  yDisp.setId(1);
  yDisp.setType(DATA_OUT);
  yDisp.setLocation(0, 1, ONE_THIRD);
  yDisp.setTitle("Y-Axis");
  yDisp.setData("");
  yDisp.setColor(BLUE);

  zDisp.setId(2);
  zDisp.setType(DATA_OUT);
  zDisp.setLocation(0, 2, ONE_THIRD);
  zDisp.setTitle("Z-Axis");
  zDisp.setData("");
  zDisp.setColor(DARK_BLUE);

  grav.setId(3);
  grav.setType(BUTTON_IN);// Defines object as a button
  grav.setLocation(1, 0, ONE_QUART);
  grav.setTitle("Gravity Sensor On");
  grav.setColor(GREEN);

  gyro.setId(4);
  gyro.setType(BUTTON_IN);
  gyro.setLocation(1, 1, ONE_QUART);
  gyro.setTitle("Gyro Sensor On");
  gyro.setColor(GREEN);

  lac.setId(5);
  lac.setType(BUTTON_IN);
  lac.setLocation(1, 2, ONE_QUART);
  lac.setTitle("LAC Sensor On");
  lac.setColor(GREEN);

  gps.setId(6);
  gps.setType(BUTTON_IN);
  gps.setLocation(1, 3, ONE_QUART);
  gps.setTitle("GPS Sensor On");
  gps.setColor(GREEN);
}

// Arduino will run instructions here repeatedly until you power it off.
void loop() {

  On = Andee.isConnected();//This function check if the Annikken Andee is connected and stores it in the variable On
  
  if (refreshUI == 0 && On == 1)//If the UI has not been updated and the Andee is connected
  {
    gyro.update();//Each UI object has to be updated to appear in the smartdevice
    grav.update();
    lac.update();
    gps.update();
    xDisp.update();
    yDisp.update();
    zDisp.update();
    refreshUI = 1;//This shows that the UI has been updated
  }
  else if (refreshUI == 1 && On == 0) //If the UI has been updated but the Andee is not connected
  {
    Andee.clear();//Clear the screen of the smartdevice on the next reconnect
    refreshUI = 0;
  }

  ////////////////////////////This is the control for the gravity sensor///////////////////////////////////////////////
  if (grav.isPressed() ) //if the grav button is pressed
  {
    // Prevent user from accidentally pressing the button again
    // until Arduino has sent an acknowledgement
    grav.ack();

    if (gravState == 0) //If gravState is low when the button is pressed
    {
      grav.gravInit(250, -1); //This will activate the Gravity sensor on the smartdevice.
      //250 is the time interval of each reading in milliseconds. Lowest is 250ms.
      //-1 is the number of times readings will be taken by the sensor. -1 means infinite
      delay(10);//A small delay is here to allow some time for the sensor to finish initialisation
      gravState = 1;//change the gravState here
      gyroState = 0;//Switch off the readings from other sensors
      lacState = 0;
      gpsState = 0;
    }
    else if (gravState == 1) //If gravState is high when button is pressed
    {
      gravState = 0;//switch off gravity sensor
      grav.gravStop();//this will tell the smart device to stop reading from its gravity sensor
    }
  }

  if (gravState == 1)//whenever gravState is HIGH
  {
    grav.setTitle("Grav Sensor off");//change the title of the button
    grav.setColor(RED);//change the colour of the button
    grav.getGravReading(&x, &y, &z); //this function saves the sensor reading into the variables x,y and z
    
    //These functions send the data to the data part of the display box UI objects
    xDisp.setData(x);
    yDisp.setData(y);
    zDisp.setData(z);
    Serial.print("x:");Serial.println(x);
    Serial.print("y:");Serial.println(y);
    Serial.print("z:");Serial.println(z);

    //Update the UI objects to reflect the current values of x,y and z
    grav.update();
    xDisp.update();
    yDisp.update();
    zDisp.update();
    delay(50);
  }
  else// if gravState is LOW
  {
    grav.setTitle("Grav Sensor On");//change the title of the button
    grav.setColor(GREEN);//change the colour of the button
    grav.update();//update the grav UI
  }

  //////////////////////This is the control for the Gyroscope sensor//////////////////////////////////////////////////////
  if (gyro.isPressed() )
  {
    gyro.ack();

    if (gyroState == 0)
    {
      gyro.gyroInit(250, -1);
      delay(10);
      gyroState = 1;
      gravState = 0;
      lacState = 0;
      gpsState = 0;
    }
    else
    {
      gyroState = 0;
      gyro.gyroStop();
    }
  }

  if (gyroState == 1)
  {
    gyro.setTitle("Gyro Sensor off");
    gyro.setColor(RED);
    gyro.getGyroReading(&x, &y, &z);    
    xDisp.setData(x);
    yDisp.setData(y);
    zDisp.setData(z);
    xDisp.update();
    yDisp.update();
    zDisp.update();
    gyro.update();
    delay(10);
  }
  else
  {
    gyro.setTitle("Gyro Sensor On");
    gyro.setColor(GREEN);
    gyro.update();
  }

  //////////////////////////This is the control for the LAC(Linear Acceleration) sensor/////////////////////////////////////////
  if (lac.isPressed() )
  {
    lac.ack();

    if (lacState == 0)
    {
      lac.lacInit(250, -1);
      delay(10);
      lacState = 1;
      gravState = 0;
      gyroState = 0;
      gravState = 0;
    }
    else
    {
      lacState = 0;
      lac.lacStop();
    }
  }

  if (lacState == 1)
  {
    lac.setTitle("LAC Sensor off");
    lac.setColor(RED);
    lac.getLacReading(&x, &y, &z);    
    xDisp.setData(x);
    yDisp.setData(y);
    zDisp.setData(z);
    xDisp.update();
    yDisp.update();
    zDisp.update();
    lac.update();
    delay(10);
  }
  else
  {
    lac.setTitle("LAC Sensor On");
    lac.setColor(GREEN);
    lac.update();
  }

  /////////////////////////////This is the control for the GPS sensor///////////////////////////////////////////////
  if (gps.isPressed() )
  {
    gps.ack();

    if (gpsState == 0)
    {
      gps.gpsInit(5000, -1);//For GPS the minimum time interval is 5000ms. The smartphone/tablet needs some time
      delay(10);            //to achieve a lock so this timing should be enough time to get the information      
      gpsState = 1;
      lacState = 0;
      gyroState = 0;
      gravState = 0;
    }
    else
    {
      gpsState = 0;
      gps.gpsStop();
    }
  }

  if (gpsState == 1)
  {
    gps.setTitle("Gps Sensor off");
    gps.setColor(RED);
    gps.getGpsReading(&x,&y);    
    xDisp.setData(x,6);
    yDisp.setData(y,6);
    zDisp.setData(0);
    xDisp.update();
    yDisp.update();
    zDisp.update();
    gps.update();
    delay(10);
  }
  else
  {
    gps.setTitle("Gps Sensor On");
    gps.setColor(GREEN);
    gps.update();
  }  
}



