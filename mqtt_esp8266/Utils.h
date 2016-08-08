#include <ESP8266WiFi.h>

#define SOCKET1 D0
#define SOCKET2 D1
#define SOCKET3 D2
#define SOCKET4 D3

/***
 * Data Model Structure
 */
struct SPEState
{
  uint8_t socket1;  //0 - off, 1 - on
  uint8_t socket2;  //0 - off, 1 - on
  uint8_t socket3;  //0 - off, 1 - on
  uint8_t socket4;  //0 - off, 1 - on
  int acs712value;  //amps drawn from socket
  
  SPEState():socket1(0),socket2(0),socket3(0),socket4(0), acs712value(0){}
  char* toString(char* inputString);  //inputString needs to be at least 12 characters long
};

/***
 * Variable definition
 */
class CSocketControl
{
  // Variable definition 
  const int analogIn = A0;
  const int mVperAmp = 66; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module
  const int ACSoffset = 2500; 
  int RawValue;
  double Voltage;
  double Amps;
  SPEState state;
    
  // Function definition 
public: 
  CSocketControl():RawValue(0), Voltage(0), Amps(0) {}
  ~CSocketControl() {}
  void setupRelays();
  void setSocket(uint8_t relay, uint8_t value);
  void setSocket(char* inputString);
  void readAmperage();
  void test_loop();
  SPEState getState() {return state;};
};
