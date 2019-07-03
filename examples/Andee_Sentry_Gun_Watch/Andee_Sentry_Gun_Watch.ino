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
#define PHONE_CONTROL 0
#define WATCH_CONTROL 1
#define INCREMENT 10

#define ANDEE_NAME "Andee SentryGun"//change name of Andee here

//initialise all 3 UI elements below
AndeeHelper joystick;
AndeeHelper fireButton;
AndeeHelper laserButton;
AndeeHelper changeState;

AndeeHelper watch_title;
AndeeHelper watch_button1;//up
AndeeHelper watch_button2;//down
AndeeHelper watch_button3;//left
AndeeHelper watch_button4;//right
AndeeHelper watch_button5;//trigger

//initialise the 2 servos here
Servo XAxis;
Servo YAxis;

//initialising values used
int xValue,yValue;
bool laser = false;
int watchX,watchY;

//initialising the state
int sentry_state = 0;

void setup() {
  // put your setup code here, to run once:
  Andee.begin();
  Andee.clear();
  setInitialData(); // Define object types and their appearance
  Andee.setName(ANDEE_NAME);
  watchX = DEFAULT_X;
  watchY = DEFAULT_Y;
  
  pinMode(PIN_FIRE,OUTPUT);//set fire and laser pins to output
  pinMode(PIN_LASER,OUTPUT);
  
  XAxis.attach(PIN_HSERVO);//set servo pins
  YAxis.attach(PIN_VSERVO);
  
  XAxis.write(DEFAULT_X);//initialise servo positions
  YAxis.write(DEFAULT_Y);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(sentry_state == 0)
  {
    phone_control();
  }
  else
  {
    watch_control();
  } 
  
  if(changeState.isPressed())
  {
    XAxis.write(DEFAULT_X);//reset servo positions
    YAxis.write(DEFAULT_Y);
    
    if (sentry_state == 0)
    {
      watchX = DEFAULT_X;
      watchY = DEFAULT_Y;
      sentry_state = 1;
      Andee.clear();           
      Andee.changeScreen(WATCH_GAMEPAD);
      Andee.showScreen();  
      changeState.setTitle("Phone Mode");
      changeState.setColor(DARK_VIOLET);      
    }
    else
    {      
      sentry_state = 0;
      Andee.hideScreen();
      changeState.setTitle("Watch Mode");
      changeState.setColor(ORANGE);

      initialiseJoystick();
    }
  }

  changeState.update(2);  
  delay(100);
}

void setInitialData()//set your UI customisation here
{
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

  changeState.setId(3);
  changeState.setType(BUTTON_IN);
  changeState.setLocation(3, 0, FULL); // Row 1, Right
  changeState.setTitle("Watch Mode");
  changeState.setColor(ORANGE);
  changeState.requireAck(false);

  watch_button1.setId(WATCH_TITLE); 
  watch_button1.setType(WATCH);

  watch_button1.setId(WATCH_BUTTON1); 
  watch_button1.setType(WATCH);

  watch_button2.setId(WATCH_BUTTON2); 
  watch_button2.setType(WATCH);

  watch_button3.setId(WATCH_BUTTON3); 
  watch_button3.setType(WATCH);

  watch_button4.setId(WATCH_BUTTON4); 
  watch_button4.setType(WATCH);

  watch_button5.setId(WATCH_BUTTON5); 
  watch_button5.setType(WATCH);
}

void initialiseJoystick()
{
  joystick.setId(4);
  joystick.setType(JOYSTICK);
  joystick.setCoord(5,0,50,50);
  joystick.setTitle("Press Me");
  joystick.setActiveColor(PURPLE);
  joystick.setBaseColor(YELLOW);
  joystick.setXAxisMax(90);
  joystick.setYAxisMax(90);
}

