/*

Andee Library for Annikken Andee U/iOS and Android

Andee.h

Author: Hasif

*/

#if defined (__arm__)
#include "itoa.h"
#endif

#define RX_DELAY 15
#define RX_MAX 100
#define TX_MAX 128

class AndeeClass
{
	private:
	
	public:
/////////////////////Main Andee Functions//////////////////
	
	void begin();
	void begin(int);
	//This is needed to start using the Annikken Andee. The chip select pin can be changed here
	
	void clear();
	//This function clears the dashboard on the screen of the smartphone/tablet.
	
	void setName(const char*);
	//This function allows you to change the Andee name
	
	void getMACAddress();	
	//Gets the MAC Address of the smartdevice connected to the Andee.(Android Only)
	
	signed char getRSSI();
	//This function gets the RSSI of the smartdevice connected to the Annikken Andee board. Only works when connected to a device for obvious reasons. Will return a value between -103 to -38. If a device is not connected or an error has occurred, -128 will be returned.
	
	void disconnect();
	//This forces the smartphone/tablet to dsconnect its BLE
	
	int isConnected();
	//Check if there is a smartdevice connected to the Andee Board
	
//////////////////////DEVICE TIME,DATE,BATTERY////////////////////	
	void getDeviceTime(int*,int*,int*);
	//This function converts the time from systemTime in millis to a HH:MM:SS 24 hour clock
	
	void getDeviceDate(int*,int*,int*);	
	//This function converts the time from systemTime in millis to a DD/MM/YYYY format date
	
	long getDeviceTimeStamp(void);
	//This function simply returns the time from systemTime in millis if this is the preferred data	
//////////////////////PHONE SENSORS & PERIPHERAL//////////////////////	

	void vibrate();
	//This function will cause the smartdevice to vibrate for roughly 1 second
	
	void gyroInit(int,int);
	//This function activates the gyro on the smartphone/tablet. The device will send gyro data based on the number of iterations and interval
	//interval(time between each sending of gyro data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getGyroReading to get the data
	void gyroStop();
	//Stop all gyro activity
	void getGyroReading(float*,float*,float*);
	//Call this function when using gyroInit. The 3 values will be stored to the user's own declared variables
	
	void lacInit(int,int);
	//This function activates the LAC on the smartphone/tablet. The device will send LAC data based on the number of iterations and interval
	//interval(time between each sending of LAC data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getLacReading to get the data
	void lacStop(); 
	//Stop all LAC activity
	void getLacReading (float*,float*,float*);
	//Call this function when using lacInit. The 3 values will be stored to the user's own declared variables
	
	void gravInit(int,int);
	//This function activates the grav on the smartphone/tablet. The device will send grav data based on the number of iterations and interval
	//interval(time between each sending of grav data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getGravReading to get the data
	void gravStop();
	//Stop all gyro activity
	void getGravReading(float*,float*,float*);
	//Call this function when using gravInit. The 3 values will be stored to the user's own declared variables
	
	void gpsInit(int,int);
	//This function activates the GPS on the smartphone/tablet. The device will send GPS data based on the number of iterations and interval
	//interval(time between each sending of GPS data) any integer
	//iteration(how many data to be sent) any integer. Use -1 if you want to send unlimited amount of data
	//You will need to call the function getGpsReading to get the data
	void gpsStop();
	//Stop all gyro activity
	void getGpsReading (float*,float*);
	//Call this function when using gpsInit. The 3 values will be stored to the user's own declared variables
	
//////////////////////////SMS//////////////////////////

	void sendSMS(char*,char*);
	//This function will make the app send an SMS to the recipient with a message
	
/////////////////Camera////////////////////////

	void takePhoto(char, char, char);
	//This function uses the smartphone/tablet camera to take pictures.
	//cameraType is either FRONT, REAR or CAM_DEFAULT
	//autoFocus AUTO, ON or OFF
	//flash AUTO, ON or OFF
	
//////////////////TTS///////////////////////

	void textToSpeech(char*, float, float, char);
	//This function activates the Text to Speech(TTS) function on the smartphone/tablet. You can place accents, speed and pitch of speech and also the text to be spoken.
	//accents available are US, GREAT_BRITON, AUSTRALIA, IRELAND, SOUTH_AFRICA 
	//speed is a value from 0.0 to 2.0(default is 1.0)
	//pitch is a value from 0.5 to 2.0(default is 1.0)
	//speech is your string that you want to be vocalized	
	
//////////////////////////////////Notification/////////////////////////////////////
	void notification(char*,char*, char*);
	
//////////////////////APPLE WATCH FUNCTIONS//////////////////////
	
	void changeScreen(char);
	//Call this function to change the Apple Watch screen to a particular screen
	
	//Apple Watch Functionality//
	void showScreen();
	//Call this to show a certain screen
	
	void hideScreen();
	//Call this to hide a certain screen
	
	void textInput();
	//Call this to use the voice to text function of the Apple Watch
};
extern AndeeClass Andee;
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////

class AndeeHelper
{
	private:
	unsigned char id;
	unsigned volatile int updateLoop;
	
	public:
	
	void setId(char);
	//This function sets the id for the widget
	
	void setType(char);
	/*
	This function sets the type of widget. The available types for the Andee101 will be 
	DATA_OUT, DATA_OUT_CIRCLE, DATA_OUT_HEADER, 
	BUTTON_IN, CIRCLE_BUTTON,
	ANALOG_DIAL_OUT, 
	KEYBOARD_IN, DATE_IN, TIME_IN, 
	SLIDER_IN, TEXTBOX, TTS, JOYSTICK	
	*/	
		
	void setLocation(char, char, char);
	//One function to set row,order and span at the same time. Arguments are Row,Column, Span.
	//This function will then 
	
	void setCoord(int, int, int, int);//(x,y,w,h) respectively
	//This function sets the position and size of the widget. x and y are the x coordinates and y coordinates respectively. w and h is the width and height of the widget respectively. 
	
	void setInputMode(char);
	//This function is to set an input behaviour, depending on the widget.
	//For Buttons: ACK for an acknowledged button press 
	//             NO_ACK for a multi press button
	/*For Sliders: 
		   ON_FINGER_UP for a slider that updates the value on lifting the finger from the screen
		   ON_VALUE_CHANGE for a slider that updates the value when the slider is moved
		   NO_FINGER for a slider that works similar to s progress bar, with no interaction needed from the user 
				   
	  For KEYBOARD_IN, TIME_IN and DATE_IN:
			ALPHA_NUMERIC for AlphaNumeric Keyboard
			ALPHA_NUMERIC_PW for AlphaNumeric keyboard that hides the typed character after a few seconds
			NUMERIC for a Numeric Keyboard with symbols
			NUMERIC_PW for a Numeric Keyboard that hides typed characters after a few seconds
	*/
	void requireAck(bool);
	
	void setColor(const char*);
	//void setColor(const char);
	//This function is to store the background color into the appropriate buffer
	
	void setTitleColor(const char*);
	//void setTitleColor(const char);
	//This function is to store the title background color into the appropriate buffer
	
	void setTitleTextColor(const char*);
	//void setTitleTextColor(const char);
	//This function is to store the title font color into the appropriate buffer
	
	void setTextColor(const char*);
	//void setTextColor(const char);
	//This function is to store the font color into the appropriate buffer
	
	void setData(const char*);
	//This function stores the string into the dataBuffer
	void setData(int);	
	//This function has the same function as the above function but handles an integer instead
	void setData(float, char);
	//This function has the same function as the above function but handles a float instead	
	
	void setTitle(const char*);
	//This function stores the string into the titleBuffer
	void setTitle(int);	
	//This function has the same function as the above function but handles an integer instead
	void setTitle(float, char);
	//This function has the same function as the above function but handles a float instead	
	
	void setUnit(const char*);
	//This function stores the string into the unitBuffer
	void setUnit(int);	
	//This function has the same function as the above function but handles an integer instead
	void setUnit(float, char);
	//This function has the same function as the above function but handles a float instead	
	
	///////////////////////Used for Slider and Analog Dial Out UI/////////////////////////////
	
	void setMinMax(int,int);
	//This function stores the min and max integer values into the minBuffer and maxBuffer respectively
	void setMinMax(float,float,char);
	//This function works in a similar way as the above function but handles a float instead	
	
	///////////////////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////Slider UI//////////////////////////////////////////
	//////////////Uses setInputMode
	
	void setSliderInitialValue(int);
	//This function sets the initial value the slider will be at. Value is an integer
	void setSliderInitialValue(float, char);	
	//This function works in a similar way as the above function but handles a float instead	
	
	void setSliderNumIntervals(int);
	//This functions sets the number of intervals the slider has
	
	void moveSliderToValue(int);		
	//This function moves the slider to the specified value
	void moveSliderToValue(float, char);	
	//This function works in a similar way as the above function but handles a float instead
	
	bool getSliderValue(int*,int);
	//This function returns the value the slider is at for Integers
	bool getSliderValue(float*,float);
	//This function returns the value the slider is at for Floats	
	
	///////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////Keyboard, Time In and Date In UI//////////////////////////////
	//////////////Uses setInputMode
	
	//void setKeyboardType(char);
	void getKeyboardMessage(char*);
	void getKeyboardMessage(char*,size_t);
	//This function returns the sentence or phrase typed into the smartphone/tablet in the KYBOARD_IN function
	
	void setDefaultDate(int,int,int);
	//When using the type DATE_IN, you can set the default date on the date picker on the smartphone/tablet
	
	void getDateInput(int*,int*,int*);
	//When using the type DATE_IN, this function can retrieve the date input from the smartphone/tablet
	
	void setDefaultTime(int,int,int);
	//When using the type TIME_IN, this function can set the default date on the smartphone/tablet
	
	void getTimeInput(int*,int*,int*);
	//When using the type TIME_IN, this function can retrieve the time input from the smartphone/tablet
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////Button UI////////////////////////////////////////////
	//////////////Uses setInputMode	
	
	int isPressed(void);
	//This function is used to check if a particular button is pressed. This function returns a the number of times the button is pressed and a 0 if no button is pressed
	
	void ack(void);
	//This function returns an acknowledgment to the smartphone/tablet that it has received the button press. This is to allow the button widget to refresh	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////Update & Remove UI//////////////////////////////////////////
	
	void update(void);
	//This function is very important. If this function is not called, the widget will not appear
	void update(int);
    
	void remove();	
	//This function is used to remove the widget. This is useful in situations where only 1 or more widgets needs to be changed. No arguments needed		
	
////////////////////Apple Watch UI////////////////////
	void setWatchId(char);
	void setWatchTitle(char*);
	void setWatchTitle(int);
	void setWatchTitle(long);
	void setWatchTitle(float, char);
	
	void setWatchColor(char*);
	void setWatchTitleColor(char*);	
};


void sendAndee(unsigned int,char*);
void sendByteAndee(unsigned int,char);
void spiSendData(char*, size_t);
bool pollRx(char*);
void resetBuffer(char*,unsigned int);
void resetRX(void);

void convertFloatToString(char*,float, int);
    
enum//For All Andee Commands
{
/////////////////////Main Andee Functions//////////////////
	PLACEHOLDER,
	CLEAR_UI,//0
	SET_ANDEE_NAME,
	GET_MAC_ADDRESS,
	GET_BLE_RSSI,
	DISCONNECT,
	CHECK_CONNECTION,	
/////////////////////DEVICE TIME,DATE,BATTERY//////////////////
	GET_BATTERY_LEVEL,//5
	GET_DEVICE_TIME,
	GET_DEVICE_DATE,
	GET_DEVICE_TIMESTAMP,
	SET_SCREEN_ORIENTATION,
	SET_SCREEN_ALWAYS_ON,//10
///////////////////////PHONE SENSORS & PERIPHERAL/////////////////////
	VIBRATE,
	GYRO_INIT,
	GYRO_STOP,
	GET_GYRO_READING,

	LAC_INIT,//15
	LAC_STOP,
	GET_LAC_READING,

	GRAV_INIT,
	GRAV_STOP,
	GET_GRAV_READING,//20

	GPS_INIT,
	GPS_STOP,
	GET_GPS_READING,

	SEND_SMS,

	TAKE_PHOTO,//25

	TEXT_TO_SPEECH,

	NOTIFICATION,
/////////////////////////////MICROSD FUNCTIONS////////////////////////////////
	APPEND_SD,
	WRITE_SD,
	READ_BYTES_FROM_SD,//30
	READ_STRING_FROM_SD,
///////////////////////////AIO PINS///////////////////////////////
	AIO_PIN_MODE,
	AIO_DIGITAL_WRITE,
	AIO_DIGITAL_READ,
//////////////////////////////APPLE WATCH FUNCTIONS///////////////////////////
	CHANGE_SCREEN,//35
	SHOW_SCREEN,
	HIDE_SCREEN,
	TEXT_INPUT,
/////////////////////////ANDEEHELPER FUNCTIONS/////////////////////////////////
	SET_ID,

	SET_TYPE,//40

	SET_LOCATION,
	SET_COORD,

	SET_INPUT_MODE,

	SET_COLOR,
        SET_TEXT_COLOR,//45
	SET_TITLE_COLOR,
	SET_TITLE_TEXT_COLOR,

	SET_DATA,
	SET_DATA_FLOAT,
	SET_TITLE,//50
	SET_TITLE_FLOAT,
	SET_UNIT,
	SET_UNIT_FLOAT,

	SET_MIN_MAX,
	SET_MIN_MAX_FLOAT,//55

	SET_SLIDER_NUM_INTERVALS,
	MOVE_SLIDER_TO_VALUE,
	GET_SLIDER_VALUE,

	GET_KEYBOARD_MSG,
	SET_DEFAULT_DATE,//60
	GET_DATE_INPUT,
	SET_DEFAULT_TIME,
	GET_TIME_INPUT,

	IS_PRESSED,
	ACKNOWLEDGE,//65
	PRESS_COUNTER,

	UPDATE,
	REMOVE,//68

}andeeCommand;


#define setMainColor setColor
#define setSubColor setTitleColor
#define setKeyboardType setInputMode

#define Helper AndeeHelper
#define SCREEN_UPPER_LIMIT 100
#define SCREEN_LOWER_LIMIT 0

/*************************************************************
 ***************      UI TYPE MACROS      ********************
 ************************************************************/
#define DATA_OUT 'C'
#define DATA_OUT_HEADER 'H'
#define DATA_OUT_CIRCLE 'G'
#define ANALOG_DIAL_OUT 'R'
#define BUTTON_IN 'B'
#define CIRCLE_BUTTON 'J'
#define KEYBOARD_IN 'K'
#define DATE_IN 'D'
#define TIME_IN 'X'
#define SLIDER_IN 'Q'
#define WATCH 'W'

/*************************************************************
 *********       ORIENTATION MACROS      *********************
 ************************************************************/
#define AUTO 0
#define AUTO_PORTRAIT 1
#define AUTO_LANDSCAPE 2
#define PORTRAIT 3
#define PORTRAIT_REVERSE 4
#define LANDSCAPE 5
#define LANDSCAPE_REVERSE 6    
    
/*************************************************************
 **************      CONNECTION MACROS      ******************
 ************************************************************/    
#define ANDEE_CONNECTED 1
#define ANDEE_NOTCONNECTED 0
#define ANDEE_TIMEOUT -1

#define CONNECTED 1
#define NOT_CONNECTED 0
#define TIMEOUT -1

/*************************************************************
 ****************      BUTTON MACROS      ********************
 ************************************************************/	 
#define ACK '0'
#define NOACK NO_ACK
#define NO_ACK '1'	 
    
/*************************************************************
 **************      LOCATION MACROS      ********************
 ************************************************************/ 
#define ONE_THIRD 1
#define TWO_THIRD 2
#define FULL 3
#define ONE_QUART 4
#define HALF 5
#define THREE_QUART 6

/*************************************************************
 ****************      SLIDER MACROS      ********************
 ************************************************************/ 
#define setActiveColor setTitleColor///Used for AnalogDial as well
#define setBaseColor setColor///Used for AnalogDial as well

#define setSliderMinMax setMinMax
#define setSliderReportMode setInputMode

#define INT 0
#define FLOAT (float)0.0
#define NO_FINGER '2'
#define ON_VALUE_CHANGE '1'
#define ON_FINGER_UP '0'

/*************************************************************
 **************      DATE/TIME INPUT MACROS      *************
 ************************************************************/ 
#define Jan 0
#define Feb 1
#define Mar 2
#define Apr 3
#define May 4
#define Jun 5
#define Jul 6
#define Aug 7
#define Sep 8
#define Oct 9
#define Nov 10
#define Dec 11

/*************************************************************
 **************      CAMERA MACROS               *************
 ************************************************************/ 
#define AUTO '0'
#define CAM_DEFAULT '0'
#define FRONT '1'
#define REAR '2'
#define ON '1'
#define OFF '2'

#define SDCARD '1'

/*************************************************************
 **************      KEYBOARD MACROS             *************
 ************************************************************/ 
#define ALPHA_NUMERIC '0'
#define ALPHA_NUMERIC_PW '1'
#define NUMERIC '2'
#define NUMERIC_PW '3'

/*************************************************************
 **************     iOS Text To Speech           *************
 ************************************************************/ 
#define US '0'
#define GREAT_BRITON '1'
#define AUSTRALIA '2'
#define IRELAND '3'
#define SOUTH_AFRICA '4'   

/*************************************************************
 ******************     APPLE WATCH           ****************
 ************************************************************/ 
#define setWatchColor setTitleColor
#define setWatchTextColor setTitleTextColor
#define setWatchText setTitle

#define WATCH_TITLE 45//77
#define WATCH_BUTTON1 46//78
#define WATCH_BUTTON2 47//79
#define WATCH_BUTTON3 48//80
#define WATCH_BUTTON4 49//81

#define WATCH_1BUTTON_A	0
#define WATCH_2BUTTON_A	1
#define WATCH_3BUTTON_A	2
#define WATCH_4BUTTON_A	3
#define WATCH_1BUTTON_B	4
#define WATCH_2BUTTON_B	5
#define WATCH_3BUTTON_B	6
#define WATCH_4BUTTON_B	7
#define WATCH_GAMEPAD	8

/*************************************************************
 **************      COLOR MACROS      ***********************
 ************************************************************/ 
#define TEXT_LIGHT "FFFFFFFF"
#define TEXT_DARK "FF000000"
#define PINK "FFFFC0CB"
#define LIGHT_PINK "FFFFB6C1"
#define HOT_PINK "FFFF69B4"
#define DEEP_PINK "FFFF1493"
#define PALE_VIOLET_RED "FFDB7093"
#define MEDIUM_VIOLET_RED "FFC71585"
#define LIGHT_SALMON "FFFFA07A"
#define SALMON "FFFA8072"
#define DARK_SALMON "FFE9967A"
#define LIGHT_CORAL "FFF08080"
#define INDIAN_RED "FFCD5C5C"
#define CRIMSON "FFDC143C"
#define FIRE_BRICK "FFB22222"
#define DARK_RED "FF8B0000"
#define RED "FFFF0000"
#define ORANGE_RED "FFFF4500"
#define TOMATO "FFFF6347"
#define CORAL "FFFF7F50"
#define DARK_ORANGE "FFFF8C00"
#define ORANGE "FFFFA500"
#define GOLD "FFFFD700"
#define YELLOW "FFFFFF00"
#define LIGHT_YELLOW "FFFFFFE0"
#define LEMON_CHIFFON "FFFFFACD"
#define LIGHT_GOLDENROD_YELLOW "FFFAFAD2"
#define PAPAYA_WHIP "FFFFEFD5"
#define MOCCASIN "FFFFE4B5"
#define PEACH_PUFF "FFFFDAB9"
#define PALE_GOLDENROD "FFEEE8AA"
#define KHAKI "FFF0E68C"
#define DARK_KHAKI "FFBDB76B"
#define CORNSILK "FFFFF8DC"
#define BLANCHED_ALMOND "FFFFEBCD"
#define BISQUE "FFFFE4C4"
#define NAVAJO_WHITE "FFFFDEAD"
#define WHEAT "FFF5DEB3"
#define BURLYWOOD "FFDEB887"
#define TAN "FFD2B48C"
#define ROSY_BROWN "FFBC8F8F"
#define SANDY_BROWN "FFF4A460"
#define GOLDENROD "FFDAA520"
#define DARK_GOLDENROD "FFDAA520"
#define PERU "FFCD853F"
#define CHOCOLATE "FFD2691E"
#define SADDLE_BROWN "FF8B4513"
#define SIENNA "FFA0522D"
#define BROWN "FFA52A2A"
#define MAROON "FF800000"
#define DARK_OLIVE_GREEN "FF556B2F"
#define OLIVE "FF808000"
#define OLIVE_DRAB "FF6B8E23"
#define YELLOW_GREEN "FF9ACD32"
#define LIME_GREEN "FF32CD32"
#define LIME "FF00FF00"
#define LAWN_GREEN "FF7CFC00"
#define CHARTREUSE "FF7FFF00"
#define GREEN_YELLOW "FFADFF2F"
#define SPRING_GREEN "FF000FF7F"
#define MEDIUM_SPRING_GREEN "FF00FA9A"
#define LIGHT_GREEN "FF90EE90"
#define PALE_GREEN "FF98FB98"
#define DARK_SEA_GREEN "FF8FBC8F"
#define MEDIUM_SEA_GREEN "FF3CB371"
#define SEA_GREEN "FF2E8B57"
#define FORREST_GREEN "FF228B22"
#define GREEN "FF008000"
#define DARK_GREEN "FF006400"
#define MEDIUM_AQUAMARINE "FF66CDAA"
#define AQUA "FF00FFFF"
#define CYAN "FF00FFFF"
#define LIGHT_CYAN "FFE0FFFF"
#define PALE_TURQUOISE "FFAFEEEE"
#define AQUAMARINE "FF7FFFD4"
#define TURQUOISE "FF40E0D0"
#define MEDIUM_TURQUOISE "FF48D1CC"
#define DARK_TURQUOISE "FF00CED1"
#define LIGHT_SEA_GREEN "FF20B2AA"
#define CADET_BLUE "FF5F9EA0"
#define DARK_CYAN "FF008B8B"
#define TEAL "FF008080"
#define LIGHT_STEEL_BLUE "FFB0C4DE"
#define POWDER_BLUE "FFB0E0E6"
#define LIGHT_BLUE "FFADD8E6"
#define SKY_BLUE "FF87CEEB"
#define LIGHT_SKY_BLUE "FF87CEFA"
#define DEEP_SKY_BLUE "FF00BFFF"
#define DODGER_BLUE "FF1E90FF"
#define CORNFLOWER_BLUE "FF6495ED"
#define STEEL_BLUE "FF4682B4"
#define ROYAL_BLUE "FF4169E1"
#define BLUE "FF0000FF"
#define MEDIUM_BLUE "FF0000CD"
#define DARK_BLUE "FF00008B"
#define NAVY "FF000080"
#define MIDNIGHT_BLUE "FF191970"
#define LAVENDER "FFE6E6FA"
#define THISTLE "FFD8BFD8"
#define PLUM "FFDDA0DD"
#define VIOLET "FFEE82EE"
#define ORCHID "FFDA70D6"
#define FUCHSIA "FFFF00FF"
#define MAGENTA "FFFF00FF"
#define MEDIUM_ORCHID "FFBA55D3"
#define MEDIUM_PURPLE "FF9370DB"
#define BLUE_VIOLET "FF8A2BE2"
#define DARK_VIOLET "FF9400D3"
#define DARK_ORCHID "FF9400D3"
#define DARK_MAGENTA "FF8B008B"
#define PURPLE "FF800080"
#define INDIGO "FF4B0082"
#define DARK_SLATE_BLUE "FF483D8B"
#define SLATE_BLUE "FF6A5ACD"
#define MEDIUM_SLATE_BLUE "FF7B68EE"
#define WHITE "FFFFFFFF"
#define SNOW "FFFFFAFA"
#define HONEYDEW "FFF0FFF0"
#define MINT_CREAM "FFF5FFFA"
#define AZURE "FFF0FFFF"
#define ALICE_BLUE "FFF0F8FF"
#define GHOST_WHITE "FFF8F8FF"
#define WHITE_SMOKE "FFF5F5F5"
#define SEASHELL "FFFFF5EE"
#define BEIGE "FFF5F5DC"
#define OLD_LACE "FFFDF5E6"
#define FLORAL_WHITE "FFFFFAF0"
#define IVORY "FFFFFFF0"
#define ANTIQUE_WHITE "FFFAEBD7"
#define LINEN "FFFAF0E6"
#define LAVENDER_BLUSH "FFFFF0F5"
#define MISTY_ROSE "FFFFE4E1"
#define GAINSBORO "FFDCDCDC"
#define LIGHTGRAY "FFD3D3D3"
#define SILVER "FFC0C0C0"
#define DARK_GRAY "FFA9A9A9"
#define GRAY "FF808080"
#define DIM_GRAY "FF696969"
#define LIGHT_SLATE_GRAY "FF778899"
#define SLATE_GRAY "FF708090"
#define DARK_SLATE_GRAY "FF2F4F4F"
#define BLACK "FF000000"
#define TRANSPARENT "00FFFFFF"
#define DEFAULT_COLOR "FF83A4C8"
#define DEFAULT_TITLE_COLOR "FF2E5B82"
    //THEME COLORS
#define THEME_TURQUOISE "FF1ABC9C"
#define THEME_TURQUOISE_DARK "FF16A085"
#define THEME_GREEN "FF2ECC71"
#define THEME_GREEN_DARK "FF27AE60"
#define THEME_BLUE "FF3498DB"
#define THEME_BLUE_DARK "FF2980B9"
#define THEME_PURPLE "FF9B59B6"
#define THEME_PURPLE_DARK "FF8E44AD"
#define THEME_MIDNIGHT "FF34495E"
#define THEME_MIDNIGHT_DARK "FF2C3E50"
#define THEME_YELLOW "FFF1C40F"
#define THEME_YELLOW_DARK "FFF39C12"
#define THEME_ORANGE "FFE67E22"
#define THEME_ORANGE_DARK "FFD35400"
#define THEME_RED "FFE74C3C"
#define THEME_RED_DARK "FFC0392B"
#define THEME_GREY "FF95A5A6"
#define THEME_GREY_DARK "FF7F8C8D"
#define THEME_DEFAULT DEFAULT_COLOR // Please set the Andee Blue Colour
#define THEME_DEFAULT_DARK DEFAULT_TITLE_COLOR // Please set  the Andee Dark Blue Colour
