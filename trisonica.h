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
    float speed, direction;
    float U,V,W;
    float temperature = 0;
    
    uint32_t timestamp = 0; //result of millis() -- not "true" timestamp
    
public:
    TrisonicaDatum(uint32_t ts = 0) : timestamp(ts)
    {}
    
    uint8_t Parse(const String& str)
    {
        speed = GetTriSonicaSubstring(str, 0).toFloat();
        direction = GetTriSonicaSubstring(str, 1).toFloat();
        U = GetTriSonicaSubstring(str, 2).toFloat();
        V = GetTriSonicaSubstring(str, 3).toFloat();
        W = GetTriSonicaSubstring(str, 4).toFloat();
        temperature = GetTriSonicaSubstring(str, 5).toFloat();
        
        return 1;
    }
    
    String MakeDataString(void)
    {
        char dataStr[100];
        
        sprintf(dataStr, "%lu,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f",
                timestamp%1000,
                speed,
                direction,
                U,
                V,
                W,
                temperature);
        
        return String(dataStr);
    }
    
    String MakeShortDataString(void)
    {
        char dataStr[100];
        
        sprintf(dataStr, "%2.2f,%2.2f,%2.2f",
                speed,
                direction,
                temperature);
        
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
                retVal = newReading.Parse(triString); //parse it; retVal holds its type
                if(retVal)
                {
                    workingDatum = newReading;
                }
                
                triString = "";
            }
        }

        return retVal;
    }
    
    uint8_t CheckSerial(TrisonicaDatum* datum)
    {
        uint8_t retVal = 0;
        while(serial->available())
        {
            char c = serial->read();
            if(c != '\n' && c != '\r') triString += c;  //ignore carriage return and newline
            if(c == '\n') //we have a complete string
            {
                TrisonicaDatum newReading(millis());
                retVal = newReading.Parse(triString); //parse it; retVal holds its type
                if(retVal)
                {
                    *datum = newReading;
                }
                
                triString = "";
                return retVal; //return here so we don't start on the next string
            }
        }
        
        return retVal;
    }
    
    int CheckSerialNoParse(void)
    {
        while(serial->available())
        {
            char c = serial->read();
            if(c != '\n' && c != '\r') triString += c;  //ignore carriage return and newline
            if(c == '\n') //we have a complete string
            {
                return true;
            }
        }
        
        return false;
    }
};


#endif /* trisonica_h */
