//
//  trisonica.h
//  
//
//  Created by Gregory C Lewin on 8/6/17.
//
//

#ifndef trisonica_h
#define trisonica_h

#include <Arduino.h>

String GetTriSonicaSubstring(const String& str, int datum);

class TrisonicaDatum
{
protected:
    uint32_t timestamp = 0;
    
    //all stored as tenths for efficiency
    int8_t U, V, W, speed = 0;
    int16_t temperature;
    
    //degrees
    int16_t direction;
    
public:
    TrisonicaDatum(uint32_t ts = 0) : timestamp(ts) {}
    
    uint8_t ParseInputString(const String& str)
    {
        speed = GetTriSonicaSubstring(str, 0).toFloat() * 10;
        direction = GetTriSonicaSubstring(str, 1).toInt();
        U = GetTriSonicaSubstring(str, 2).toFloat() * 10;
        V = GetTriSonicaSubstring(str, 3).toFloat() * 10;
        W = GetTriSonicaSubstring(str, 4).toFloat() * 10;
        temperature = GetTriSonicaSubstring(str, 5).toFloat() * 10;
        
        return 1;
    }
    
    String MakeDataString(void)
    {
        char dataStr[100];
        
        sprintf(dataStr, "%lu,%2.1f,%i,%2.1f,%2.1f,%2.1f,%2.1f",
                timestamp,
                speed / 10.0,
                direction,
                U / 10.0,
                V / 10.0,
                W / 10.0,
                temperature / 10.0);
        
        return String(dataStr);
    }
    
    String MakeShortDataString(void)
    {
        char dataStr[100];
        
        sprintf(dataStr, "%2.1f,%i,%2.1f",
                speed / 10.0,
                direction,
                temperature / 10.0);
        
        return String(dataStr);
    }
};

class Trisonica
{
protected:
    String triString;
    HardwareSerial* serial;
    
    TrisonicaDatum workingDatum;
    
public:
    Trisonica(HardwareSerial* ser) : serial(ser) {}
    
    TrisonicaDatum GetReading(void) {return workingDatum;}
    String MakeDataString(void) {return workingDatum.MakeDataString();}

    int CheckSerial(void)
    {
        int retVal = 0;
        while(serial->available())
        {
            char c = serial->read();
            if(c != '\n' && c != '\r') triString += c;  //ignore carriage return and newline
            if(c == '\n') //we have a complete string
            {
                TrisonicaDatum newReading(millis());
                retVal = newReading.ParseInputString(triString); //parse it; retVal holds its type
                if(retVal)
                {
                    workingDatum = newReading;
                }
                
                triString = "";
            }
        }

        return retVal;
    }
};


#endif /* trisonica_h */
