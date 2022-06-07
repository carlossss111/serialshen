#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "Volume.h"

#define charToInt(num) static_cast<int>(num - 48)

//set the volume by providing a number between 0-100
void Volume::setVolume(int app, int vol){
    if(0 > vol || vol > 100)
        return;
    if(!app)
        return;

    char sysStr[42];
    sprintf(sysStr, "pactl set-sink-input-volume %d %d%%", app, vol);
    system(sysStr);
}

//return the "sink number" of an applicatio name, or 0 if not found
int Volume::getApplicationNum(const char* appName) const{
    FILE *fp; char word[256]; char *formattedName;
    int sinkNum;

    //format the name to match
    formattedName = (char *) malloc(sizeof(char) * strlen(appName) + 2 + 1);
    sprintf(formattedName, "\"%s\"", appName);

    //run bash command and return output as a file
    fp = popen("pactl list sink-inputs", "r");

    //loop through word by word
    while (fscanf(fp,"%s", word) > 0){

        //find "Sink Input #n" and record n
        if(strstr("Sink", word)){
            fscanf(fp,"%s", word);
            if(strstr("Input", word)){
                fscanf(fp,"%s", word);
                sinkNum = charToInt(word[1]);
            }
        }

        //find "name", if found return the sink number
        if(strstr(formattedName, word)){
            pclose(fp);
            free(formattedName);
            return sinkNum;
        }
    }

    //The application sink number could not be found, return 0
    pclose(fp);
    free(formattedName);
    return 0;
}