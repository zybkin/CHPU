#include "commands.h"


UpDown::UpDown(const ByteArray& data){
      
      upDown = data[1];
}

Reset::Reset(const ByteArray& data){
    
};

MoveTo::MoveTo(const ByteArray& data){

    x =*reinterpret_cast<const int32_t*>(data.cdata()+1); //+current - смещение на текущую позицию (пропустить success и up)
    y =*reinterpret_cast<const int32_t*>(data.cdata()+5);  

};
Status::Status(bool success, bool up, int32_t x, int32_t y){
    m_success = success;
    m_up = up;
    m_x = x;
    m_y = y;
};
ByteArray Status::serialize(){
     ByteArray data(11); // 11 - размер массива, status = <byte cmdtype=STATUS><byte результат команды><byte updown><int32 x><int32 y>.
     int current = 0;
     data[current++] = STATUS;
     data[current++] = m_success;
     data[current++] = m_up;    
          
     char* xBytes = reinterpret_cast<char*>(&m_x); //Берем адрес переменной x и преобразуем его к типу char*, а затем присваиваем его переменной xBytes
     for (int i=0; i<sizeof(int32_t);i++){
         data[current++]=xBytes[i];
     }

     char* yBytes = reinterpret_cast<char*>(&m_y);
     for (int i=0; i<sizeof(int32_t);i++){
         data[current++]=yBytes[i];
     };

     return data;
};

ByteArray Config::serialize(){
     ByteArray data(9); // 9 - размер массива, .
     int current = 0;
     data[current++] = STATUS;
          
     char* xBytes = reinterpret_cast<char*>(&m_maxX); //Берем адрес переменной x и преобразуем его к типу char*, а затем присваиваем его переменной xBytes
     for (int i=0; i<sizeof(int32_t);i++){
         data[current++]=xBytes[i];
     }

     char* yBytes = reinterpret_cast<char*>(&m_maxY);
     for (int i=0; i<sizeof(int32_t);i++){
         data[current++]=yBytes[i];
     };

     return data;
};

    Commands Status::getType(){return STATUS;}
    Commands Config::getType(){return CONFIG;}