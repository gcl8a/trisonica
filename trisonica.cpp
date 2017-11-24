#include <trisonica.h>

//utility function for grabbing data; assumes no labels; annoyingly, the data can have either
//one or two spaces, depending on a '-' sign. I think it's fixed width, but I'm not sure there
//is that guarantee, so I'll just step through the string, dropping spaces as needed.
//There may be a space at the beginning, as well...sigh
String GetTriSonicaSubstring(const String& str, int datum)
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
