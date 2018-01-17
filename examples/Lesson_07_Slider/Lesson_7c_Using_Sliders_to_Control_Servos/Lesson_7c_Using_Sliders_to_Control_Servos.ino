/************************************************************
  [Annikken Andee]
  ================
  Lesson 7c
  Using Sliders to Control Servos
  
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
#include <Servo.h> 

// We'll create 3 buttons. One to make it slowly turn left, the other
// to make it slowly turn right. And the third one to quickly turn to
// a user-specified position. We'll also create one display box to tell
// us about the servo's current position
AndeeHelper displaybox;
AndeeHelper sliderPosition;

Servo theServo; // Declare a servo
int currentPosition; // Stores current position of servo (from 0 to 180)
const int servoPin = 2; // Servo connected to pin 2

// The setup() function is meant to tell Arduino what to do 
// only when it starts up.
void setup()
{
  Andee.begin();  // Setup communication between Annikken Andee and Arduino
  Andee.setName("Servo Demo");//Set the name of the Andee. This name will appear in the app
  Andee.clear();  // Clear the screen of any previous displays
  currentPosition = 0; // Initialise position to 0
  theServo.attach(servoPin); // Tell Arduino which pin the servo is connected to
  theServo.write(currentPosition); // Set servo to position 0
  setInitialData(); // Define object types and their appearance
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  displaybox.setId(0);
  displaybox.setType(DATA_OUT); // Sets object as a text input button
  displaybox.setLocation(0,0,FULL);
  displaybox.setTitle("Current Position");
  displaybox.setData(0);
  
  // Draw position slider
  sliderPosition.setId(1);  
  sliderPosition.setType(SLIDER_IN); // Set object as a slider
  sliderPosition.setLocation(1, 0, FULL); // Sliders can only be full size
  sliderPosition.setTitle("Control Servo Position");
  sliderPosition.setSliderMinMax(0, 180);
  sliderPosition.setSliderInitialValue(currentPosition);  
  sliderPosition.setSliderNumIntervals(181); // Set as discrete slider
  sliderPosition.setSliderReportMode(ON_FINGER_UP); // Update values as you're moving 
}

// Arduino will run instructions here repeatedly until you power it off.
void loop()
{
  if(sliderPosition.getSliderValue(&currentPosition,INT))// Use INT to retrieve integer value
  {
    theServo.write(currentPosition);
    displaybox.setData(currentPosition);
  }

  displaybox.update();
  sliderPosition.update();
  
  delay(500); // Always leave a short delay for Bluetooth communication
  // Do not use a delay less than 100. Arduino's built-in servo library 
  // isn't well-designed and will cause the Arduino to crash.
}



