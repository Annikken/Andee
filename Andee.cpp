// Andee.cpp - Annikken Andee Library
// Annikken Pte Ltd
// Copyright (c) 2016 Annikken Pte Ltd.  All right reserved.
// Author: Muhammad Hasif
// Version: 31.1
// Dated: 22 Nov 16

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Andee.h>
#include <SPI.h>

#define INCOMING_BUF_SIZE 20
#define FBUF_SIZE 15
#define INT_CHAR_BUF_SIZE 8
#define LONG_CHAR_BUF_SIZE 11

bool debug = false;
//Set this to true if you want to see certain debug print
//Sadly the print statement has been commented away as they take up memory too

char _incomingBuffer[INCOMING_BUF_SIZE];
char _cmdBuf[5];
char _idBuf[3];
char _intCharBuf[INT_CHAR_BUF_SIZE];
char _longCharBuf[LONG_CHAR_BUF_SIZE];
char _fBuf[FBUF_SIZE];
char _cmdResult[3];

int errorReturn = -21474;

char _buttonBuffer[102];

int _cs = 8; // Default Chipselect Pin to be 8
int watchFlag;

static char  AIO_TRIS[9]= {'2','2','2' ,'2','2','2','2' ,'2','\0'};  // ''o output , ''1'' as input , '2'' as not used.

/****************************************
 ****************************************
 ****************************************
 *
 *
 *  Independent functions
 *
 *
 *
 ****************************************
 ****************************************
 ***************************************/



char mSpiTransfer(char data)        // Send 1 Byte
{
    char rtnChar;
    digitalWrite(_cs, LOW);         // Flag Low on _cs to get it ready to receive bytes
    delayMicroseconds(4);           // Let the signal sink in
    rtnChar = SPI.transfer(data);   // Transfer the Data
    digitalWrite(_cs, HIGH);        // Flag High on _cs to get it stop 	receiving bytes	
    return rtnChar;
}



boolean mGetReply(){
    int count;
    char rData;
    
    rData = mSpiTransfer(0x00);
    count = strlen(_incomingBuffer);
    
    delay(2); //wake module
    
    while(rData != 0x00 && rData > 0){
        _incomingBuffer[count++] = rData;
        rData = mSpiTransfer(0x00);
    }
    
    _incomingBuffer[count] = '\0';
    
    
    if(count > 0){
        return true;
    }
    else{
        return false;
    }
}

boolean mGetLongMessagesFromAndee(char* buffer){
    int count;
    int bCount;
    char rData;
    
    
    rData = mSpiTransfer(0x00);
    
    count = strlen(_incomingBuffer);
    bCount = strlen(buffer);
    delay(2); //wake module
    
    //Serial.println("get messages");
    while(rData != 0x00 && rData > 0){
        if(count > 5)
            buffer[bCount++] = rData;
        else
            _incomingBuffer[count++] = rData;
        
        rData = mSpiTransfer(0x00);
    }
    
    buffer[bCount] = '\0';
    if((count + bCount)> 0){
        
        //Serial.println("retug true");
        return true;
    }
    else{
        // Serial.println("retug false");
        return false;
    }
}

boolean mGetCommandMessagesFromAndee(char* buffer){
    int count;
    int bCount;
    char rData;
    char cmdCount;
    
    rData = mSpiTransfer(0x00);
    
    count = strlen(_incomingBuffer);
    //bCount = strlen(buffer);
    bCount = 0;
    delay(2); //wake module
    
    cmdCount = 0;
	
	//Serial.print("buffer:");Serial.println(rData,HEX);
    
    while(rData != 0x00 && rData > 0){
        if(count > 7){
            buffer[bCount++] = rData;
            count++;
        }
        else if(count > 5){
            _cmdResult[cmdCount++] = rData;
            count++;
        }
        else{
            _incomingBuffer[count++] = rData;
        }
        rData = mSpiTransfer(0x00);
    }
    
    buffer[bCount] = '\0';
    _cmdResult[2] = '\0';
    
    //Serial.println(_cmdResult);
    //Serial.println(_incomingBuffer);
    //Serial.println(buffer);
    
    if((count + bCount)> 0){
        return true;
    }
    else{
        return false;
    }
}




bool mGetSDReplies(char* buffer, int* position){
    int count;
    int bCount;
    int pCount;
    int errMsgCount;
    char rData;
    char cmdCount;
    bool positionCollected;
    bool errCollected;
    //char errorMsg[10];
    
    rData = mSpiTransfer(0x00);
    
    count = strlen(_incomingBuffer);
    bCount = strlen(buffer);
    delay(2); //wake module
    
    cmdCount = 0;
    pCount = 0;
    errMsgCount = 0;
    
    memset(_fBuf, 0x00, FBUF_SIZE);
    positionCollected = false;
    errCollected = false;
    _cmdResult[0] = 0x00;
    _cmdResult[1] = 0x00;
    
    while(rData != 0x00 && rData > 0){
        if(count > 7){
            if(_cmdResult[0] == 'S'){
                if(!positionCollected){
                    if(rData != ','){
                        _fBuf[pCount++] = rData;
                        count++;
                    }
                    else{
                        positionCollected = true;
                        (*position) = atoi(_fBuf);
                        count++;
                    }
                }
                else{
                    buffer[bCount++] = rData;
                    count++;
                }
            }
            else{
                
                if(!errCollected){
                    if(rData != ','){
                        buffer[bCount++] = rData;
                        count++;
                    }
                    else{
                        errCollected = true;
                        count++;
                    }
                }
                else{
                    if(!positionCollected){
                        if(rData != ','){
                            _fBuf[pCount++] = rData;
                            count++;
                        }
                        else{
                            positionCollected = true;
                            (*position) = atoi(_fBuf);
                            count++;
                        }
                    }
                    else{
                        buffer[bCount++] = rData;
                        count++;
                    }
                }
            }
            
        }
        else if(count > 5){
            _cmdResult[cmdCount++] = rData;
            count++;
        }
        else{
            _incomingBuffer[count++] = rData;
        }
        rData = mSpiTransfer(0x00);
    }
    
    if(_cmdResult[0] == 'E'){
        (*position) = atoi(_fBuf);
    }
    
    buffer[bCount] = '\0';
    _cmdResult[2] = '\0';
    
    //Serial.println(_cmdResult);
    //Serial.println(_incomingBuffer);
    //Serial.println(buffer);
    
    if((count + bCount)> 0){
        return true;
    }
    else{
        return false;
    }
}

void mReplaceChars(char* buffer){
    int len;
    int i;
    
    len = strlen(buffer);
    for(i=0; i<len; i++){
        if(buffer[i] == ';'){
            
            buffer[i] = 0x05;
        }
    }
}

bool mIsCorrectId(char* address, char id){
    char iid;
    //Serial.print("checking:");
    //Serial.println(address);
    if(strlen(address) < 6)
        return false;
    if(address[0] =='#' && address[5] =='#'){
        iid = atoi(address+3);
        if(iid == id){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

BOOL mTextAndee2(char link, char id, char* cmd, char* buffer){
    
    delay(50);
    int i;
    char rData;
    char len;
    int timeout = 300; //for android 50*3,
    
    if(id < 0) return false;
    
    //transfer header info
    mSpiTransfer('#');
    delay(2);
    mSpiTransfer('0'); //link
    mSpiTransfer(',');
    mSpiTransfer('0');
    mSpiTransfer('0');
    itoa(id, _idBuf, 10);
    len =strlen(_idBuf);
    if(len == 1){
        mSpiTransfer('0');
        mSpiTransfer(_idBuf[0]);
    }
    else{
        mSpiTransfer(_idBuf[0]);
        mSpiTransfer(_idBuf[1]);
    }
    mSpiTransfer('#');
    //Finish transferring header info
    
    //transfer command
    mSpiTransfer(cmd[0]);
    mSpiTransfer(cmd[1]);
    mSpiTransfer(cmd[2]);
    mSpiTransfer(cmd[3]);
    mSpiTransfer(',');
    
    
    
    //finish transfer command
    
    //Serial.print(cmd);
    
    //transfer any additional content
    i=0;
    while(buffer[i] != 0x00){
        rData = mSpiTransfer(buffer[i++]);
    }
    rData = mSpiTransfer(';');
    //finish extra content
    
    //Detect till idling
    //delayMicroseconds(25);
    
    while(rData != 0x00 && rData > 0){
        rData = mSpiTransfer(0x00);
    }
    
    memset(_incomingBuffer, 0x00, INCOMING_BUF_SIZE);
    
    for(i=0; i<timeout; i++){
        
        if(mGetReply()){
            len = strlen(_incomingBuffer);
            if(len >= 8){
                
                if(!mIsCorrectId(_incomingBuffer, id)){
                    
                }
                
                if(strncmp("OK", _incomingBuffer+6, 2) == 0){
                    memset(_incomingBuffer, 0x00, INCOMING_BUF_SIZE);
                    if(debug){
                        //Serial.print(i);
                        //Serial.print("  ");
                    }
                    
                    
                    //Serial.print(" - Succeed  in ");
                    //Serial.println(i);
                    return true;
                }
                else{
                    if(debug){
                        Serial.print(i);
                        Serial.print("  ");
                    }
                    Serial.println(" - Fail");
                    return false;
                }
            }
            
        }
        else{
            
        }
        
    }
    
    //Serial.print(id, DEC);
    //Serial.println(" - timeout");
    return false;
}
//Robin
void mTextAndeeNoVerify(char link, char id, char* cmd, char* buffer, int type){
    unsigned char i;
    char rData;
    char len;
    //char delayAmount;
    //int timeout = 50*3; //milliseconds * 5
    //unsigned char timeout = 20; //multiply by 2ms.
    
    if(id < 0) return;
    
    //transfer header info
    mSpiTransfer('#');
    delay(2);
    mSpiTransfer('0'); //link
    mSpiTransfer(',');
    mSpiTransfer('0');
    mSpiTransfer('0');
    itoa(id, _idBuf, 10);
	len =strlen(_idBuf);
    if(len == 1){
        mSpiTransfer('0');
        mSpiTransfer(_idBuf[0]);
    }
    else{
        mSpiTransfer(_idBuf[0]);
        mSpiTransfer(_idBuf[1]);
    }
    mSpiTransfer('#');
    //Finish transferring header info
    
    //transfer command
    mSpiTransfer(cmd[0]);
    mSpiTransfer(cmd[1]);
    mSpiTransfer(cmd[2]);
    mSpiTransfer(cmd[3]);
    mSpiTransfer(',');
    //finish transfer command
    
    switch(type){
        case DTIME:
            mSpiTransfer('D');
            mSpiTransfer('T');
            mSpiTransfer('I');
            mSpiTransfer('M');
            mSpiTransfer('E');
            mSpiTransfer(',');
            break;
        case CONNSTAT:
            mSpiTransfer('C');
            mSpiTransfer('O');
            mSpiTransfer('N');
            mSpiTransfer('N');
            mSpiTransfer('S');
            mSpiTransfer('T');
            mSpiTransfer('A');
            mSpiTransfer('T');
            mSpiTransfer(',');
            break;
        case GETBUTTON:
            mSpiTransfer('B');
            mSpiTransfer('U');
            mSpiTransfer('T');
            mSpiTransfer('T');
            mSpiTransfer('O');
            mSpiTransfer('N');
            mSpiTransfer('S');
            mSpiTransfer(',');
            
            break;
            
        case DTT:
            mSpiTransfer('D');
            mSpiTransfer('T');
            mSpiTransfer('T');
            mSpiTransfer(',');
            break;
        case DTD:
            mSpiTransfer('D');
            mSpiTransfer('T');
            mSpiTransfer('D');
            mSpiTransfer(',');
            break;
        case DTS:
            mSpiTransfer('D');
            mSpiTransfer('T');
            mSpiTransfer('S');
            mSpiTransfer(',');
            break;
        case CLR:
            mSpiTransfer('_');
            mSpiTransfer('C');
            mSpiTransfer('L');
            mSpiTransfer('R');
            mSpiTransfer(',');
            break;
        case DCN:
            mSpiTransfer('_');
            mSpiTransfer('D');
            mSpiTransfer('C');
            mSpiTransfer('N');
            mSpiTransfer(',');
            break;
        case UIMAKER:
            mSpiTransfer('_');
            mSpiTransfer('U');
            mSpiTransfer('I');
            mSpiTransfer('M');
            mSpiTransfer(',');
            break;
        case BEGIN:
            mSpiTransfer('_');
            mSpiTransfer('B');
            mSpiTransfer('G');
            mSpiTransfer('N');
            mSpiTransfer(',');
            break;
        case CSC:
            mSpiTransfer('_');
            mSpiTransfer('C');
            mSpiTransfer('S');
            mSpiTransfer('C');
            mSpiTransfer(',');
            break;
        case HSC:
            mSpiTransfer('_');
            mSpiTransfer('H');
            mSpiTransfer('S');
            mSpiTransfer('C');
            mSpiTransfer(',');
            break;
        case SSC:
            mSpiTransfer('_');
            mSpiTransfer('S');
            mSpiTransfer('S');
            mSpiTransfer('C');
            mSpiTransfer(',');
            break;
        case INP:
            mSpiTransfer('_');
            mSpiTransfer('I');
            mSpiTransfer('N');
            mSpiTransfer('P');
            mSpiTransfer(',');
			break;
		case BAT:
			mSpiTransfer('_');
            mSpiTransfer('B');
            mSpiTransfer('A');
            mSpiTransfer('T');
            mSpiTransfer(',');
			break;
		case VIB:
			mSpiTransfer('_');
            mSpiTransfer('V');
            mSpiTransfer('I');
            mSpiTransfer('B');
            mSpiTransfer(',');
			break;
    }
    
    //transfer any additional content
    i=0;
    
    while(buffer[i] != 0x00){
        rData = mSpiTransfer(buffer[i++]);
        //Serial.println(buffer[i], DEC);
        //i++;
    }
    mSpiTransfer(';');
    //finish extra content
    
    //Detect till idling
    delayMicroseconds(50);
    while(rData != 0x00 && rData > 0){
        rData = mSpiTransfer(0x00);
    }
    
    
    memset(_incomingBuffer, 0x00, INCOMING_BUF_SIZE);
    //delay(10);	
    
    return;
    
}

char* mCommand(char a, char b, char c){
    _cmdBuf[0] = '_';
    _cmdBuf[1] = a;
    _cmdBuf[2] = b;
    _cmdBuf[3] = c;
    _cmdBuf[4] = 0x00;
    return _cmdBuf;
}



/****************************************
 ****************************************
 ****************************************
 *
 *
 *  Functions for Class
 *
 *
 *
 ****************************************
 ****************************************
 ***************************************/


AndeeClass::AndeeClass(){
    
}
#if defined (__arm__)
void AndeeClass::begin(){
    
    pinMode(_cs, OUTPUT);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(_cs, 21);
    SPI.begin();
    delay(2500);
}

#else
void AndeeClass::begin(){
    int i;
    int timeout = 50*8;
    
    pinMode(_cs, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, INPUT);
    pinMode(13, OUTPUT);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    
    digitalWrite(_cs, HIGH);
    
    //From source of SPI.begin();
    pinMode(SS, OUTPUT);
    
    // Warning: if the SS pin ever becomes a LOW INPUT then SPI
    // automatically switches to Slave, so the data direction of
    // the SS pin MUST be kept as OUTPUT.
    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);
    
    // Set direction register for SCK and MOSI pin.
    // MISO pin automatically overrides to INPUT.
    // By doing this AFTER enabling SPI, we avoid accidentally
    // clocking in a single bit since the lines go directly
    // from "input" to SPI control.
    // http://code.google.com/p/arduino/issues/detail?id=888
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    
    //SPI.begin();
    delay(2500);
    
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, BEGIN);
    for(i=0; i<timeout; i++){
        
        if(mGetLongMessagesFromAndee(_intCharBuf)){
            
            if(_intCharBuf[0] == 'S' && _intCharBuf[2] == 'T'){
                memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                //Serial.println("ACK RCVD");
                return;
            }
            else{
                memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                //Serial.println("SME ELSE RCVD");
                return;
            }
            
        }
        else{
            //return -1;
        }
    }
    //return -1; //timeout
    //Serial.println("timeout");
    //delay(2500);
}
#endif

bool AndeeClass::useUIMaker(){
    int i;
    int timeout = 50*3;
    
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, UIMAKER);
    for(i=0; i<timeout; i++){
        
        if(mGetLongMessagesFromAndee(_intCharBuf)){
            
            if(_intCharBuf[0] == 'S' && _intCharBuf[2] == 'T'){
                memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                
                return 1;
            }
            else{
                memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                
                return 0;
            }
            
        }
        else{
            //return -1;
        }
    }
    
    return -1; //timeout
}

void AndeeClass::setupChipSelect(int pin){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void AndeeClass::setChipSelect(int pin){
    _cs = pin;
}

void AndeeClass::begin(int pin){
    _cs = pin;
    begin();
}

/* void AndeeClass::watchBegin (){
	watchFlag = true ;
	begin();
 } */

void AndeeClass::clear(){
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, CLR);
    delay(80);
}

void AndeeClass::changeScreen(int screen){
    sprintf(_longCharBuf, "_CSC,0,%i", screen);
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), _longCharBuf, NONE );
    delay(80);
}

void AndeeClass::showScreen() {
    mTextAndeeNoVerify(0,99, mCommand('C','M','D'), NULL, SSC );
    delay(80);
}

void AndeeClass::hideScreen() {
    mTextAndeeNoVerify(0,99, mCommand('C','M','D'), NULL, HSC );
    delay(80);
}

void AndeeClass::textInput() {
    mTextAndeeNoVerify(0,99, mCommand('C','M','D'), NULL, INP );
    delay(80);
}


void AndeeClass::disconnect(){
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, DCN);
}

char AndeeClass::checkButtons(){
    
    int i;
    int timeout = 100;
    memset(_buttonBuffer, 0x00, 102);
    
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, GETBUTTON);
    for(i=0; i<timeout; i++){
     //   Serial.print(".");
        if(mGetLongMessagesFromAndee(_buttonBuffer)){
            
            if(_buttonBuffer[0] == 'B' && _buttonBuffer[1] == 'U'){
                if(debug){
                    // Serial.print("CheckButtons Successful - ");
                    //  Serial.println(i);
                }
                return 1;
            }
            else{
                if(debug){
                    // Serial.print("CheckButtons Fail - ");
                    //  Serial.println(i);
                }
                return 0;
            }
            
        }
        else{
        }
    }
    //  Serial.println("CheckButtons Timeout");
    if(debug){
        //  Serial.println("CheckButtons Timeout");
    }
    return -1; //timeout
    
}


int AndeeClass::isConnected(){
    return isConnected(100);
}
int AndeeClass::isConnected(int timeout){
    //Serial.print("Connection ");
    int i;
	delay(50);
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, CONNSTAT);
	delay(10);
    for(i=0; i<timeout; i++){
        
        if(mGetLongMessagesFromAndee(_intCharBuf)){
            
            if(_intCharBuf[0] == 'S' && _intCharBuf[2] == 'T'){
                memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                // Andee is Connected
                if(debug){
                    //    Serial.println("Andee Connection Check - Connected");
                }
                return 1;
            }
            else{
                memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                // Andee is Not Connected
                if(debug){
                    //     Serial.println("Andee Connection Check - Not Connected");
                }
                return 0;
            }
            
        }
    }
    if(debug){
        //      Serial.println("Andee Connection Check - Timeout");
    }
    return -1;
}
//int* battlevel, int*maxbatt , int* charging
/* void AndeeClass::getBatt(char* buffer )
{
    int i;
    int timeout = 50*3;
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, BAT);   
	
	buffer[0] = 0x00;
	for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(buffer)){
              
            return;            
        }
    }
        
} */

void AndeeClass::getBatt(char* battlevel, char* connection)
{
	int i;	
    int timeout = 50*3;
	char buffer[3];
	
	mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, BAT);   
	
	memset(buffer,0x00,3);	
	delay(1);
	for(i=0; i<timeout; i++)
	{
        if(mGetLongMessagesFromAndee(buffer))
		{
			//Serial.print("buffer:");
			//Serial.println(buffer);
			
			if(buffer[1] = ',')		
			{
			sprintf(battlevel,"%d",(buffer[0]-32));
			sprintf(connection,"%d",(buffer[2]-32));
			}			
        }		
    }
	
	
	//Serial.print(battlevel);
	//Serial.print("  - ");
	//Serial.println(connection);
}


/* void AndeeClass::printDeviceTime(char* format, char* reply){//Deprecated
    int i;
    int timeout = 50*4;
	char datareply[128];
	memset(datareply,0x00,128);
    delay(5);
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), format, DTIME);
	delay(5);
    reply[0] = 0x00;
    
     for(i=0; i<timeout; i++){
        if(mGetCommandMessagesFromAndee(datareply))
		{
			memcpy(reply,datareply,strlen(datareply));
			//Serial.print("REPLY:");
			//Serial.println(reply);
			return;			
        }
        else{
            //Serial.print("REPLYelse:");Serial.println(reply);
        }
    }
    //Serial.println("timeout");
} */
void AndeeClass::getDeviceTime(int* h, int* m, int* s){
    int i;
    int timeout = 50*4;
    //sendCommand("DTT", buffer);
	delay(5);
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, DTT);
    delay(5);
    _longCharBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetCommandMessagesFromAndee(_longCharBuf)){
            *s = atoi(_longCharBuf+4);
            _longCharBuf[4] = 0x00;
            *m = atoi(_longCharBuf+2);
            _longCharBuf[2] = 0x00;
            *h = atoi(_longCharBuf);
            return;
        }
        else{
            //return -1;
        }
    }
    
}
void AndeeClass::getDeviceDate(int* d, int* m, int* y){
    int i;
    int timeout = 50*4;
	delay(5);
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, DTD);
	delay(5);
    _longCharBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetCommandMessagesFromAndee(_longCharBuf)){
            *y = atoi(_longCharBuf+4);
            _longCharBuf[4] = 0x00;
            *m = atoi(_longCharBuf+2);
            _longCharBuf[2] = 0x00;
            *d = atoi(_longCharBuf);
            return;
        }
        else{
            //return -1;
        }
    }
}
long AndeeClass::getDeviceTimeStamp(){
    int i;
    int timeout = 50*3;
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, DTS);
    _fBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetCommandMessagesFromAndee(_fBuf)){
            return atol(_fBuf);
        }
        else{
            //return -1;
        }
    }
    return 0;
}

void AndeeClass::setScreenOrientation(int orientation){
    int i;
    int timeout = 50* 3;
    
    sprintf(_longCharBuf, "_SOW,0,%i", orientation);
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), _longCharBuf, NONE);
    for(i=0; i<timeout; i++){
        if(mGetCommandMessagesFromAndee(_intCharBuf)){
            return;
        }
        else{
        }
    }
}

void AndeeClass::screenAlwaysOn(bool b){
    int i;
    int timeout = 50*3;
    
    if(b){
        mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), "_SOW,1,1", NONE);
        //sendCommand("_SOW,1,1", NULL);
    }
    else{
        mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), "_SOW,1,0", NONE);
        //sendCommand("_SOW,1,0", NULL);
    }
    for(i=0; i<timeout; i++){
        if(mGetCommandMessagesFromAndee(_intCharBuf)){
            return;
        }
        else{
            //return -1;
        }
    }
}

bool AndeeClass::sendCommand(char* cmd, char* replyBuffer){
    int i;
    int timeout = 50*3;
    
    mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), cmd, NONE);
    
    //_intCharBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetCommandMessagesFromAndee(replyBuffer)){
            //Serial.print("Cmd Msg Reply:");
            //Serial.println(replyBuffer);
            return true;
        }
        else{
            //return -1;
        }
    }
    //Serial.println("send cmd timeout");
    return false;
}

AndeeClass Andee;

int AndeeClass::readLineFromSD(char* filename, int offset, char* replyBuffer, int maxLen, char* delimiter){
    return _sendSDCommands(filename, offset, replyBuffer, delimiter, 4, maxLen);
}
int AndeeClass::appendSD(char* filename, char* contents, char* errorMsg){
    return _sendSDCommands(filename, 0, errorMsg, contents, 2, 0);
}

int AndeeClass::writeSD(char* filename, char* contents, int offset, char* errorMsg){
    return _sendSDCommands(filename, offset, errorMsg, contents, 3, 0);
}
int AndeeClass::readBytesFromSD(char* filename, int offset, char* replyBuffer, int numBytesToRead){
    return _sendSDCommands(filename, offset, replyBuffer, NULL, 1, numBytesToRead);
}

void _transferInteger(int v){
    int len;
    int i;
    memset(_fBuf, 0x00, FBUF_SIZE);
    sprintf(_fBuf, "%i", v);
    len = strlen(_fBuf);
    for(i=0; i<len; i++){
        mSpiTransfer(_fBuf[i]);
    }
}

int AndeeClass::_sendSDCommands(char* filename, int offset, char* replyBuffer, char* contents, char type, int numBytesToRead){
    
    bool incoming;
    int timeout = 50*5; //milliseconds * 5
    char i;
    int result;
    char position[10];
    int pos;
    int replyLen;
    int errorLen;
    int delayAmount;
    char rData;
    char len;
    
    
    mSpiTransfer('#');
    delay(2);
    mSpiTransfer('0'); //link
    mSpiTransfer(',');
    mSpiTransfer('0');
    mSpiTransfer('0');
    mSpiTransfer('9');
    mSpiTransfer('9');
    mSpiTransfer('#');
    //Finish transferring header info
    
    //transfer command
    mSpiTransfer('_');
    mSpiTransfer('C');
    mSpiTransfer('M');
    mSpiTransfer('D');
    mSpiTransfer(',');
    //finish transfer command
    
    mSpiTransfer('S');
    mSpiTransfer('D');
    
    mReplaceChars(contents);
    
    if(type == 0){
        //sprintf(buf, "%i,%04i#_CMD,SDRL,%i,%s,%i;",0, 99, offset, filename, numBytesToRead);
        mSpiTransfer('R');
        mSpiTransfer('L');
        mSpiTransfer(',');
        
        _transferInteger(offset);
        mSpiTransfer(',');
        
        i=0;
        while(filename[i] != 0x00){
            rData = mSpiTransfer(filename[i++]);
        }
        mSpiTransfer(',');
        
        _transferInteger(numBytesToRead);
        mSpiTransfer(';');
        
    }
    else if(type == 1){
        //sprintf(buf, "%i,%04i#_CMD,SDRB,%i,%i,%s;",0, 99, offset, numBytesToRead, filename);
        //Serial.println(buf);
        mSpiTransfer('R');
        mSpiTransfer('B');
        mSpiTransfer(',');
        
        _transferInteger(offset);
        mSpiTransfer(',');
        
        _transferInteger(numBytesToRead);
        mSpiTransfer(',');
        
        i=0;
        while(filename[i] != 0x00){
            rData = mSpiTransfer(filename[i++]);
        }
        mSpiTransfer(';');
    }
    else if(type == 2){
        //sprintf(buf, "%i,%04i#_CMD,SDAP,%s,%s;",0, 99, filename, contents);
        mSpiTransfer('A');
        mSpiTransfer('P');
        mSpiTransfer(',');
        
        i=0;
        while(filename[i] != 0x00){
            rData = mSpiTransfer(filename[i++]);
        }
        mSpiTransfer(',');
        
        i=0;
        while(contents[i] != 0x00){
            rData = mSpiTransfer(contents[i++]);
        }
        mSpiTransfer(';');
    }
    else if(type == 3){
        //sprintf(buf, "%i,%04i#_CMD,SDWR,%i,%s,%s;",0, 99, offset, filename, contents);
        mSpiTransfer('W');
        mSpiTransfer('R');
        mSpiTransfer(',');
        
        _transferInteger(offset);
        mSpiTransfer(',');
        
        i=0;
        while(filename[i] != 0x00){
            rData = mSpiTransfer(filename[i++]);
        }
        mSpiTransfer(',');
        
        i=0;
        while(contents[i] != 0x00){
            rData = mSpiTransfer(contents[i++]);
        }
        mSpiTransfer(';');
    }
    else if(type == 4){
        //sprintf(buf, "%i,%04i#_CMD,SDRD,%i,%s,%i,%s;",0, 99, offset, filename, numBytesToRead, contents);
        
        mSpiTransfer('R');
        mSpiTransfer('D');
        mSpiTransfer(',');
        
        _transferInteger(offset);
        mSpiTransfer(',');
        
        i=0;
        while(filename[i] != 0x00){
            rData = mSpiTransfer(filename[i++]);
        }
        mSpiTransfer(',');
        
        _transferInteger(numBytesToRead);
        mSpiTransfer(',');
        
        i=0;
        while(contents[i] != 0x00){
            rData = mSpiTransfer(contents[i++]);
        }
        mSpiTransfer(';');
    }
    
    //Detect till idling
    delayMicroseconds(50);
    while(rData != 0x00 && rData > 0){
        rData = mSpiTransfer(0x00);
    }
    
    memset(_incomingBuffer, 0x00, INCOMING_BUF_SIZE );
    replyBuffer[0] = 0x00;
    
    for(i=0; i<timeout; i++){
        if(mGetSDReplies(replyBuffer, &pos)){
            return pos;
        }
    }
    //Serial.println("AndeeClass SD Timeout");
    strcpy(replyBuffer, "TIMEOUT");
    return -1;
}

char AndeeClass::watchButtonPress() {
    int timeout = 10*5; //milliseconds * 5
    int i;
    char count;
    mTextAndeeNoVerify(0, 99, mCommand('W','A','B'), "", NONE);
    
    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_intCharBuf)){
            
            
            count = *_intCharBuf;
            
            return count;
            
        }
        else{
        }
    }
    return count;
}

void AndeeClass::AIO_pinMode( int  PinNum,  int Setting)
{
    int InternalData= 0x32;
    
    if(Setting == 0 )
    {
        InternalData= 0x31;
    }
    else if(Setting == 1)
    {
        InternalData= 0x30;
    }
    
    if((  PinNum < 8 ) &  ( (Setting == 0 ) || (Setting == 1)))
    {
        AIO_TRIS [PinNum] =  InternalData;
        
        AIO_INPUT_SETTING (AIO_TRIS);
        delay(15);
    }
}

void AndeeClass::AIO_digitalWrite( int  PinNum,  int Setting)
{
    char  AIO_OUT[9]= {'2','2','2' ,'2','2','2','2' ,'2','\0'};  // ''o output , ''1'' as input , '2'' as not used.
    
    if((  PinNum < 8 ) &  ( (Setting == 0 ) || (Setting == 1)))
    {
        AIO_OUT [PinNum] =   0x30 +Setting ;
        AIO_OUTPUT_WRITE(AIO_OUT);
        delay(15);
    }
}

bool AndeeClass::AIO_digitalRead (int  PinNum)
{
    int readByte=0;
    int cmpareBit =1;
    
    if((  PinNum < 8 ) )
    {
        readByte =  AIO_INPUTS_READ();
        
        if( readByte &  (cmpareBit<< PinNum))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void AndeeClass::AIO_INPUT_SETTING(char* text){
    mTextAndeeNoVerify(0, 99, mCommand('D','I','S'), text,NONE);
}

void AndeeClass::AIO_OUTPUT_WRITE(char* text){
    //mTextAndee2(_link, _id, mCommand('D','O','W'), text);
    mTextAndeeNoVerify(0, 99, mCommand('D','O','W'), text,NONE);
}

unsigned int  AndeeClass::AIO_INPUTS_READ(){
    int timeout = 10; //milliseconds * 5
    int i;
    unsigned int  count;
    
    
    mTextAndeeNoVerify(0, 99, mCommand('D','I','R'), "", NONE);
    //Serial.println("AIO_READ");
    memset(_incomingBuffer, 0x00, INCOMING_BUF_SIZE);
    //delay(1);
    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
    for(i=0; i<timeout; i++){
        
        if(mGetLongMessagesFromAndee(_intCharBuf)){
			//Serial.println(_intCharBuf);
            
            //if(_isCorrectId(_incomingBuffer))
            {
                //count = abs(_intCharBuf);
				count = strtol(_intCharBuf,NULL,16);
                	  //Serial.println(_intCharBuf);
                
                	  //Serial.println(count);
                return count;
                //	return atoi(_intCharBuf);
            }
        }
        
    }
    _incomingBuffer[0] = 0x00;
    //Serial.println("is pressed timeout");
    return 0;
}

void AndeeClass::vibrate()
{
	mTextAndeeNoVerify(0, 99, mCommand('C','M','D'), NULL, VIB);
}


/****************************************
 ****************************************
 ****************************************
 *
 *
 *  Functions for Helper
 *
 *
 *
 ****************************************
 ****************************************
 ***************************************/

AndeeHelper::AndeeHelper(){
    _id = -1;
}

void AndeeHelper::setId(char id){
    _id = id;
    if (watchFlag == 1) {
        mTextAndee2(_link, _id, mCommand('W','I','D'), "");
    } else {
        mTextAndee2(_link, _id, mCommand('S','I','D'), "");
    }
    
    //delayMicroseconds(500);
    //delay(20);
}

/* void AndeeHelper::simpleDataBox(char id, char* title, char* data)
{
	_id = id;
	char titleData[130];
	sprintf(titleData, "%s %s", title, data);
	mTextAndee2(_link, _id, mCommand('S','D','B'), titleData);	
}
void AndeeHelper::simpleDataBox(char id, char* title, int data)
{
	_id = id;
	char titleData[130];
	sprintf(titleData, "%s %d", title, data);
	mTextAndee2(_link, _id, mCommand('S','D','B'), titleData);	
}
void AndeeHelper::simpleDataBox(char id, char* title, long data)
{
	_id = id;
	char titleData[130];
	sprintf(titleData, "%s %ld", title, data);
	mTextAndee2(_link, _id, mCommand('S','D','B'), titleData);	
}
void AndeeHelper::simpleDataBox(char id, char* title, float data)
{
	_id = id;
	char titleData[130];
	sprintf(titleData, "%s %.4f", title, data);
	mTextAndee2(_link, _id, mCommand('S','D','B'), titleData);	
}
void AndeeHelper::simpleDataBox(char id, char* title, double data)
{
	_id = id;
	char titleData[130];
	sprintf(titleData, "%s %g", title, data);
	mTextAndee2(_link, _id, mCommand('S','D','B'), titleData);	
} */



void AndeeHelper::setType(char type){
    char cmd[2];
    cmd[0] = type;
    cmd[1] = 0x00;
    
    mTextAndee2(_link, _id, mCommand('T','Y','P'), cmd);
    
    if (type == WATCH) {
        watchFlag = 1;
    }
    else {
        watchFlag = 0;
    }
}
void AndeeHelper::setRow(char row){
    char cmd[2];
    cmd[0] = 0x30 | row;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('R','O','W'), cmd);
}
void AndeeHelper::setOrder(char order){
    char cmd[2];
    cmd[0] = 0x30 | order;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('O','D','R'), cmd);
}
void AndeeHelper::setSpan(char span){
    char cmd[2];
    cmd[0] = 0x30 | span;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('S','P','N'), cmd);
}
void AndeeHelper::setLocation(char row, char order, char span){
    char cmd[4];
    
    cmd[0] = 0x30 | row;
    cmd[1] = 0x30 | order;
    cmd[2] = 0x30 | span;
    cmd[3] = 0x00;
    mTextAndee2(_link, _id, mCommand('R','O','S'), cmd);
    //delay(20);
}

//****************************************
//
//               SET TITLE
//
//****************************************

void AndeeHelper::setTitle(char* titleField){
    if (watchFlag == 1) {
        mTextAndee2(_link, _id, mCommand('W','T','L'), titleField);
    } else {
        //Serial.print("SetTitle to '");
        //Serial.print(titleField);
        //Serial.print("' = ");
        mTextAndee2(_link, _id, mCommand('T','T','L'), titleField);
    }
    delay(20); //this if for long titles
}
void AndeeHelper::setTitle(int value){
    if (watchFlag == 1) {
        _setInt(mCommand('W','T','L'), value);
    }
    //itoa(value, _intCharBuf, 10);
    //mTextAndee2(_link, _id, mCommand('T','T','L'), _intCharBuf);
    else {
        _setInt(mCommand('T','T','L'), value);
    }
}
void AndeeHelper::setTitle(long value){
    if (watchFlag == 1) {
        _setInt(mCommand('W','T','L'), value);
    }
    //ltoa(value, _longCharBuf, 10);
    //mTextAndee2(_link, _id, mCommand('T','T','L'), _longCharBuf);
    else {
        _setLong(mCommand('T','T','L'), value);
    }
}
void AndeeHelper::setTitle(float value, char numDecimalPlaces){
    //dtostrf(value, 3, numDecimalPlaces, _fBuf);
    if (watchFlag == 1) {
        _setInt(mCommand('W','T','L'), value);
    }
    else {
        _setFloat(mCommand('T','T','L'), value, numDecimalPlaces);
    }
}
void AndeeHelper::setTitle(double value, char numDecimalPlaces){
    //dtostrf(value, 3, numDecimalPlaces, _fBuf);
    if (watchFlag == 1) {
        _setInt(mCommand('W','T','L'), value);
    } else {
        _setDouble(mCommand('T','T','L'), value, numDecimalPlaces);
    }
}

//****************************************
//
//                SET DATA
//
//****************************************
void AndeeHelper::setData(char* dataField){
    mTextAndee2(_link, _id, mCommand('D','A','T'), dataField);
}
void AndeeHelper::setData(int value){
    _setInt(mCommand('D','A','T'), value);
}
void AndeeHelper::setData(long value){
    _setLong(mCommand('D','A','T'), value);
}
void AndeeHelper::setData(unsigned long value){
    _setULong(mCommand('D','A','T'), value);
}
void AndeeHelper::setData(float value){
    _setFloat(mCommand('D','A','T'), value, 3);
}
void AndeeHelper::setData(float value, char numDecimalPlaces){
    _setFloat(mCommand('D','A','T'), value, numDecimalPlaces);
}
void AndeeHelper::setData(double value, char numDecimalPlaces){
    _setDouble(mCommand('D','A','T'), value, numDecimalPlaces);
}

//****************************************
//
//              SET UNIT
//
//****************************************
void AndeeHelper::setUnit(char* unitField){
    mTextAndee2(_link, _id, mCommand('U','N','T'), unitField);
}
void AndeeHelper::setUnit(int value){
    _setInt(mCommand('U','N','T'), value);
}
void AndeeHelper::setUnit(long value){
    _setLong(mCommand('U','N','T'), value);
}
void AndeeHelper::setUnit(float value, char numDecimalPlaces){
    _setFloat(mCommand('U','N','T'), value, numDecimalPlaces);
}
void AndeeHelper::setUnit(double value, char numDecimalPlaces){
    _setDouble(mCommand('U','N','T'), value, numDecimalPlaces);
}



//****************************************
//
//              SET COLORS
//
//****************************************

void AndeeHelper::setColor(char* color){
    if (watchFlag == 1) {
        mTextAndee2(_link, _id, mCommand('W','C','L'), color);
    } else {
        mTextAndee2(_link, _id, mCommand('D','B','C'), color);
    }
}
void AndeeHelper::setTitleColor(char* color){
    if (watchFlag == 1) {
        mTextAndee2(_link, _id, mCommand('W','T','C'), color);
    } else {
        mTextAndee2(_link, _id, mCommand('T','B','C'), color);
    }
}

void AndeeHelper::setTextColor(char color){
    char cmd[2];
    cmd[0] = color;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('D','T','C'), cmd);
}
void AndeeHelper::setTextColor(char* color){
    mTextAndee2(_link, _id, mCommand('D','T','C'), color);
}
void AndeeHelper::setTitleTextColor(char color){
    char cmd[2];
    cmd[0] = color;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('T','T','C'), cmd);
}
void AndeeHelper::setTitleTextColor(char* color){
    mTextAndee2(_link, _id, mCommand('T','T','C'), color);
}
void AndeeHelper::setSliderColor(char* color){
    mTextAndee2(_link, _id, mCommand('T','B','C'), color);
}
//****************************************
//
//              UPDATE DATA
//
//****************************************
void AndeeHelper::update(){
    // Serial.println("<A>");
    //   Serial.print("Update = ");
    //   Serial.println();
	delay(10);
    mTextAndee2(_link, _id, mCommand('U','P','D'), "");
	delay(2);
    //Serial.println("<B>");
	//Serial.print("_id: ");Serial.println(_id,HEX);
}
void AndeeHelper::update2(){
    // Serial.println("<A>");
    //   Serial.print("Update = ");
    //   Serial.println();
    Serial.print("Update Cmd  ");
    mTextAndee2(_link, _id, mCommand('U','P','D'), "");
    //Serial.println("<B>");
}


void AndeeHelper::updateData(char* text){
    mTextAndee2(_link, _id, mCommand('U','P','V'), text);
    //textAndee("_UPV", text);
}
void AndeeHelper::updateData(int value){
    _setInt(mCommand('U','P','V'), value);
}
void AndeeHelper::updateData(long value){
    _setLong(mCommand('U','P','V'), value);
}
void AndeeHelper::updateData(unsigned long value){
    _setULong(mCommand('U','P','V'), value);
}
void AndeeHelper::updateData(float value, char numDecimalPlaces){
    _setFloat(mCommand('U','P','V'), value, numDecimalPlaces);
}
void AndeeHelper::updateData(double value, char numDecimalPlaces){
    _setDouble(mCommand('U','P','V'), value, numDecimalPlaces);
}


//****************************************
//
//       BUTTON / KEYBOARD FN
//
//****************************************


bool AndeeHelper::isPressed(){
    int timeout = 10; //milliseconds * 5
    int i;
    
    //Serial.println("");
    
    mTextAndeeNoVerify(_link, _id, mCommand('I','P','S'), "", NONE);
    
    memset(_incomingBuffer, 0x00, INCOMING_BUF_SIZE);
    //delay(1);
    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
    for(i=0; i<timeout; i++){
        
        if(mGetLongMessagesFromAndee(_intCharBuf)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                if(strcmp(_intCharBuf, "TRUE") == 0){
                    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                    //iaFunc[0]();
                    return true;
                }
                else{
                    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                    return false;
                }
            }
        }
        else{
            
        }
    }
    _incomingBuffer[0] = 0x00;
    //Serial.println("is pressed timeout");
    return false;
}

bool AndeeHelper::isPressed2(){
    for(int x = 0;x<50;x++){
        int pos = (x*2)+2;
        if(_id==_buttonBuffer[pos]-32){
            if((_buttonBuffer[pos+1]-32)>0){
                //    Serial.print("Press counter = ");
                //    Serial.println((int)(_buttonBuffer[pos+1]-32));
                //Serial.println((char)x);
                return true;
            }
            else{
            }
        }
    }
    return false;
}
int AndeeHelper::pressCounter(){
    for(int x = 0;x<50;x++){
        int pos = (x*2)+2;
        if(_id==_buttonBuffer[pos]-32){
            if((_buttonBuffer[pos+1]-32)>0){
                
                return (int)(_buttonBuffer[pos+1]-32);
            }
            else{
            }
        }
    }
    return 0;
}

int AndeeHelper::isPressed(int timeout){
    //int timeout = 10; //milliseconds * 5
    int i;
    
    //Serial.println("");
    
    mTextAndeeNoVerify(_link, _id, mCommand('I','P','S'), "", NONE);
    
    memset(_incomingBuffer, 0x00, INCOMING_BUF_SIZE);
    //delay(1);
    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
    for(i=0; i<timeout; i++){
        
        if(mGetLongMessagesFromAndee(_intCharBuf)){
            //Serial.print("isPressed fn:");
            //Serial.println(_incomingBuffer);
            if(_isCorrectId(_incomingBuffer)){
                
                if(strcmp(_intCharBuf, "TRUE") == 0){
                    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                    return 1;
                }
                else{
                    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
                    return 0;
                }
            }
        }
        else{
        }
    }
    
    _incomingBuffer[0] = 0x00;
    //Serial.println("is pressed timeout");
    return -1;
}

int AndeeHelper::getButtonPressCount(){
    int timeout = 10*5; //milliseconds * 5
    int i;
    int count;
    count = -1;
    mTextAndeeNoVerify(_link, _id, mCommand('I','P','N'), "", NONE);
    
    memset(_intCharBuf, 0x00, INT_CHAR_BUF_SIZE);
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_intCharBuf)){
            
            if(_isCorrectId(_incomingBuffer)){
                count = atoi(_intCharBuf);
                
                return count;
            }
        }
        else{
        }
    }
    return count;
}


void AndeeHelper::requireAck(bool b){
    char cmd[2];
    
    if(b){
        cmd[0] = '0';
    }
    else{
        cmd[0] = '1';
    }
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('R','A','K'), cmd);
}
void AndeeHelper::ack(){
    mTextAndee2(_link, _id, mCommand('A','C','K'), "");
}

void AndeeHelper::getKeyboardMessage(char* buffer){
    int timeout = 10*5; //milliseconds * 5
    int i;
    int len;
    mTextAndeeNoVerify(_link, _id, mCommand('K','B','M'), "", NONE);
    //delay(10);
    buffer[0]=0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(buffer)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                return;
            }
            else{
                
                buffer[0] = 0x00;
            }
        }
    }
    //return false;
}
void AndeeHelper::setKeyboardType(char keyboardType){
    char cmd[2];
    cmd[0] = keyboardType;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('K','Y','T'), cmd);
}

void AndeeHelper::setHintText(char* text){
    mTextAndee2(_link, _id, mCommand('H','T','X'), text);
}


void AndeeHelper::remove(){
    mTextAndee2(_link, _id, mCommand('R','M','V'), "");
}


//*****************************************
//
//                SLIDERS
//
//*****************************************

void AndeeHelper::setSliderMinMax(float min, float max, char numDecimalPlaces){
    char len;
    if(numDecimalPlaces < 1)
        numDecimalPlaces = 1;
#if defined (__arm__)
    char bufTmp[5];
    bufTmp[0] = '%';
    bufTmp[1] = '.';
    bufTmp[2] = 0x30 | numDecimalPlaces;
    bufTmp[3] = 'f';
    bufTmp[4] = 0x00;
    sprintf(_incomingBuffer, bufTmp, min);
#else
    dtostrf(min, 3, numDecimalPlaces, _fBuf);
#endif
    len = strlen(_fBuf);
    _fBuf[len] = ',';
    
#if defined (__arm__)
    sprintf(_fBuf + len + 1, bufTmp, max);
#else
    dtostrf(max, 3, numDecimalPlaces, _fBuf + len + 1);
#endif
    
    mTextAndee2(_link, _id, mCommand('S','M','M'), _fBuf);
}

void AndeeHelper::setSliderMinMax(int min, int max){
    char len;
    char mBuf[14]; //catered for unsigned long
    //sprintf(_incomingBuffer, "%i,%i", min, max);
    itoa(min, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(max, mBuf+len+1, 10);
    mTextAndee2(_link, _id, mCommand('S','M','M'), mBuf);
    //memset(_incomingBuffer, 64, 0x00);
}
void AndeeHelper::setSliderMinMax(long min, long max){
    char len;
    char mBuf[22]; //catered for unsigned long
    //sprintf(_incomingBuffer, "%i,%i", min, max);
    ltoa(min, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    ltoa(max, mBuf+len+1, 10);
    mTextAndee2(_link, _id, mCommand('S','M','M'), mBuf);
    //memset(_incomingBuffer, 64, 0x00);
}

void AndeeHelper::setSliderInitialValue(float value, char numDecimalPlaces){
    
    if(numDecimalPlaces < 1)
        numDecimalPlaces = 1;
#if defined (__arm__)
    char bufTmp[5];
    bufTmp[0] = '%';
    bufTmp[1] = '.';
    bufTmp[2] = 0x30 | numDecimalPlaces;
    bufTmp[3] = 'f';
    bufTmp[4] = 0x00;
    sprintf(_fBuf, bufTmp, value);
#else
    dtostrf(value, 3, numDecimalPlaces, _fBuf);
#endif
    mTextAndee2(_link, _id, mCommand('S','L','D'), _fBuf);
    //memset(_incomingBuffer, 64, 0x00);
}

void AndeeHelper::setSliderInitialValue(int value){
    char mBuf[7];
    itoa(value, mBuf, 10);
    mTextAndee2(_link, _id, mCommand('S','L','D'), mBuf);
}

void AndeeHelper::setSliderNumIntervals(int numInterval){
    char mBuf[7];
    itoa(numInterval, mBuf, 10);
    //sprintf(_incomingBuffer, "%i", numInterval);
    mTextAndee2(_link, _id, mCommand('S','I','N'), mBuf);
}
void AndeeHelper::moveSliderToValue(int value){
    char mBuf[7];
    itoa(value, mBuf, 10);
    //sprintf(_incomingBuffer, "%i", value);
    mTextAndee2(_link, _id, mCommand('S','M','V'), mBuf);
}

void AndeeHelper::moveSliderToValue(float value, char numDecimalPlaces){
    if(numDecimalPlaces < 1)
        numDecimalPlaces = 1;
#if defined (__arm__)
    char bufTmp[5];
    bufTmp[0] = '%';
	bufTmp[1] = '.';
    bufTmp[2] = 0x30 | numDecimalPlaces;	
    bufTmp[3] = 'f';
    bufTmp[4] = 0x00;
    sprintf(_fBuf, bufTmp, value);
#else
    dtostrf(value, 3, numDecimalPlaces, _fBuf);
#endif
    mTextAndee2(_link, _id, mCommand('S','M','V'), _fBuf);
}

void AndeeHelper::setSliderReportMode(char mode){
    char cmd[2];
    cmd[0] = mode;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('S','R','M'), cmd);
}

int AndeeHelper::getSliderValue(int f){
    int timeout = 10*10; //milliseconds * 5
    int i;
    int value;
    int id;
    delay(10);
    mTextAndeeNoVerify(_link, _id, mCommand('S','G','V'), "", NONE);
    delay(2);
    memset(_fBuf, 0x00, FBUF_SIZE);
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_fBuf)){
            if(_isCorrectId(_incomingBuffer))
			{
				value = atoi(_fBuf);
                return value;
            }
            else{
                _fBuf[0] = 0x00;
                return errorReturn;
            }
        }        
    }
    Serial.println("GSV timeout");
    //Serial.println(incomingBuffer);
    memset(_incomingBuffer, INCOMING_BUF_SIZE, 0x00);
    //return false;        
}

float AndeeHelper::getSliderValue(float f){
    int timeout = 10*10; //milliseconds * 5
    int i;
    float value;
    int id;
	delay(10);
    mTextAndeeNoVerify(_link, _id, mCommand('S','G','V'), "", NONE);
    delay(2);
    memset(_fBuf, 0x00, FBUF_SIZE);
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_fBuf)){
            //I should verify ID of message
            if(_isCorrectId(_incomingBuffer)){
                value = atof(_fBuf);
                return value;
            }
            else{
                //Serial.print("id no match:");
                //Serial.println(incomingBuffer);
				_fBuf[0] = 0x00;
                return errorReturn;
            }
        }
    }
    Serial.println("GSV timeout");
    //Serial.println(incomingBuffer);
    memset(_incomingBuffer, INCOMING_BUF_SIZE, 0x00);
    //return false;	
}

//*****************************************
//
//         DATE AND TIME INPUT
//
//*****************************************

void AndeeHelper::setDefaultTime(int hr, int min, int sec){
    char buf[7];
    sprintf(buf, "%02i%02i%02i", hr%24, min%60, sec%60);
    
    mTextAndee2(_link, _id, mCommand('T','M','E'), buf);
}

void AndeeHelper::setDefaultDate(int date, int mth, int year){
    char buf[9];
    
    sprintf(buf, "%02i%02i%04i", date%31, mth%12, year);
    
    mTextAndee2(_link, _id, mCommand('D','T','E'), buf);
    //textAndee("_DTE", buf);
}
void AndeeHelper::getTimeInput(char* buffer, char* format){
    int timeout = 10*5; //milliseconds * 5
    int i;
    
    mTextAndeeNoVerify(_link, _id, mCommand('T','M','G'), format, NONE);
    delay(10);
    buffer[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(buffer)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                return;
            }
            else{
                
            }
        }
    }
    //return false;
}
void AndeeHelper::getTimeInput(int* h, int* m, int* s){
    int timeout = 10*5; //milliseconds * 5
    int i;
    
    mTextAndeeNoVerify(_link, _id, mCommand('T','M','G'), "hhmmss", NONE);
    delay(1);
    memset(_fBuf, 0x00, FBUF_SIZE);
    //_incomingBuffer[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_fBuf)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                *s = atoi(_fBuf+4);
                _fBuf[4] = 0x00;
                *m = atoi(_fBuf+2);
                _fBuf[2] = 0x00;
                *h = atoi(_fBuf);
                return;
            }
            return;
        }
    }
    //Serial.println("timeout");
    //return false;
}
void AndeeHelper::getDateInput(char* buffer, char* format){
    int timeout = 10*5; //milliseconds * 5
    int i;
    
    mTextAndeeNoVerify(_link, _id, mCommand('D','M','G'), format, NONE);
    delay(1);
    buffer[0] = 0x00;
    _incomingBuffer[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(buffer)){
            if(_isCorrectId(_incomingBuffer)){
                return;
            }
            
        }
    }
    //return false;
}

void AndeeHelper::getDateInput(int* d, int* m, int* y){
    int timeout = 10*5; //milliseconds * 5
    int i;
    
    mTextAndeeNoVerify(_link, _id, mCommand('D','M','G'), "DDMMYYYY", NONE);
    delay(1);
    memset(_fBuf, 0x00, FBUF_SIZE);
    _incomingBuffer[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_fBuf)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                *y = atoi(_fBuf+4);
                _fBuf[4] = 0x00;
                *m = atoi(_fBuf+2);
                _fBuf[2] = 0x00;
                *d = atoi(_fBuf);
                
                return;
            }
            else{
                
                return;
            }
        }
        else{
            
        }
    }
    
    //return false;
}

//*****************************************
//
//         Camera Functions
//
//*****************************************
void AndeeHelper::setCamera(char type){
    char cmd[2];
    cmd[0] = type;
    cmd[1] = 0x00;
    mTextAndee2(_link, _id, mCommand('P','S','C'), cmd);
}
void AndeeHelper::setAutoFocus(char type){
    char cmd[2];
    if(type == ON){
        cmd[0] = '0';
    }
    else{
        cmd[0] = '1';
    }
    cmd[1] = 0x00;
    
    mTextAndee2(_link, _id, mCommand('P','S','A'), cmd);
}
void AndeeHelper::setFlash(char flashType){
    char cmd[2];
    cmd[0] = flashType;
    cmd[1] = 0x00;
    
    mTextAndee2(_link, _id, mCommand('P','S','F'), cmd);
}
void AndeeHelper::setPhotoFilename(char* text){
    
    mTextAndee2(_link, _id, mCommand('P','F','N'), text);
}
void AndeeHelper::takePhoto(){
    //textAndee("_PTK", NULL);
    mTextAndee2(_link, _id, mCommand('P','T','K'), "");
}


void AndeeHelper::setSaveLocation(char loc){
    char cmd[2];
    cmd[0] = loc;
    cmd[1] = 0x00;
    //textAndee("_PSL", cmd);
    mTextAndee2(_link, _id, mCommand('P','S','L'), cmd);
}
void AndeeHelper::setFileOverwrite(char type){
    char cmd[2];
    if(type == ON)
        cmd[0] = '1';
    else
        cmd[0] = '0';
    cmd[1] = 0x00;
    //textAndee("_PFO", cmd);
    mTextAndee2(_link, _id, mCommand('P','F','O'), cmd);
}

bool AndeeHelper::takePhotoResultReady(){
    int timeout = 10*5; //milliseconds * 5
    int i;
    
    //textAndee("_PSR", "");
    mTextAndeeNoVerify(_link, _id, mCommand('P','S','R'), "", NONE);
    
    for(i=0; i<timeout; i++){
        if(mGetReply()){
            if(_isCorrectId(_incomingBuffer)){
                if(strcmp(_incomingBuffer + 6, "T") == 0){
                    return true;
                }
                else{
                    return false;
                }
            }
        }
        else{
            //Serial.println("no message yet");
        }
        
    }
    
    //Serial.println("isPressed timeout");
    return false;
}
char AndeeHelper::getLastTakePhotoResult(){
    int timeout = 10*5; //milliseconds * 5
    int i;
    char result;
    
    mTextAndeeNoVerify(_link, _id, mCommand('P','G','S'), "", NONE);
    
    
    for(i=0; i<timeout; i++){
        if(mGetReply()){
            if(_isCorrectId(_incomingBuffer)){
                result = _incomingBuffer[6];
                
                switch(result){
                    case '0': return 0;
                    case '1': return -1;
                    case '2': return -2;
                    case '3': return -3;
                    case '4': return -4;
                    case '5': return 0;
                    case '6': return -5;
                }
                
                return -6;
                
            }
        }
        else{
            
        }
    }
    
    return -7;
}

//*****************************************************
//
//      Gyro Functions
//
//*****************************************************

void AndeeHelper::GYRO_1_INIT()
{
    mTextAndeeNoVerify(_link, _id, mCommand('G','R','1'), "", NONE);
}

void AndeeHelper::gyroInit(int interval, int iteration)
{
    char len;
    char mBuf[14];
    itoa(interval, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(iteration, mBuf+len+1, 10);
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('G','R','2'), mBuf, NONE);
	delay(10);
}

void AndeeHelper::gyroStop()
{
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('G','R','S'), "", NONE);
	delay(10);
}

/* void AndeeHelper::getGyroReading(char* buffer){
    int timeout = 40*5; //milliseconds * 5
    int i;
    
    mTextAndeeNoVerify(_link, _id, mCommand('G','G','R'), "", NONE);
    delay(10);
    buffer[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(buffer)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                return;
            }
            else{
                
            }
        }
    }
    //return false;
} */

void AndeeHelper::getGyroReading(float* x, float* y, float* z){
    int timeout = 10*5; //milliseconds * 5
    int i;
	char* pEnd;	
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('G','G','R'), "", NONE);
    delay(10);
    _longCharBuf[0] = 0x00;	
	
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_longCharBuf)){
            //Serial.print("Original Buffer:");
			//Serial.println(_longCharBuf);
						
            if(_isCorrectId(_incomingBuffer)){
				
				*x = strtod (_longCharBuf, &pEnd);
				*y = strtod (pEnd, &pEnd);
				*z = strtod (pEnd, NULL);				
			   
            } 
            
        }
    }
    //return false;
}


//*****************************************************
//
//      Gravity,Accelerometer,GPS Functions
//
//*****************************************************

void AndeeHelper::ORN_1_INIT()
{
    mTextAndeeNoVerify(_link, _id, mCommand('O','R','1'), "", NONE);
    delay(1);
}

void AndeeHelper::orientInit(int interval, int iteration)
{
    char len;
    char mBuf[14];
    itoa(interval, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(iteration, mBuf+len+1, 10);
    mTextAndeeNoVerify(0, 99, mCommand('O','R','2'), mBuf, NONE);
    delay(1);
}

void AndeeHelper::orientStop()
{
    mTextAndeeNoVerify(0, 99, mCommand('O','R','S'), "", NONE);
    delay(1);
}


void AndeeHelper::LAC_1_INIT()
{
    mTextAndeeNoVerify(_link, _id, mCommand('L','A','1'), "", NONE);
    delay(1);
}

void AndeeHelper::lacInit(int interval, int iteration)
{
    char len;
    char mBuf[14];
    itoa(interval, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(iteration, mBuf+len+1, 10);
    mTextAndeeNoVerify(_link, _id, mCommand('L','A','2'), mBuf, NONE);
    delay(1);
}

void AndeeHelper::lacStop()
{
    mTextAndeeNoVerify(_link, _id, mCommand('L','A','S'), "", NONE);
    delay(1);
}


void AndeeHelper::ACC_1_INIT()
{
    mTextAndeeNoVerify(_link, _id, mCommand('A','C','1'), "", NONE);
}

void AndeeHelper::accInit(int interval, int iteration)
{
    char len;
    char mBuf[14];
    itoa(interval, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(iteration, mBuf+len+1, 10);
    mTextAndeeNoVerify(_link, _id, mCommand('A','C','2'), mBuf, NONE);
    delay(1);
}

void AndeeHelper::accStop()
{
    mTextAndeeNoVerify(_link, _id, mCommand('A','C','S'), "", NONE);
    delay(1);
}


void AndeeHelper::GRV_1_INIT()
{
    mTextAndeeNoVerify(_link, _id, mCommand('G','V','1'), "", NONE);
}

void AndeeHelper::gravInit(int interval, int iteration)
{
    char len;
    char mBuf[14];
    itoa(interval, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(iteration, mBuf+len+1, 10);
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('G','V','2'), mBuf, NONE);
    //mTextAndee2(_link, _id, mCommand('G','V','2'), mBuf);
    delay(10);
}
/*
 void AndeeHelper::GRV_2_INIT()
 {
	mTextAndeeNoVerify(0, 99, mCommand('G','V','2'), "", NONE);
 }
 */
void AndeeHelper::gravStop()
{
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('G','V','S'), "", NONE);
    delay(10);
}

void AndeeHelper::PRX_1_INIT()
{
    mTextAndeeNoVerify(_link, _id, mCommand('P','X','1'), "", NONE);
}

void AndeeHelper::proxInit(int interval, int iteration)
{
    char len;
    char mBuf[14];
    itoa(interval, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(iteration, mBuf+len+1, 10);
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('P','X','2'), mBuf, NONE);
    delay(10);
}

void AndeeHelper::proxStop()
{
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('P','X','S'), "", NONE);
    delay(10);
}

void AndeeHelper::GPS_1_INIT()
{
    mTextAndeeNoVerify(_link, _id, mCommand('G','P','1'), "", NONE);
}

void AndeeHelper::gpsInit(int interval, int iteration)
{
    char len;
    char mBuf[14];
    itoa(interval, mBuf, 10);
    len = strlen(mBuf);
    mBuf[len] = ',';
    itoa(iteration, mBuf+len+1, 10);
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('G','P','2'), mBuf, NONE);
    delay(10);
}

void AndeeHelper::gpsStop()
{
	delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('G','P','S'), "", NONE);
    delay(10);
}

void AndeeHelper::getAccReading(float* x, float* y, float* z){
    int timeout = 10*5; //milliseconds * 5
    int i;
	char* pEnd;
    delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('A','G','R'), "", NONE);
    delay(10);
    _longCharBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_longCharBuf)){   
			
			if(_isCorrectId(_incomingBuffer)){
				
				*x = strtod (_longCharBuf, &pEnd);
				*y = strtod (pEnd, &pEnd);
				*z = strtod (pEnd, NULL);				
			   
            } 
        }
    }
    //return false;
}

void AndeeHelper::getGravReading(float* x, float* y, float* z){
    int timeout = 10*5; //milliseconds * 5
    int i;
	char* pEnd;
    delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('V','G','R'), "", NONE);
    delay(10);
    _longCharBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_longCharBuf)){
            
            if(_isCorrectId(_incomingBuffer)){
				
				*x = strtod (_longCharBuf, &pEnd);
				*y = strtod (pEnd, &pEnd);
				*z = strtod (pEnd, NULL);				
			   
            } 
        }
    }
    //return false;
}

void AndeeHelper::getGpsReading(float* x, float* y){
    int timeout = 10*5; //milliseconds * 5
    int i;
	char* pEnd;
    delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('P','G','R'), "", NONE);
    delay(10);
    _longCharBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_longCharBuf)){
            
            if(_isCorrectId(_incomingBuffer)){
				
				*x = strtod (_longCharBuf, &pEnd);
				*y = strtod (pEnd, &pEnd);
				//*z = strtod (pEnd, NULL);				
			   
            } 
        }
    }
    //return false;
}

void AndeeHelper::getLacReading(float* x, float* y, float* z){
    int timeout = 10*5; //milliseconds * 5
    int i;
	char* pEnd;
    delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('L','G','R'), "", NONE);
    delay(10);
    _longCharBuf[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(_longCharBuf)){
            
            if(_isCorrectId(_incomingBuffer)){
				
				*x = strtod (_longCharBuf, &pEnd);
				*y = strtod (pEnd, &pEnd);
				*z = strtod (pEnd, NULL);				
			   
            } 
        }
    }
    //return false;
}


void AndeeHelper::getOrientReading(char* buffer){
    int timeout = 10*5; //milliseconds * 5
    int i;
    delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('O','G','R'), "", NONE);
    delay(10);
    buffer[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(buffer)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                return;
            }
            else{
                
            }
        }
    }
    //return false;
}

void AndeeHelper::getProxReading(char* buffer){
    int timeout = 10*5; //milliseconds * 5
    int i;
    delay(5);
    mTextAndeeNoVerify(_link, _id, mCommand('X','G','R'), "", NONE);
    delay(10);
    buffer[0] = 0x00;
    for(i=0; i<timeout; i++){
        if(mGetLongMessagesFromAndee(buffer)){
            
            if(_isCorrectId(_incomingBuffer)){
                
                return;
            }
            else{
                
            }
        }
    }
    //return false;
}

//*****************************************************
//      Private Functions
//*****************************************************

bool AndeeHelper::_isCorrectId(char* address){
    char iid;
    //Serial.print("checking:");
    //Serial.println(address);
    if(strlen(address) < 6)
        return false;
    if(address[0] =='#' && address[5] =='#'){
        iid = atoi(address+3);
        if(iid == _id){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
void AndeeHelper::_setInt(char* type, int value){
    itoa(value, _intCharBuf, 10);
    mTextAndee2(_link, _id, type, _intCharBuf);
}
void AndeeHelper::_setLong(char* type, long value){
    ltoa(value, _longCharBuf, 10);
    mTextAndee2(_link, _id, type, _longCharBuf);
}
void AndeeHelper::_setULong(char* type, unsigned long value){
    ltoa((long)value, _longCharBuf, 10);
    mTextAndee2(_link, _id, type, _longCharBuf);
}
void AndeeHelper::_setFloat(char* type, float value, char numDecimalPlaces){
    // if(numDecimalPlaces < 1)
    //     numDecimalPlaces = 1;
#if defined (__arm__)
    char bufTmp[5];
    bufTmp[0] = '%';
	bufTmp[1] = '.';
    bufTmp[2] = 0x30 | numDecimalPlaces;	
    bufTmp[3] = 'f';
    bufTmp[4] = 0x00;
    sprintf(_fBuf, bufTmp, value);	
#else
    dtostrf(value, 3, numDecimalPlaces, _fBuf);
#endif
    mTextAndee2(_link, _id, type, _fBuf);
}
void AndeeHelper::_setDouble(char* type, double value, char numDecimalPlaces){
    if(numDecimalPlaces < 1)
        numDecimalPlaces = 1;
#if defined (__arm__)
    char bufTmp[5];
    bufTmp[0] = '%';
	bufTmp[1] = '.';
    bufTmp[2] = 0x30 | numDecimalPlaces;	
    bufTmp[3] = 'f';
    bufTmp[4] = 0x00;
    sprintf(_fBuf, bufTmp, value);	
#else    
	dtostrf(value, 3, numDecimalPlaces, _fBuf);
#endif
    mTextAndee2(_link, _id, type, _fBuf);
}
void AndeeHelper::setRecipient(char* numString){
    //textAndee("_TTL", numString);
    mTextAndee2(_link, _id, mCommand('T','T','L'), numString);
}
void AndeeHelper::send(){
    //textAndee("_UPD", "");
    mTextAndee2(_link, _id, mCommand('U','P','D'), "");
}
void AndeeHelper::setMessage(char* message){
    //textAndee("_DAT", message);
    mTextAndee2(_link, _id, mCommand('D','A','T'), message);
}
void AndeeHelper::setTicker(char* message){
    //textAndee("_UNT", message);
    mTextAndee2(_link, _id, mCommand('U','N','T'), message);
}
void AndeeHelper::notify(){
    //textAndee("_UPD", "");
    mTextAndee2(_link, _id, mCommand('U','P','D'), "");
}

//Only for iOS, ignored in Android
void AndeeHelper::setAccent(int type){
    //setInt("_TAC", type);
    _setInt(mCommand('T','A','C'), type);
}

void AndeeHelper::setUtteranceSpeed(float value){    
	//dtostrf(value, 3, 1, _fBuf);
	//sprintf(sprintf())
#if defined (__arm__)
	sprintf(_fBuf, "%.01f", value);
	Serial.print("utter:");Serial.println(_fBuf);
#else
	dtostrf(value, 3, 1, _fBuf);
#endif
    mTextAndee2(_link, _id, mCommand('T','U','S'), _fBuf);
	
}

void AndeeHelper::setPitch(float value){
    //setFloat("_TSP", value, 1);
    //_setFloat(mCommand('T','S','P'), value, 1);
    //dtostrf(value, 3, 1, _fBuf);
#if defined (__arm__)
	sprintf(_fBuf, "%.01f", value);
	Serial.print("pitch:");Serial.println(_fBuf);
#else
	dtostrf(value, 3, 1, _fBuf);
#endif
    mTextAndee2(_link, _id, mCommand('T','S','P'), _fBuf);
}

void AndeeHelper::setCoord(int x, int y, int w, int h){
    if(x > C_HLIMIT)
        x=C_HLIMIT;
    else if(x < C_LLIMIT)
        x = C_LLIMIT;
    
    if(y > C_HLIMIT)
        y=C_HLIMIT;
    else if(y < C_LLIMIT)
        y = C_LLIMIT;
    
    
    if(w > C_HLIMIT)
        w=C_HLIMIT;
    else if(w < C_LLIMIT)
        w = C_LLIMIT;
    
    
    if(h > C_HLIMIT)
        h=C_HLIMIT;
    else if(h < C_LLIMIT)
        h = C_LLIMIT;
    
    memset(_fBuf, 0x00, 15);
    sprintf(_fBuf, "%03i%03i%03i%03i", x,y,w,h);
    mTextAndee2(_link, _id, mCommand('S','C','R'), _fBuf);
    
}

void AndeeHelper::setCoordFraction(float X, float Y, float W, float H){
    if(X > 0.95)
        X=0.95;
    else if(X < 0.0)
        X = 0.0;
    
    if(Y > 0.95)
        Y=0.95;
    else if(Y < 0.0)
        Y = 0.0;
    
    
    if(W > 1.0)
        W=1.0;
    else if(W < 0.05)
        W = 0.05;
    
    
    if(H > 1.0)
        H=1.0;
    else if(H < 0.05)
        H = 0.05;
    
    int x = X*(float)SCREEN_WIDTH;
    int w = W*(float)SCREEN_WIDTH;
    int y = Y*(float)SCREEN_HEIGHT;
    int h = H*(float)SCREEN_HEIGHT;
    
    memset(_fBuf, 0x00, 15);
    sprintf(_fBuf, "%03i%03i%03i%03i", x,y,w,h);
    Serial.println(_fBuf);
    mTextAndee2(_link, _id, mCommand('S','C','R'), _fBuf);
    
}

//*****************************************
//
//         Apple Watch Functions
//
//*****************************************



///////////////Set Watch UI Title
void AndeeHelper::setWatchTitle(char* titleField){
    mTextAndee2(_link, _id, mCommand('W','T','L'), titleField);
    delay(20); //this if for long titles
}
void AndeeHelper::setWatchTitle(int value){
    _setInt(mCommand('W','T','L'), value);
}
void AndeeHelper::setWatchTitle(long value){
    _setLong(mCommand('W','T','L'), value);
}
void AndeeHelper::setWatchTitle(float value, char numDecimalPlaces){
    _setFloat(mCommand('W','T','L'), value, numDecimalPlaces);
}
void AndeeHelper::setWatchTitle(double value, char numDecimalPlaces){
    _setDouble(mCommand('W','T','L'), value, numDecimalPlaces);
}	
///////////////Set Watch Colors
void AndeeHelper::setWatchColor(char* color){
    mTextAndee2(_link, _id, mCommand('W','C','L'), color);
}
void AndeeHelper::setWatchTitleColor(char* color){
    mTextAndee2(_link, _id, mCommand('W','T','C'), color);
}






