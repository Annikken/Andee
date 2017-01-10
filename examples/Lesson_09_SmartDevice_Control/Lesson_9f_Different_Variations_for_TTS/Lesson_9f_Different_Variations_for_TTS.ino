/************************************************************
  [Annikken Andee]
  ================
  Lesson 9f
  Different variations for TTS
  
  Check out our Resources section for more information and 
  ideas on what you can do with the Annikken Andee!
  http://resources.annikken.com

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
#include <Andee.h>
#include <SPI.h>

AndeeHelper textToSpeech;

void setup(){
  Andee.begin();
  Andee.clear();

  textToSpeech.setId(0);
  textToSpeech.setType(TTS);
  textToSpeech.setData("Hello...");
}

void loop(){
  textToSpeech.setPitch(1.0); //1.0 = NORMAL
  textToSpeech.setUtteranceSpeed(0.4); //0.4 = NORMAL
  textToSpeech.setAccent(GREAT_BRITON);//in iOS you can have accents
  textToSpeech.updateData("A text to speech demonstration using Annikken Andee.");
  delay(4000);
  
  textToSpeech.setData("Hello! I am from the states!");
  textToSpeech.setAccent(US);
  textToSpeech.update(); //tell the device to speak
  delay(3000);
  
  textToSpeech.setData("Hello! I am from Great Briton!");
  textToSpeech.setAccent(GREAT_BRITON);
  textToSpeech.update();
  delay(3000);
  
  textToSpeech.setData("Hello! I am from Australia!");
  textToSpeech.setAccent(AUSTRALIA);
  textToSpeech.update();
  delay(3000);
  
  textToSpeech.setData("Hello! I am from Ireland!");
  textToSpeech.setAccent(IRELAND);
  textToSpeech.update();
  delay(3000);
  
  textToSpeech.setData("Hello! I am from South Africa!");
  textToSpeech.setAccent(SOUTH_AFRICA);
  textToSpeech.update();
  delay(3000);
  
  textToSpeech.setUtteranceSpeed(0.0);
  textToSpeech.setData("Hello! I am speaking really slow.");
  textToSpeech.setAccent(GREAT_BRITON);
  textToSpeech.update();
  delay(3000);
  
  textToSpeech.setUtteranceSpeed(1.5);
  textToSpeech.setData("Hello! I am speaking really fast!");
  textToSpeech.setAccent(AUSTRALIA);
  textToSpeech.update();
  delay(3000);
  
  textToSpeech.setAccent(US);
  textToSpeech.setUtteranceSpeed(0.4);
  textToSpeech.setPitch(2.0);
  textToSpeech.updateData("This is a high pitch voice!");
  
  delay(3000);
  
  textToSpeech.setAccent(GREAT_BRITON);
  textToSpeech.setPitch(0.5);
  textToSpeech.updateData("This is a low pitch voice!");
  
  delay(3000);
  
}
