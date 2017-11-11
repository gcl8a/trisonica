//
//  trisonica.h
//  
//
//  Created by Gregory C Lewin on 8/6/17.
//
//

#ifndef trisonica_h
#define trisonica_h

//utility function for grabbing data; assumes no labels; annoyingly, the data can have either
//one or two spaces, depending on a '-' sign. I think it's fixed width, but I'm not sure there
//is that guarantee, so I'll just step through the string, dropping spaces as needed.
//There may be a space at the beginning, as well...sigh
String GetSubstring(const String& str, int datum)
{
    int charIndex = 0;
    int nextSpace = 0;
    
    for(int i = 0; i <= datum; i++)
    {
        //update nextSpace
        charIndex = nextSpace;
        
        //nibble away white space
        while(str[charIndex] == ' ') charIndex++;
        
        //now find the next white space
        nextSpace = str.indexOf(' ', charIndex);
        if(nextSpace == -1) return String("");
    }
    
    return str.substring(charIndex, nextSpace);
}

class TrisonicaReading
{
protected:
    float speed, direction;
    float U,V,W;
    float temperature = 0;
    
    uint32_t timestamp = 0; //result of millis() -- not "true" timestamp
    
public:
    TrisonicaReading(uint32_t ts = 0) : timestamp(ts)
    {}
    
    uint8_t Parse(const String& str)
    {
        Serial.println(str);
        
        speed = GetSubstring(str, 0).toFloat();
        direction = GetSubstring(str, 1).toFloat();
        U = GetSubstring(str, 2).toFloat();
        V = GetSubstring(str, 3).toFloat();
        W = GetSubstring(str, 4).toFloat();
        temperature = GetSubstring(str, 5).toFloat();
        
        return 1;
    }
    
    String MakeDataString(void)
    {
        char dataStr[100];
        
        sprintf(dataStr, "%lu,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f,%2.2f",
                timestamp,
                speed,
                direction,
                U,
                V,
                W,
                temperature);
        
        return String(dataStr);
    }
    
};

class Trisonica
{
protected:
    String triString;
    HardwareSerial* serial;
    
    TrisonicaReading workingDatum;
    
public:
    Trisonica(HardwareSerial* ser) : serial(ser) {}
    
    TrisonicaReading GetReading(void) {return workingDatum;}
    
    int CheckSerial(void)
    {
        int retVal = 0;
        while(serial->available())
        {
            char c = serial->read();
            if(c != '\n' && c != '\r') triString += c;  //ignore carriage return and newline
            if(c == '\n') //we have a complete string
            {
                TrisonicaReading newReading(millis());
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
};


#endif /* trisonica_h */
