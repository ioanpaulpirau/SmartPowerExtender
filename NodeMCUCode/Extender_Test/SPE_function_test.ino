#include <ESP8266WiFi.h>

/*
Measuring Current Using ACS712
*/
const int analogIn = A0;
int mVperAmp = 66; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;

#define SOCKET1 D0
#define SOCKET2 D1
#define SOCKET3 D2
#define SOCKET4 D3

struct SPEState
{
  uint8_t socket1;
  uint8_t socket2;
  uint8_t socket3;
  uint8_t socket4;
  int acs712value;
  SPEState():socket1(0),socket2(0),socket3(0),socket4(0), acs712value(0){} 
} State;

void setupRelays()
{
 pinMode(SOCKET1, OUTPUT);
 pinMode(SOCKET2, OUTPUT);
 pinMode(SOCKET3, OUTPUT);
 pinMode(SOCKET4, OUTPUT);  

/*
 digitalWrite(SOCKET1, HIGH);
 digitalWrite(SOCKET2, HIGH);
 digitalWrite(SOCKET3, HIGH);
 digitalWrite(SOCKET4, HIGH);
 */
}

void setSocket(uint8_t relay, uint8_t value)
{
  uint8_t pin = 255;
  uint8_t val = HIGH;
  if (value == 0) val = LOW;

  Serial.print("\tChanging Socket ");
  Serial.print(relay); 
  Serial.print(" Value to : " ); 
  Serial.print(value); 

  switch(relay)
  {
    case 1: {
      digitalWrite(SOCKET1, val);    
      State.socket1 = val;
      break;
    }
    case 2: {
      digitalWrite(SOCKET2, val);    
      State.socket2 = val;
      break;
    }
    case 3: {
      digitalWrite(SOCKET3, val);    
      State.socket3 = val;
      break;
    }
    case 4: {
      digitalWrite(SOCKET4, val);    
      State.socket4 = val;
      break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);

 setupRelays();
 pinMode(A0, INPUT);
}

void test_loop()
{
 RawValue = analogRead(analogIn);
 Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
 Amps = ((Voltage - ACSoffset) / mVperAmp);
 State.acs712value = RawValue;

 Serial.print("\tRaw Value = " ); // shows pre-scaled value 
 Serial.print(RawValue); 
 Serial.print("\t mV = "); // shows the voltage measured 
 Serial.print(Voltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
 Serial.print("\t Amps = "); // shows the voltage measured 
 Serial.println(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point

  setSocket(1,0);
  Serial.println();
  delay(2500);
  setSocket(1,1);
  setSocket(2,0);
  Serial.println();
  delay(2500);
  setSocket(2,1);
  setSocket(3,0);
  Serial.println();
  delay(2500);
  setSocket(3,1);
  setSocket(4,0);
  Serial.println();
  delay(2500);
  setSocket(4,1);
  Serial.println();
 
  delay(2500);   
}

}
void loop() {
   // put your main code here, to run repeatedly:
  test_loop();
}
