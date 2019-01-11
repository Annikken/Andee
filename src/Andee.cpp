/*

Andee Library for Annikken Andee U/iOS and Android

Andee.cpp

Author: Hasif

*/

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <SPI.h>
#include <Andee.h>

unsigned int SS_PIN = 8;

char txBuffer[TX_MAX];
char rxBuffer[RX_MAX];

char EMPTY[3] = {'N','A','\0'};
AndeeClass Andee;

///////////////////////ANDEECLASS///////////////////
/////////////////////Main Andee Functions//////////////////
//void AndeeClass::AndeeClass(){}


void AndeeClass::begin(){
	pinMode(SS_PIN,OUTPUT);
	digitalWrite(SS_PIN,HIGH);
	#if defined(__SAM3X8E__)
		SPI.begin(SS_PIN);
		SPI.setClockDivider(SS_PIN, 21);
	#else		
		SPI.begin();	
	#endif 	
	
	delay(800);
	
	memset(rxBuffer,0x00,RX_MAX);	
}

void AndeeClass::begin(int pin){
	SS_PIN = pin;
	begin();	
}


void AndeeClass::clear(){
	andeeCommand = CLEAR_UI;
	sendAndee(99,EMPTY);
}

void AndeeClass::setName(const char* name){
	andeeCommand = SET_ANDEE_NAME;
	char limit[33];
	if(strlen(name) > 32)
	{
		memcpy(limit,name,32);
		limit[32] = '\0';
		sendAndee(99,(char*)limit);
	}
	else
	{
		sendAndee(99,(char*)name);
	}
	delay(100);	
}

const char* AndeeClass::getMACAddress(){
	andeeCommand = GET_MAC_ADDRESS;
	sendAndee(99,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		if(rxBuffer[1] > 0x2F)
		{			
			Serial.print("MAC:");Serial.println(rxBuffer);
			return rxBuffer;
		}
		
	}
	else
	{
		return "No Mac Found";
	}
}

signed char AndeeClass::getRSSI(){
	andeeCommand = GET_BLE_RSSI;
	sendAndee(99,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		if(rxBuffer[0] == 'R')
		{			
			return rxBuffer[1];
		}
		else
		{
			return 69;
		}
	}
	else
	{
		return 70;
	}
}

void AndeeClass::disconnect(){
	andeeCommand = DISCONNECT;
	sendAndee(99,EMPTY);
}

int AndeeClass::isConnected(){
	andeeCommand = CHECK_CONNECTION;
	sendAndee(99,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		if(rxBuffer[0] == 'Y')
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}


/////////////////////DEVICE TIME,DATE,BATTERY//////////////////
void AndeeClass::getDeviceTime(int* h,int* m, int* s){
	andeeCommand = GET_DEVICE_TIME;
	sendAndee(99,EMPTY);
	
	char* tempH;
	char* tempM;
	char* tempS;
	
	if(pollRx(rxBuffer))
	{
		tempH = strtok(rxBuffer,",");
		tempM = strtok(NULL,",");
		tempS = strtok(NULL, "");
		
		*h = strtod (tempH, NULL);
		*m = strtod (tempM, NULL);
		*s = strtod (tempS, NULL);		
	}
}

void AndeeClass::getDeviceDate(int* d, int* m, int* y){
	andeeCommand = GET_DEVICE_DATE;
	sendAndee(99,EMPTY);
	
	char* tempD;
	char* tempM;
	char* tempY;
	
	if(pollRx(rxBuffer))
	{
		tempD = strtok(rxBuffer,",");
		tempM = strtok(NULL,",");
		tempY = strtok(NULL, "");
		
		*d = strtod (tempD, NULL);
		*m = strtod (tempM, NULL);
		*y = strtod (tempY, NULL);		
	}
}

long AndeeClass::getDeviceTimeStamp(){
	andeeCommand = GET_DEVICE_TIMESTAMP;
	sendAndee(99,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		return atol(rxBuffer);		
	}
	return -10000;
}
///////////////////////////PHONE SENSORS & PERIPHERAL/////////////////////////
void AndeeClass::vibrate(){
	andeeCommand = VIBRATE;
	sendAndee(99,EMPTY);
}

void AndeeClass::gyroInit(int interval, int iteration){
	andeeCommand = GYRO_INIT;	
	
	char temp[14];
	unsigned int len;	
	itoa(interval, temp, 10);
	len = strlen(temp);
    temp[len] = ',';
    itoa(iteration, temp+len+1, 10);
	
	sendAndee(99,temp);
}

void AndeeClass::gyroStop(){
	andeeCommand = GYRO_STOP;
	sendAndee(99,EMPTY);
}

void AndeeClass::getGyroReading(float* x,float* y,float* z){
	andeeCommand = GET_GYRO_READING;
	sendAndee(99,EMPTY);
	
	char* tempX;
	char* tempY;
	char* tempZ;	
	
	if(pollRx(rxBuffer))
	{
		tempX = strtok(rxBuffer,",");
		tempY = strtok(NULL,",");
		tempZ = strtok(NULL, "");
		
		*x = strtod (tempX, NULL);
		*y = strtod (tempY, NULL);
		*z = strtod (tempZ, NULL);
	}	
}

void AndeeClass::lacInit(int interval, int iteration){
	andeeCommand = LAC_INIT;
	
	char temp[14];
	unsigned int len;	
	itoa(interval, temp, 10);
	len = strlen(temp);
    temp[len] = ',';
    itoa(iteration, temp+len+1, 10);
	
	sendAndee(99,temp);
}

void AndeeClass::lacStop(){
	andeeCommand = LAC_STOP;
	sendAndee(99,EMPTY);
}
void AndeeClass::getLacReading(float* x,float* y,float* z){
	andeeCommand = GET_LAC_READING;
	sendAndee(99,EMPTY);
	
	char* tempX;
	char* tempY;
	char* tempZ;	
	
	if(pollRx(rxBuffer))
	{
		tempX = strtok(rxBuffer,",");
		tempY = strtok(NULL,",");
		tempZ = strtok(NULL, "");
		
		*x = strtod (tempX, NULL);
		*y = strtod (tempY, NULL);
		*z = strtod (tempZ, NULL);		
	}	
}

void AndeeClass::gravInit(int interval, int iteration){
	andeeCommand = GRAV_INIT;
	
	char temp[14];
	unsigned int len;	
	itoa(interval, temp, 10);
	len = strlen(temp);
    temp[len] = ',';
    itoa(iteration, temp+len+1, 10);
	
	sendAndee(99,temp);
}

void AndeeClass::gravStop(){
	andeeCommand = GRAV_STOP;
	sendAndee(99,EMPTY);
}

void AndeeClass::getGravReading(float* x,float* y,float* z){
	andeeCommand = GET_GRAV_READING;
	sendAndee(99,EMPTY);
	
	char* tempX;
	char* tempY;
	char* tempZ;	
	//char* pEnd;
	
	if(pollRx(rxBuffer))
	{
		tempX = strtok(rxBuffer,",");
		tempY = strtok(NULL,",");
		tempZ = strtok(NULL, "");
		
		*x = strtod (tempX, NULL);
		*y = strtod (tempY, NULL);
		*z = strtod (tempZ, NULL);
	}	
}

void AndeeClass::gpsInit(int interval, int iteration){
	andeeCommand = GPS_INIT;
	
	char temp[14];
	unsigned int len;	
	itoa(interval, temp, 10);
	len = strlen(temp);
    temp[len] = ',';
    itoa(iteration, temp+len+1, 10);
	
	sendAndee(99,temp);
}

void AndeeClass::gpsStop(){
	andeeCommand = GPS_STOP;
	sendAndee(99,EMPTY);
}

void AndeeClass::getGpsReading(float* x,float* y){
	andeeCommand = GET_GPS_READING;
	sendAndee(99,EMPTY);
	
	char* tempX;
	char* tempY;
	
	if(pollRx(rxBuffer))
	{
		tempX = strtok(rxBuffer,",");
		tempY = strtok(NULL,",");
		
		*x = strtod (tempX, NULL);
		*y = strtod (tempY, NULL);						
	}		
}

void AndeeClass::sendSMS(char* number,char* message){
	andeeCommand = SEND_SMS;
	
	char temp[90];
	char limit[65];
	
	if(strlen(message) >= 64)
	{
		memcpy(limit,message,64);	
		limit[64] = '\0';
		sprintf(temp,"%s,%s",number,limit);
	}
	else
	{
		sprintf(temp,"%s,%s",number,message);		
	}
	sendAndee(99,temp);
}

void AndeeClass::takePhoto(char cameraType, char autoFocus, char flash){
	andeeCommand = TAKE_PHOTO;
	
	char temp[4];
	temp[0] = cameraType;
	temp[1] = autoFocus;
	temp[2] = flash;
	temp[3] = 0x00;
	
	sendAndee(99,temp);
}

void AndeeClass::textToSpeech(char* speech, float speed, float pitch, char accent ){
	andeeCommand = TEXT_TO_SPEECH;	
	char temp[90];
	char limit[65];
	
	if(strlen(speech) >= 64)
	{
		memcpy(limit,speech,64);
		limit[64] = '\0';
	}
	else
	{
		memcpy(limit,speech,strlen(speech));
		limit[strlen(speech)] = '\0';
	}
	
	
	int speedInt = speed * 10;
	int pitchInt = pitch * 10;
	sprintf(temp,"%s,%i,%i,%c",limit,speedInt,pitchInt,accent);	
	
	sendAndee(99,temp);
}

void AndeeClass::notification(char* title,char* message, char* ticker){
	andeeCommand = NOTIFICATION;
	char temp[90];
	char titleLimit[31];
	char msgLimit[41];
	char tickerLimit[16];
	unsigned int titleLen;
	unsigned int msgLen;
	unsigned int tickerLen;
	
	titleLen = strlen(title);
	msgLen = strlen(message);
	tickerLen = strlen(ticker);

	if(titleLen >= 30)
	{
		memcpy(titleLimit,title,30);
		titleLimit[30] = '\0';
	}
	else
	{
		memcpy(titleLimit,title,titleLen);
		titleLimit[titleLen] = '\0';
	}
	
	if(msgLen >= 40)
	{
		memcpy(msgLimit,message,40);
		msgLimit[40] = '\0';
	}
	else
	{
		memcpy(msgLimit,message,msgLen);
		msgLimit[msgLen] = '\0';
	}
	
	if(tickerLen >= 15)
	{
		memcpy(tickerLimit,ticker,15);
		tickerLimit[15] = '\0';
	}
	else
	{
		memcpy(tickerLimit,ticker,tickerLen);
		tickerLimit[tickerLen] = '\0';
	}	
	
	memset(temp,0x00,90);
	sprintf(temp,"%s,%s,%s",titleLimit,msgLimit,tickerLimit);
	
	sendAndee(99,temp);
}
//////////////////////////////APPLE WATCH FUNCTIONS////////////////////////////////
void AndeeClass::changeScreen(char screen){
	andeeCommand = CHANGE_SCREEN;
	sendByteAndee(99,screen+48);
}

void AndeeClass::showScreen(){
	andeeCommand = SHOW_SCREEN;
	sendAndee(99,EMPTY);
}

void AndeeClass::hideScreen(){
	andeeCommand = HIDE_SCREEN;
	sendAndee(99,EMPTY);
}

void AndeeClass::textInput(){
	andeeCommand = TEXT_INPUT;
	sendAndee(99,EMPTY);
}
///////////////////////////////////////////AIO///////////////////////////////////////
void AndeeClass::AIO_pinMode(char pin,uint8_t mode)
{
	if(pin>=0 || pin<=7)
	{
		andeeCommand = AIO_PIN_MODE;
		char temp[5];
		memset(temp,0x00,5);
		sprintf(temp,"%c,%c",pin+48,mode+32);
		sendAndee(99,temp);
	}
}

void AndeeClass::AIO_digitalWrite(char pin,uint8_t mode)
{
	if(pin>=0 || pin<=7)
	{
		andeeCommand = AIO_DIGITAL_WRITE;
		char temp[5];
		memset(temp,0x00,5);
		sprintf(temp,"%c,%c",pin+48,mode+32);
		sendAndee(99,temp);
	}
}

int AndeeClass::AIO_digitalRead(char pin)
{
	int pinRead = 0;
	if(pin>=0 || pin<=7)
	{
		andeeCommand = AIO_DIGITAL_READ;
		sendByteAndee(99,pin+48);		
		
		if(pollRx(rxBuffer))
		{
			pinRead = (rxBuffer[0] - 48);
			if(pinRead >= 0 || pinRead <= 1)
			{
				return pinRead;
			}			
		}
	}
	else
	{
		return 10;
	}
	
}

/*****************************************************************************
****************************ANDEEHELPER CLASS*********************************
************************* Andee Commands for UI ******************************
******************************************************************************
*****************************************************************************/

void AndeeHelper::setId(char value){
	id = value;
	updateLoop = 0;
	andeeCommand = SET_ID;
	sendAndee(id,EMPTY);
}

void AndeeHelper::setType(char type){
	andeeCommand = SET_TYPE;
	sendByteAndee(id,type);
}

void AndeeHelper::setLocation(char row, char order, char span){
	andeeCommand = SET_COORD;//set location is changed to output like using setCoord
	
	char temp[6];
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = 0;
	unsigned int h = 20;	
	
	if(row == 0)
	{
		y = 4;
	}
	else if(row == 1)
	{
		y = 28;
	}
	else if(row == 2)
	{
		y = 52;
	}
	else if(row == 3)
	{
		y = 76;
	}
	
	switch(span){
		case ONE_THIRD:
			if(order == 0)
			{
				x = 4;
				w = 30;
			}
			else if(order == 1)
			{
				x = 35;
				w = 30;
			}
			else if(order == 2)
			{
				x = 66;
				w = 30;
			}
			break;
		case TWO_THIRD:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 35;
			}
			else if(order == 2)
			{
				x = 66;
			}
			w = 61;
			break;
		case FULL:
			x = 4;
			w = 92;
			break;
		case ONE_QUART:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 28;
			}
			else if(order == 2)
			{
				x = 52;
			}
			else if(order == 3)
			{
				x = 76;
			}
			w = 20;
			break;
		case THREE_QUART:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 28;
			}
			else if(order == 2)
			{
				x = 52;
			}
			w = 68;
			break;
		case HALF:
			if(order == 0)
			{
				x = 4;
			}
			else if(order == 1)
			{
				x = 52;
			}
			w =44;
			break;	
		default:
			//Serial.println("Andee UI Span ERROR");
			break;
	}
	
	temp[0] = x+32;
	temp[1] = y+32;
	temp[2] = w+32;
	temp[3] = h+32;
	temp[4] = 0x00;
	
	sendAndee(id,temp);
}

void AndeeHelper::setCoord(int x, int y, int w, int h){
	andeeCommand = SET_COORD;
	
	
	if(x < SCREEN_LOWER_LIMIT)
	{
		x = SCREEN_LOWER_LIMIT;
	}
	else if (x > SCREEN_UPPER_LIMIT)
	{
		x = SCREEN_UPPER_LIMIT;
	}
	
	if(y < SCREEN_LOWER_LIMIT)
	{
		y = SCREEN_LOWER_LIMIT;
	}
	else if (y > SCREEN_UPPER_LIMIT)
	{
		y = SCREEN_UPPER_LIMIT;
	}
	
	if(w < SCREEN_LOWER_LIMIT)
	{
		w = SCREEN_LOWER_LIMIT;
	}
	else if (w > SCREEN_UPPER_LIMIT)
	{
		w = SCREEN_UPPER_LIMIT;
	}
	
	if(h < SCREEN_LOWER_LIMIT)
	{
		h = SCREEN_LOWER_LIMIT;
	}
	else if (h > SCREEN_UPPER_LIMIT)
	{
		h = SCREEN_UPPER_LIMIT;
	}
	
	char temp[6];
	temp[0] = x+32;
	temp[1] = y+32;
	temp[2] = w+32;
	temp[3] = h+32;
	temp[4] = 0x00;
		
	sendAndee(id,temp);
}

void AndeeHelper::setInputMode(char type){
	andeeCommand = SET_INPUT_MODE;
	sendByteAndee(id,type);
}

void AndeeHelper::requireAck(bool input){
	if (input == true)
	{
		setInputMode(ACK);
	}
	else
	{
		setInputMode(NO_ACK);
	}
}

void AndeeHelper::setColor(const char* color){
	andeeCommand = SET_COLOR;
	char limit[9];
	memcpy(limit,color,8);
	limit[8] = '\0';
	sendAndee(id,limit);
}

/* void AndeeHelper::setColor(const char color){
	andeeCommand = SET_COLOR;
	sendAndee(id,color);
} */

void AndeeHelper::setTitleColor(const char* color){
	andeeCommand = SET_TITLE_COLOR;
	char limit[9];
	memcpy(limit,color,8);
	limit[8] = '\0';
	sendAndee(id,limit);
}

/* void AndeeHelper::setTitleColor(const char color){
	andeeCommand = SET_TITLE_COLOR;
	sendAndee(id,color);
} */

void AndeeHelper::setTitleTextColor(const char* color){
	andeeCommand = SET_TITLE_TEXT_COLOR;
	char limit[9];
	memcpy(limit,color,8);
	limit[8] = '\0';
	sendAndee(id,limit);
}

/* void AndeeHelper::setTitleTextColor(const char color){
	andeeCommand = SET_TITLE_TEXT_COLOR;
	sendAndee(id,color);
} */

void AndeeHelper::setTextColor(const char* color){
	andeeCommand = SET_TEXT_COLOR;
	char limit[9];
	memcpy(limit,color,8);
	limit[8] = '\0';
	sendAndee(id,limit);
}

/* void AndeeHelper::setTextColor(const char color){
	andeeCommand = SET_TEXT_COLOR;
	sendAndee(id,color);
} */

void AndeeHelper::setData(const char* data){
	andeeCommand = SET_DATA;
	char temp[65];
	unsigned int dataLen;
	
	dataLen = strlen(data);
	if(dataLen == 0)
	{
		sprintf(temp,"     ");
	}
	else if(dataLen > 64)
	{
		memcpy(temp,data,64);
		temp[64] = '\0';
	}
	else
	{
		memcpy(temp,data,dataLen);
		temp[dataLen] = '\0';
	}
	sendAndee(id,temp);
}

void AndeeHelper::setData(int data){
	andeeCommand = SET_DATA;
	char temp[13];
	sprintf(temp,"%d",data);
	sendAndee(id,temp);
}

void AndeeHelper::setData(float data,char decPlace){
	andeeCommand = SET_DATA_FLOAT;
	
	char temp[26];	
	convertFloatToString(temp,data,decPlace);
	
	sendAndee(id,temp);
}

void AndeeHelper::setTitle(const char* title){
	andeeCommand = SET_TITLE;
	char temp[65];
	unsigned int titleLen;
		
	titleLen = strlen(title);
	if(strlen(title) == 0)
	{
		sprintf(temp,"     ");
	}
	else if(titleLen > 64)
	{
		memcpy(temp,title,64);
		temp[64] = '\0';		
	}
	else
	{
		memcpy(temp,title,titleLen);
		temp[titleLen] = '\0';	
	}
	sendAndee(id,temp);
}

void AndeeHelper::setTitle(int title){
	andeeCommand = SET_TITLE;
	char temp[13];
	sprintf(temp,"%d",title);
	sendAndee(id,temp);
}

void AndeeHelper::setTitle(float title, char decPlace){
	andeeCommand = SET_TITLE_FLOAT;
	
	char temp[26];	
	convertFloatToString(temp,title,decPlace);
	
	sendAndee(id,temp);
}

void AndeeHelper::setUnit(const char* unit){
	andeeCommand = SET_UNIT;
	char temp[65];
	unsigned int unitLen;
	
	unitLen = strlen(unit);
	if(unitLen == 0)
	{
		unit = "     ";
	}
	else if(unitLen > 64)
	{
		memcpy(temp,unit,64);
		temp[64] = '\0';
	}
	else
	{
		memcpy(temp,unit,unitLen);
		temp[unitLen] = '\0';
	}
	sendAndee(id,temp);
}

void AndeeHelper::setUnit(int unit){
	andeeCommand = SET_UNIT;
	char temp[13];
	sprintf(temp,"%d",unit);
	sendAndee(id,temp);
}

void AndeeHelper::setUnit(float unit, char decPlace){
	andeeCommand = SET_UNIT_FLOAT;
	
	char temp[26];
	convertFloatToString(temp,unit,decPlace);
	
	sendAndee(id,temp);
}

void AndeeHelper::setMinMax(int min, int max){
	andeeCommand = SET_MIN_MAX;
	
	char temp[23];
	sprintf(temp,"%d,%d",min,max);
	sendAndee(id,temp);
}

void AndeeHelper::setMinMax(float min,float max,char decPlace){
	andeeCommand = SET_MIN_MAX_FLOAT;
	
	char temp[36];
	char minF[15];
	char maxF[15];
	convertFloatToString(minF,min,decPlace);
	convertFloatToString(maxF,max,decPlace);
	
	sprintf(temp,"%s,%s",minF,maxF);
	
	sendAndee(id,temp);
}

void AndeeHelper::setSliderNumIntervals(int numInterval){
	andeeCommand = SET_SLIDER_NUM_INTERVALS;
	char temp;
	if(numInterval <= 223 && numInterval >= 0)
	{
		temp = numInterval + 32;
	}
	else if(numInterval > 223) 
	{
		temp = 255;
	}else if (numInterval <= 0)
	{
		temp = 0;
	}
	sendByteAndee(id,temp);
}

void AndeeHelper::setSliderInitialValue(int value){
	moveSliderToValue(value);
}
void AndeeHelper::setSliderInitialValue(float value,char decPlace){
	moveSliderToValue(value,decPlace);
}

void AndeeHelper::moveSliderToValue(int value){
	andeeCommand = MOVE_SLIDER_TO_VALUE;
	char temp[15];
	itoa(value,temp,10);
	sendAndee(id,temp);
}
void AndeeHelper::moveSliderToValue(float value,char decPlace){
	andeeCommand = MOVE_SLIDER_TO_VALUE;
	char temp[15];	
	convertFloatToString(temp,value,decPlace);
	
	sendAndee(id,temp);
}

bool AndeeHelper::getSliderValue(int* value,int type){
	andeeCommand = GET_SLIDER_VALUE;
	sendAndee(id,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		if(type == INT)
		{
			if(id == rxBuffer[0]-32)
			{
				*value = atoi(rxBuffer+2);
				return true;
			}
			else
			{
				return false;
			}
		}		
	}	
	
	return false;
	
}

bool AndeeHelper::getSliderValue(float* value,float type){
	andeeCommand = GET_SLIDER_VALUE;
	sendAndee(id,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		if(type == FLOAT)
		{
			if(id == rxBuffer[0]-32)
			{
				*value = atof(rxBuffer+2);
				return true;
			}
			else
			{
				return false;
			}
			
		}		
	}	
	
	return false;
	
}

void AndeeHelper::getJoystick(int* xAxis, int* yAxis){
	andeeCommand = GET_JOYSTICK;
	sendAndee(id,EMPTY);
	
	char tempX[5];
	char tempY[5];
	int i;
	
	if(pollRx(rxBuffer))
	{
		if(id == rxBuffer[0] - 32)
		{
			for(i = 0;i<4;i++)
			{
				tempX[i] = rxBuffer[i+2];
				tempY[i] = rxBuffer[i+6];
			}
			tempX[4] = '\0';
			tempY[4] = '\0';
			*xAxis = atoi(tempX);
			*yAxis = atoi(tempY);
		}
	}
}

void AndeeHelper::getKeyboardMessage(char* message){
	andeeCommand = GET_KEYBOARD_MSG;
	sendAndee(id,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		if(id == rxBuffer[0] - 32)
		{
			memcpy(message,rxBuffer + 2,32);
			message[31] = '\0';
		}	
	}
}

void AndeeHelper::getKeyboardMessage(char* message, size_t number){
	andeeCommand = GET_KEYBOARD_MSG;
	sendAndee(id,EMPTY);
	
	if(pollRx(rxBuffer))
	{
		if(id == rxBuffer[0] - 32)
		{
			memcpy(message,rxBuffer + 2,number);
			message[number - 1] = '\0';
		}	
	}	
}

void AndeeHelper::setDefaultDate(int day, int month, int year){
	andeeCommand = SET_DEFAULT_DATE;
	
	char temp[12];
	sprintf(temp,"%02i%02i%04i", day%31, month%12, year);
	sendAndee(id,temp);
}

void AndeeHelper::getDateInput(int* d,int* m, int* y){
	andeeCommand = GET_DATE_INPUT;
	sendAndee(id,EMPTY);
	char day[3];
	char month[3];
	char year[5];
	
	if(pollRx(rxBuffer))		
	{
		if(id == rxBuffer[0] - 32)
		{
			day[0] = rxBuffer[2];
			day[1] = rxBuffer[3];
			day[2] = '\0';
			
			month[0] = rxBuffer[4];
			month[1] = rxBuffer[5];
			month[2] = '\0';
			
			year[0] = rxBuffer[6];
			year[1] = rxBuffer[7];
			year[2] = rxBuffer[8];
			year[3] = rxBuffer[9];
			year[4] = '\0';
			
			*d = atoi(day);
			*m = atoi(month);
			*y = atoi(year);
		}
	}
	
}

void AndeeHelper::setDefaultTime(int hour,int min, int sec){
	andeeCommand = SET_DEFAULT_TIME;
	
	char temp[10];
	sprintf(temp,"%02i%02i%02i", hour%24, min%60, sec%60);
	
	sendAndee(id,temp);
}

void AndeeHelper::getTimeInput(int* h,int* m,int* s){
	andeeCommand = GET_TIME_INPUT;
	sendAndee(id,EMPTY);
	
	char hour[3];
	char minute[3];
	char second[3];
	
	if(pollRx(rxBuffer))
	{
		if(id == rxBuffer[0] - 32)
		{
			hour[0] = rxBuffer[2];
			hour[1] = rxBuffer[3];
			hour[2] = '\0';
			
			minute[0] = rxBuffer[4];
			minute[1] = rxBuffer[5];
			minute[2] = '\0';
			
			second[0] = rxBuffer[6];
			second[1] = rxBuffer[7];
			second[2] = '\0';
			
			*h = atoi(hour);
			*m = atoi(minute);
			*s = atoi(second);
		}
	}
}

int AndeeHelper::isPressed(){
	andeeCommand = IS_PRESSED;
	sendAndee(id,EMPTY);
	
	pollRx(rxBuffer);
	if( (id == (rxBuffer[0] - 32)) && (rxBuffer[1] == ',') )
	{
		return (rxBuffer[2] - 48);
	}
	else{
		return 0;
	}
}

void AndeeHelper::ack(){
	andeeCommand = ACKNOWLEDGE;
	delay(50);
	sendAndee(id,EMPTY);
}


void AndeeHelper::update(){
	update(0);
	delay(50);
}
void AndeeHelper::update(unsigned int loop){
	andeeCommand = UPDATE;
		
	if(loop == 0 || loop == updateLoop)
	{
		sendAndee(id,EMPTY);
		updateLoop = 1;
	}
	else
	{
		updateLoop = updateLoop+1;
		delayMicroseconds(1);
	}
}

void AndeeHelper::remove(){
	andeeCommand = REMOVE;
	sendAndee(id,EMPTY);
	delay(50);
}


/****************************************************************************
 * Functions to pack and send commands to PIC32
 *****************************************************************************/
void sendAndee(unsigned int id,char* message){
	memset(txBuffer,0x00,TX_MAX);
	sprintf(txBuffer,"#%d#%d#%s;",id,andeeCommand,message);	
	
	spiSendData( txBuffer,strlen(txBuffer) );	
}
void sendByteAndee(unsigned int id,char message){
	memset(txBuffer,0x00,TX_MAX);
	sprintf(txBuffer,"#%d#%d#%c;",id,andeeCommand,message);
	
	spiSendData( txBuffer,strlen(txBuffer) );	
}


/****************************************************************************
 * Function to send and receive data to and from PIC32 using SPI
 *****************************************************************************/
void spiSendData(char* txBuffer, size_t bufferLength){
	unsigned int txCount = 0;
	
	SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
	
	digitalWrite(SS_PIN,LOW);
	for(txCount = 0;txCount < bufferLength;txCount++)//send whole buffer
	{		
		SPI.transfer(txBuffer[txCount]);//transfer and receive 1 char in SPI
		delayMicroseconds(40);
	}
	digitalWrite(SS_PIN,HIGH);
	
	SPI.endTransaction();
	delay(10);
}

bool pollRx(char* buffer)
{
	unsigned int rxCount = 0;
	unsigned char tempChar;	
	resetRX();
	delay(3);	
	SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
	
	digitalWrite(SS_PIN,LOW);	
	for(int i = 0;i<RX_DELAY;)
	{
		tempChar = SPI.transfer(0x00);
		if(tempChar > 31)
		{				
			if(tempChar == ';' || tempChar == ']')
			{
				//Serial.print("pollRx:");Serial.println(buffer);
				buffer[rxCount] = '\0';
				
				digitalWrite(SS_PIN,HIGH);
				SPI.endTransaction();
				
				delay(5);
				return true;			
				break;
			}
			else if(tempChar == 173)
			{
				//Serial.println("pollRx: No Reply");
				digitalWrite(SS_PIN,HIGH);
				SPI.endTransaction();
				
				delay(5);
				return false;
				break;
			}
			else
			{
				buffer[rxCount++] = tempChar;	
			}
		}
		else
		{
			i++;
		}
	}	
	digitalWrite(SS_PIN,HIGH);
	
	SPI.endTransaction();
	delay(5);	
	return false;
}

void resetBuffer(char* buffer,unsigned int length)
{
	memset(buffer,0x00,length);
}

void resetRX()
{
	memset(rxBuffer,0x00,RX_MAX);
}


///////////////////////////////////////////////////////////////////////////////////////////////
long power10[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

void convertFloatToString(char* destination,float value, int decPlace){//converts float to long to printf to buffer
	unsigned long convertedInt;
	if(value<0)
	{
		value = value * -1;
		convertedInt = value* power10[decPlace];
		sprintf(destination,"-%lu|%d",convertedInt,decPlace);		
	}
	else
	{
		convertedInt = value* power10[decPlace];
		sprintf(destination,"%lu|%d",convertedInt,decPlace);
	}

	
}


