#ifndef BYTEARRAY_H
#define BYTEARRAY_H

//#include "stdcanalog.h"

class ByteArray{

public:
  ByteArray(const char* data, int len);
  ByteArray(const char* data);
  ByteArray(const ByteArray& src);
  bool operator==(const ByteArray& other) const;
  ByteArray(int len);
  ByteArray();

  inline char& operator[](const int pos){ //Перекрываем доступ по индексу массива []
    return _data[pos];
  }
  inline const char& operator[](const int pos) const{ //Перекрываем доступ по индексу массива []
    return _data[pos];
  } 
  inline char* data(){return _data;}
  inline const char* cdata() const {return _data;}
  
  inline int length() const{
    return _len;
  }


  ~ByteArray();
  
private:
  char* _data;
  int _len;
  void copyLen(const char* from, char* to,const int len);
  void fillLen(char smb, char* to,const int len);
  
};
#endif
