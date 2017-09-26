#include "stdcanalog.h"


/*
int strlen(const char * str){
   int _len=0;
    while(str[_len++]);
    return len;

}*/

void strcpy(const char*from, char* to, int len){
  for(int i=0;i<len;i++){
       to[i]=from[i];
     }
}

void strfill(char*str, char symbol, int len){
   for(int i=0;i<len;i++){
       str[i]=symbol;
     }
}
