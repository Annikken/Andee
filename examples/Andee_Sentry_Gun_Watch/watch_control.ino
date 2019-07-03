void watch_control()
{
  if(watch_button1.isPressed())//up
  {
    watchY = watchY + INCREMENT;
  }
  
  if(watch_button2.isPressed())//down
  {
    watchY = watchY - INCREMENT;
  }

  if(watch_button3.isPressed())//left
  {
    watchX = watchX + INCREMENT;
  }

  if(watch_button4.isPressed())//right
  {
    watchX = watchX - INCREMENT;
  }

  if(watch_button5.isPressed())//trigger
  {
    digitalWrite(PIN_LASER,HIGH);//toggle laser pin
    delay(50);
    digitalWrite(PIN_FIRE,HIGH);
    delay(25);
    digitalWrite(PIN_FIRE,LOW);
    delay(50);
    digitalWrite(PIN_LASER,LOW);//toggle laser pin
  }

  if(watchX < 0)
  {
    watchX == 0;
  }
  if(watchX > 180)
  {
    watchX == 180;
  }
  if(watchY < 0)
  {
    watchY == 0;
  }
  if(watchY > 180)
  {
    watchY == 180;
  }

  XAxis.write(watchX);
  YAxis.write(watchY);
}

