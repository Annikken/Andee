// Always include these libraries. Annikken Andee needs them
// to work with the Arduino!
#include <SPI.h>
#include <Andee.h>

// Every object that appears on your smartphone's screen
// needs to be declared like this:
AndeeHelper gyro;
AndeeHelper xDisp;
AndeeHelper yDisp;
AndeeHelper zDisp;

//Initialising 3 variables to store the values from the smartdevice
float x;
float y;
float z;

//Initialising 2 more variables to check if the UI has been updated yet
int setgyro = 0;

void setup() {
  // put your setup code here, to run once:
  Andee.begin();// Setup communication between Annikken Andee and Arduino
  Andee.setName("Gyro Demo");
  Andee.clear();// Clear the screen of any previous displays
  setInitialData();// Define object types and their appearance
}

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
  
  gyro.setId(3);
  gyro.setType(BUTTON_IN);
  gyro.setLocation(1, 1, FULL);
  gyro.setTitle("gyro Sensor On");
  gyro.setColor(GREEN);
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(gyro.isPressed())
  {
    gyro.ack();
    if(setgyro == 0)
    {
      Andee.gyroInit(400, -1);//Initialise the gyro sensor output. 
      //400 is the interval between sensor readings and -1 is the amount of times. -1 means infinite
      setgyro = 1;
    }
    else
    {
      Andee.gyroStop();//Stop the phone from sending gyro readings
      setgyro = 0;
    }     
  } 
  
  Andee.getGyroReading(&x, &y, &z); //this function saves the sensor reading into the variables x,y and z
  gyro.update(2);//Each UI object has to be updated to appear in the smartdevice
  xDisp.setData(x,4);
  yDisp.setData(y,4);
  zDisp.setData(z,4);
  
  xDisp.update();
  yDisp.update();
  zDisp.update();
  //This will help from sending the Andee updates every loop to not flood the Andee with information
  delay(500);
}
