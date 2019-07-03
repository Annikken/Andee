#include <Servo.h>
#include <SPI.h>
#include <Andee.h>

//values are defined here for easy changing later on
#define PIN_LASER 9
#define PIN_FIRE 4
#define PIN_HSERVO A1
#define PIN_VSERVO A0
#define DEFAULT_X 90
#define DEFAULT_Y 15

#define ANDEE_NAME "Andee SentryGun"//change name of Andee here

//initialise all 3 UI elements below
AndeeHelper joystick;
AndeeHelper fireButton;
AndeeHelper laserButton;

//initialise the 2 servos here
Servo XAxis;
Servo YAxis;

//initialising values used
int xValue,yValue;
bool laser = false;

void setup() {
  // put your setup code here, to run once:
  Andee.begin();
  Andee.clear();
  setInitialData(); // Define object types and their appearance
  Andee.setName(ANDEE_NAME);
  
  pinMode(PIN_FIRE,OUTPUT);//set fire and laser pins to output
  pinMode(PIN_LASER,OUTPUT);
  
  XAxis.attach(PIN_HSERVO);//set servo pins
  YAxis.attach(PIN_VSERVO);
  
  XAxis.write(DEFAULT_X);//initialise servo positions
  YAxis.write(DEFAULT_Y);
}

void loop() {
  // put your main code here, to run repeatedly:
  joystick.getJoystick(&xValue,&yValue);//function to get joystick value
  XAxis.write(xValue + DEFAULT_X);//change HSERVO position based on X joystick value
  YAxis.write(yValue + DEFAULT_Y);//change VSERVO position based on Y joystick value
   
  if(fireButton.isPressed())
  {
    //leaving turret PIN on HIGH the whole time will shoot all bullets
    //so toggling HIGH and LOW will shoot 1 bullet at a time
    digitalWrite(PIN_FIRE,HIGH);
    delay(25);
    digitalWrite(PIN_FIRE,LOW);
  }
  if(laserButton.isPressed())
  {
    laser = !laser;
    digitalWrite(PIN_LASER,laser);//toggle laser pin    
  }
  delay(50);
  
  joystick.update(2);//number in update dictates how many loops to wait before refreshing UI
  fireButton.update(3);
  laserButton.update(4);
}

void setInitialData()//set your UI customisation here
{
  joystick.setId(0);
  joystick.setType(JOYSTICK);
  joystick.setCoord(5,0,50,50);
  joystick.setTitle("Press Me");
  joystick.setActiveColor(PURPLE);
  joystick.setBaseColor(YELLOW);
  joystick.setXAxisMax(90);
  joystick.setYAxisMax(90);

  fireButton.setId(1);
  fireButton.setType(BUTTON_IN);
  fireButton.setLocation(0, 3, ONE_QUART); // Row 0, Right
  fireButton.setTitle("FIRE!!!");
  fireButton.setColor(RED);
  fireButton.requireAck(false);

  laserButton.setId(2);
  laserButton.setType(BUTTON_IN);
  laserButton.setLocation(1, 3, ONE_QUART); // Row 1, Right
  laserButton.setTitle("AIM");
  laserButton.setColor(BLUE);
  laserButton.requireAck(false);
}

