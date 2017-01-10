// Andee.h - Annikken Andee Library
// Annikken Pte Ltd
// Copyright (c) 2016 Annikken Pte Ltd.  All right reserved.
// Author: Muhammad Hasif

#ifndef	ANDEE_H
#define ANDEE_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined (__arm__)
#include "itoa.h"
#endif

#define BUILD_NUMBER 31
#define BUILD_31

#define IA_TO_CALL_BUF_SIZE 6

    char mSpiTransfer(char);
    bool mGetMessagesFromAndee();

    class AndeeClass{
    private:
        int _sendSDCommands(char*, int, char*, char*, char, int);
    public:
        AndeeClass();
        void begin();
        void begin(int);
		void watchBegin();
        bool sendCommand(char*, char*);
        int readLineFromSD(char*, int, char*, int, char*);
        int readBytesFromSD(char*, int, char*, int);
        int appendSD(char* filename, char* contents, char* errorMsg);
        int writeSD(char* filename, char* contents, int offset, char* errorMsg);
        char checkButtons();        // Store the Button States in the Arduino Buffer
        int isConnected();          // 1 = Connected, 0 = not Connected, -1 = Timeout
        int isConnected(int);
        void clear();               // Clear the Screen on the phone
        void disconnect();          // Disconnect from the connected device
		void getBatt(char*, char*);
        //void printDeviceTime(char*, char*);//Deprecated
        void getDeviceTime(int*, int*, int*);
        void getDeviceDate(int*, int*, int*);
        long getDeviceTimeStamp();
        void setScreenOrientation(int);
        void screenAlwaysOn(bool);
        
        //Bluetooth Functions
        void setupChipSelect(int);
        void setChipSelect(int);
        
        //Watch Functions
        bool useUIMaker();
		void changeScreen(int);
		void showScreen();
		void hideScreen();
		void textInput();
		char watchButtonPress(void);
		
		void AIO_pinMode( int  PinNum,  int Setting);
		void AIO_INPUT_SETTING(char*);
		bool AIO_digitalRead (int  PinNum);
		void AIO_digitalWrite(int  PinNum,  int Setting);

		unsigned int   AIO_INPUTS_READ();
		void AIO_OUTPUT_WRITE(char*);

		void vibrate();

 };

    class AndeeHelper{
    private:
        char _id;
        char _link;
        bool _isCorrectId(char*);
        void _setFloat(char*, float, char);
        void _setDouble(char*, double, char);
        void _setInt(char*, int);
        void _setLong(char*, long);
        void _setULong(char*, unsigned long);
    public:
        AndeeHelper();
        void setId(char);
		
		void simpleDataBox(char,char*,char*);
		void simpleDataBox(char,char*,int);
		void simpleDataBox(char,char*,long);
		void simpleDataBox(char,char*,float);
		void simpleDataBox(char,char*,double);
		
			
		
        void setTitle(char*);
        void setTitle(int);
        void setTitle(long);
        void setTitle(float, char);
        void setTitle(double, char);
        void setUnit(char*);
        void setUnit(int);
        void setUnit(long);
        void setUnit(float, char);
        void setUnit(double, char);
        void setData(char*);
        void setData(int);
        void setData(long);
        void setData(unsigned long);
        void setData(float);
        void setData(float, char);
        void setData(double, char);
        void setType(char);
        void setOrder(char);
        void setRow(char);
        void setSpan(char);
        void setLocation(char, char, char);
        void setCoord(int,int,int,int);
        void setCoordFraction(float,float,float,float);
        void update();
        void update2();
        void updateData(char*);
        void updateData(int);
        void updateData(long);
        void updateData(unsigned long);
        void updateData(float, char);
        void updateData(double, char);
        bool isPressed();   //Check Button Press Status by asking Andee via SPI
        bool isPressed2();  //Check Button Press Status by checking the Buffer in Arduino Library, Call checkButtons() to update Buffer
        int  pressCounter();
        int  isPressed(int);
        void ack();
        void getKeyboardMessage(char*);
		void sendKeyboardMessage(char*);
        void remove();
        void setColor(char*);
        void setTitleColor(char*);
        void setTextColor(char);
        void setTextColor(char*);
        void setTitleTextColor(char);
        void setTitleTextColor(char*);
        void setHintText(char*);
        void setKeyboardType(char);
        void setRecipient(char*);
        void send();
        void setMessage(char*);
        void setTicker(char*);
        void notify();
        void setDefaultTime(int hr, int min, int sec);
        void setDefaultDate(int date, int mth, int year);
        void getTimeInput(char*, char*);
        void getDateInput(char*, char*);
        void getDateInput(int*, int*, int*);
        void getTimeInput(int*, int*, int*);
        void setCamera(char);
        void setAutoFocus(char);
        void setFlash(char);
        void setPhotoFilename(char*);
        void takePhoto();
        bool takePhotoResultReady();
        char getLastTakePhotoResult();
        void setSaveLocation(char);
        void setFileOverwrite(char);
        void requireAck(bool);
        int getButtonPressCount();
		//Watch Funtions
		void setWatchId(char);
        void setWatchTitle(char*);
        void setWatchTitle(int);
        void setWatchTitle(long);
        void setWatchTitle(float, char);
        void setWatchTitle(double, char);
		void setWatchColor(char*);
        void setWatchTitleColor(char*);
		
				
        void setSliderMinMax(float, float, char);
        void setSliderMinMax(int, int);
        void setSliderMinMax(long, long);
        void setSliderInitialValue(float, char);
        void setSliderInitialValue(int);
        void setSliderNumIntervals(int);
        void setSliderReportMode(char);
        void moveSliderToValue(int);
        void moveSliderToValue(float, char);
        bool newSliderValueAvailable();
        int  getSliderValue(int);
        float getSliderValue(float);
        void setSliderColor(char*);

        //for iOS TTS
        void setUtteranceSpeed(float);
        void setPitch(float);
        void setAccent(int);
		
		//GYRO,ACC,LAC,GPS,ORN functions
		//void getGyroReading(char*);
		void getGyroReading(float*,float*,float*);
		void getAccReading (float*,float*,float*);
		void getGravReading(float*,float*,float*);
		void getOrientReading(char*);
		void getLacReading (float*,float*,float*);
		void getGpsReading (float*,float*);
		void getProxReading (char*);
		
        void GYRO_1_INIT();
		void gyroInit(int,int);
		void gyroStop();
		
		void ORN_1_INIT();
		void orientInit(int,int);
		void orientStop();

		void LAC_1_INIT();
		void lacInit(int,int);
		void lacStop();

		void ACC_1_INIT();
		void accInit(int,int);
		void accStop();
		
		void GRV_1_INIT();
		void gravInit(int,int);
		void gravStop();

		void PRX_1_INIT();
		void proxInit(int,int);
		void proxStop();
		
		void GPS_1_INIT();
		void gpsInit(int,int);
		void gpsStop();
        
    };

    extern AndeeClass Andee;

#define setDefaultValue setSliderNumIntervals
#define setInputType setSliderReportMode
#define setMinMax setSliderMinMax

#define setBaseColor setColor
#define setActiveColor setTitleColor

#define Helper AndeeHelper
#define BOOL bool
#define C_HLIMIT 999
#define C_LLIMIT 0
#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 960
    /*************************************************************
     **************       Internal MACROS      *******************
     ************************************************************/
#define NONE 0
#define DTIME 1
#define CONNSTAT 6
#define DTT 7
#define DTD 8
#define DTS 9
#define CLR 10
#define DCN 11
#define UIMAKER 12
#define BEGIN 13
#define CSC 14
#define SSC 15
#define HSC 16
#define INP 17
#define GETBUTTON 20
#define BAT 22
#define VIB 23
#define WATCH_TITLE 44
#define WATCH_BUTTON0 45
#define WATCH_BUTTON1 46
#define WATCH_BUTTON2 47
#define WATCH_BUTTON3 48
#define WATCH_BUTTON4 49

    /*************************************************************
     **************       TYPE MACROS      ***********************
     ************************************************************/
#define DATA_OUT 'C'
#define DATA_OUT_HEADER 'H'
#define DATA_OUT_CIRCLE 'G'
#define ANALOG_DIAL_OUT 'R'
#define BUTTON_IN 'B'
#define CIRCLE_BUTTON 'J'
#define KEYBOARD_IN 'I'
#define DATE_IN 'E'
#define TIME_IN 'F'
#define SLIDER_IN 'Q'
#define CAMERA 'P'
#define TTS 'T'
#define SMS_SENDER 'S'
#define NOTIFICATION 'N'
#define WATCH 'W'
#define VIBRATE '~'


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
     **************      COLOR MACROS      ***********************
     ************************************************************/
#define TEXT_LIGHT '0'
#define TEXT_DARK '1'
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
    // Legacy colour codes
#define DKGRAY "FF444444"
#define LTGRAY "FFCCCCCC"
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

    
    /*************************************************************
     **************      CONNECTION MACROS      ******************
     ************************************************************/
    
#define ANDEE_CONNECTED 1
#define ANDEE_NOTCONNECTED 0
#define ANDEE_TIMEOUT -1
    
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
     **************      SLIDER MACROS      ********************
     ************************************************************/
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
#define US 0
#define GREAT_BRITON 1
#define AUSTRALIA 2
#define IRELAND 3
#define SOUTH_AFRICA 4

#define CONNECTED 1
#define NOT_CONNECTED 0
#define TIMEOUT -1
    
    /*************************************************************
     **************     APPLE WATCH           *************
     ************************************************************/
#define WATCH_1BUTTON_A	0
#define WATCH_2BUTTON_A	1
#define WATCH_3BUTTON_A	2
#define WATCH_4BUTTON_A	3
#define WATCH_1BUTTON_B	4
#define WATCH_2BUTTON_B	5
#define WATCH_3BUTTON_B	6
#define WATCH_4BUTTON_B	7
#define WATCH_GAMEPAD	8


#ifdef  __cplusplus
}
#endif

#endif