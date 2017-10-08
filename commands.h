#ifndef COMMANDS_H
#define COMMANDS_H


#include <inttypes.h>
#include "bytearray.h"

enum Commands{
    UPDOWN,
    MOVETO,
    RESET,
    STATUS,
    CONFIG,// Домашнее задание написать STATUS
    COMMANDS_SIZE
};



class IncomingCommand {
  public:
  virtual Commands getType()=0;
  virtual int getDataLen()=0;
  virtual ~IncomingCommand(){};
};

IncomingCommand* commandFactory(const Commands type, const ByteArray& data);

/*class CommandDescriptor{

    CommandDescriptor(Commands _type, int _dataLen):
    type(_type),dataLen(_dataLen){
    }
  public:
    inline Commands geType(){return type;}
    inline int getDataLen(){return dataLen;}
  private:
    Commands type;
    int dataLen;
  public:
    static const CommandDescriptor descriptors [];
};*/

class OutgoingCommand {
  public:
  virtual Commands getType()=0;
  virtual ByteArray serialize()=0;
  virtual ~OutgoingCommand(){};
};

class UpDown: public IncomingCommand
{
public:
    UpDown(const ByteArray& data);
    char getUpDown(){return upDown;}
    Commands getType(){return UPDOWN;}
    int getDataLen(){return 2;}
private:
    char upDown;
};

class Reset: public IncomingCommand
{
public:
    Reset(const ByteArray& data);
    Commands getType(){return RESET;}
    int getDataLen(){return 1;}
};

class MoveTo: public IncomingCommand
{
public:
    MoveTo(const ByteArray& data);
    Commands getType(){return MOVETO;}
    int getDataLen(){return 9;}
    inline int32_t getX(){return x;}
    inline int32_t getY(){return y;}
private:
    int32_t x;
    int32_t y;
};

class Status: public OutgoingCommand
{
    public:
    Status(bool success, bool up, int32_t x, int32_t y);
    ByteArray serialize();
    virtual Commands getType();
  
  protected:
    bool m_success;
    bool m_up;
    int32_t m_x;
    int32_t m_y;
    //arr
};

class Config: public Status //    config = <byte cmdtype=CONFIG> <int32 maxX><int32 maxY>
{
    public:
    Config(int32_t maxX, int32_t maxY);
    ByteArray serialize();
    virtual Commands getType();
    
    private:
    int32_t m_maxX;
    int32_t m_maxY;
};

#endif
