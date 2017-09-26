#include "bytearray.h"
#include <Arduino.h>

ByteArray::ByteArray(const char* data, int len){
    _len = len;
     _data = new char[_len];
     strcpy(data,_data,_len);
     
  }

  ByteArray::ByteArray(const char* data){
    _len=strlen(data);
     _data = new char[_len];
     strcpy(data,_data,_len);
  }


  ByteArray::ByteArray(const ByteArray& src){
     _len = src._len;
     _data = new char[_len];
     strcpy(src.cdata() ,_data,_len);
  }

  bool ByteArray::operator==(const ByteArray& other) const{
    if(_len!= other._len){
      return false;
    }
    for(int i=0;i<_len;i++){
      if(_data[i]!=other._data[i]){
        return false;
      }
    }
    return true;
  }

  ByteArray::ByteArray(int len){
    _len = len;
     _data = new char[len];
     strfill(_data,0,_len);
  }
    
  ByteArray::ByteArray(){
    _len = 0;
     _data = NULL;
  }



  ByteArray::~ByteArray(){
     if(_data){
       delete[] _data;
       
     }
  }
