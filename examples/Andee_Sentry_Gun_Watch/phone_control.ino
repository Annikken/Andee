void phone_control()
{
  joystick.getJoystick(&xValue,&yValue);//function to get joystick value
  XAxis.write(xValue + DEFAULT_X);//change HSERVO position based on X joystick value
  YAxis.write(yValue + DEFAULT_Y);//change VSERVO position based on Y joystick value
  
  Serial.print("yValue:");Serial.println(yValue);
  Serial.print("xValue:");Serial.println(xValue);
  
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
  joystick.update(3);//number in update dictates how many loops to wait before refreshing UI
  fireButton.update(4);
  laserButton.update(4);
}

