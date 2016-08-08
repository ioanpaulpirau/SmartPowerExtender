#include "Utils.h"

char* SPEState::toString(char* inputString)
{
  char converter[10];  
  if (inputString == NULL) return NULL;

  strcpy(inputString, itoa(!socket1, converter, 10));
  strcat(inputString, itoa(!socket2, converter, 10));
  strcat(inputString, itoa(!socket3, converter, 10));
  strcat(inputString, itoa(!socket4, converter, 10));
  strcat(inputString, "-");
  strcat(inputString, itoa(acs712value, converter, 10));
    
  return inputString;
}

void CSocketControl::setupRelays()
{
 pinMode(SOCKET1, OUTPUT);
 pinMode(SOCKET2, OUTPUT);
 pinMode(SOCKET3, OUTPUT);
 pinMode(SOCKET4, OUTPUT);  

 pinMode(A0, INPUT);
/*
 digitalWrite(SOCKET1, HIGH);
 digitalWrite(SOCKET2, HIGH);
 digitalWrite(SOCKET3, HIGH);
 digitalWrite(SOCKET4, HIGH);
 */
}

void CSocketControl::setSocket(uint8_t relay, uint8_t value)
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
      state.socket1 = val;
      break;
    }
    case 2: {
      digitalWrite(SOCKET2, val);    
      state.socket2 = val;
      break;
    }
    case 3: {
      digitalWrite(SOCKET3, val);    
      state.socket3 = val;
      break;
    }
    case 4: {
      digitalWrite(SOCKET4, val);    
      state.socket4 = val;
      break;
    }
  }
}

void CSocketControl::setSocket(char* inputString)
{
  if (inputString == 0) return;
  uint8_t len = strlen(inputString);
  if (len < 2 || len > 10) return;
  // interpret commands
  if (!strcmp(inputString, "alloff"))
    for (int i = 1; i < 4; i++)
      setSocket(i, 1);
    else
      if (!strcmp(inputString, "allon"))
        for (int i = 1; i < 4; i++)
          setSocket(i, 0);
      else
        if (!strcmp(inputString, "looptest"))
          test_loop();
        else
        {
          uint8_t socket = 9;
          uint8_t state = 0;
          
          socket = inputString[0] - 48;
          state = inputString[1] - 49;
        
          setSocket(socket, state);
        }
}

void CSocketControl::readAmperage()
{
  RawValue = analogRead(analogIn);
  Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
  Amps = ((Voltage - ACSoffset) / mVperAmp);

  Serial.print("\nRaw Value = " ); // shows pre-scaled value 
  Serial.print(RawValue); 
  Serial.print("\t mV = "); // shows the voltage measured 
  Serial.print(Voltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
  Serial.print("\t Amps = "); // shows the voltage measured 
  Serial.print(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
  
  state.acs712value = (int)Amps;
}

void CSocketControl::test_loop()
{
  readAmperage();

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
