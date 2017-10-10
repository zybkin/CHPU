// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

/* Домашнее задание
1. найти куда загружены библиотеки Ардуино    /home/pavel/arduino-1.8.3/hardware/teensy/avr/libraries
2. если получится добавить в класс MultiStepper функции move и reset, убедиться что проект собирается
3. сделать функцию getCurrentPosition(long[] arrayToFill)
*/

#include <AccelStepper.h>
#include <MultiStepper.h>
//#include "bytearray.h"
#include "commands.h"

AccelStepper stepperX(AccelStepper::FULL4WIRE,2,3,4,5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperY1(AccelStepper::FULL4WIRE,14,15,16,17); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperY2(AccelStepper::FULL4WIRE,6,7,8,9); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
MultiStepper plot;
#define OURSTEPPERS 3
#define PACKETSIZE OURSTEPPERS*4

/*buttons*/
#define TOPRIGHT (51)
#define BOTTOMRIGHT (47)
#define RIGHT (46)

#define TOPLEFT (50)
#define BOTTOMLEFT (49)
#define LEFT (48)

#define BUTTONOFFSET 46

byte buttonStates[6]={LOW,LOW,LOW,LOW,LOW,LOW}; 


void doButtons(){
  Serial.println("R  BR L  BL TL TR ");    
  for(int i = RIGHT;i<=TOPRIGHT;i++){
    buttonStates[i-BUTTONOFFSET]=digitalRead(i);
    
    Serial.print((int)buttonStates[i-BUTTONOFFSET],DEC);
    Serial.print("  ");
  }
  Serial.println("");
  
}

bool checkAllButtonsDisabled(){
  for(int i=0;i<sizeof(buttonStates);i++){
    if(buttonStates[i]!=HIGH){
      return false;
    }
  }
  return true;
}

/*steppers*/
 long moveCoord[OURSTEPPERS]={100,100,-100};
 const int lengths[3]={2,9,1};
 void prepareStepper(AccelStepper& stepper){
   stepper.setMaxSpeed(400);
   stepper.setAcceleration(100);
   stepper.setCurrentPosition(100000);
  
    
 }

 /*global states*/
enum GlobalStates{
  RESET_START,
  RESET_END,
  WAITING_INCOMING,
  ANSWERING,
  MOVING
};
OutgoingCommand* pendingCommand=NULL;

GlobalStates currentState = RESET_START;
#define DELTA (100)

static long ZeroSearchArray[]={-DELTA,-DELTA,DELTA};
static long EndSearchArray[]={DELTA,DELTA,-DELTA};

#define XSTEPPER (0)
#define Y1STEPPER (1)
#define Y2STEPPER (2)

bool checkNeedMove(long* arr){
  for(int i=0;i<3;i++){
    if(arr[i]!=0)
      return true;
  }
  return false;
}

void onResetStart(){
  if(!plot.run()){
      if(buttonStates[RIGHT-BUTTONOFFSET]==HIGH){
        ZeroSearchArray[XSTEPPER]=DELTA;
      }else{
        ZeroSearchArray[XSTEPPER]=0;
      }
      
      if(buttonStates[BOTTOMRIGHT-BUTTONOFFSET]==HIGH){
        ZeroSearchArray[Y1STEPPER]=DELTA;
      }else{
        ZeroSearchArray[Y1STEPPER]=0;
      }
      
      if(buttonStates[BOTTOMLEFT-BUTTONOFFSET]==HIGH){
        ZeroSearchArray[Y2STEPPER]=-DELTA;
      }else{
        ZeroSearchArray[Y2STEPPER]=0;
      }
      
      if(checkNeedMove(ZeroSearchArray)){
        plot.move(ZeroSearchArray);
      }else{
        // switch to ResetEnd
        currentState = RESET_END;
      }
   }else{
    plot.runSpeedToPosition();
   }
  }

void onResetEnd(){
  if(!plot.run()){
      if(buttonStates[LEFT-BUTTONOFFSET]==HIGH){
        EndSearchArray[XSTEPPER]=DELTA;
      }else{
        EndSearchArray[XSTEPPER]=0;
      }
      
      if(buttonStates[TOPRIGHT-BUTTONOFFSET]==HIGH){
        EndSearchArray[Y1STEPPER]=DELTA;
      }else{
        EndSearchArray[Y1STEPPER]=0;
      }
      
      if(buttonStates[TOPLEFT-BUTTONOFFSET]==HIGH){
        EndSearchArray[Y2STEPPER]=-DELTA;
      }else{
        ZeroSearchArray[Y2STEPPER]=0;
      }
      
      if(checkNeedMove(EndSearchArray)){
        plot.move(EndSearchArray);
      }else{
        // switch to ResetEnd
        pendingCommand = new Config(stepperX.currentPosition(),stepperY1.currentPosition());
        currentState = ANSWERING;
      }
   }else{
      plot.runSpeedToPosition();
   }
  }

void onWaitingIncoming(){
   if( Serial.available()>0){
          char next =Serial.peek(); // peek не читает , а просто проверяет оставляя на месте байт
          if(next < sizeof(lengths)){
            int len =lengths[next];
            if(len<=Serial.available()){
              ByteArray arr(len);
              Serial.readBytes(arr.data(),len);
              IncomingCommand* cmd = commandFactory((Commands)next,arr);
              switch(cmd->getType()){
                   case UPDOWN:
                      pendingCommand = new Status(true,0,stepperX.currentPosition(),stepperY1.currentPosition());
                      currentState = ANSWERING;
                   break;
                   case MOVETO:{
                        MoveTo * cmdMove = (MoveTo*)cmd;
                        moveCoord[XSTEPPER] = cmdMove->getX();
                        moveCoord[Y1STEPPER] = cmdMove->getY();
                        moveCoord[Y2STEPPER] = -cmdMove->getY();
                        plot.moveTo(moveCoord);
                        currentState = MOVING;
                   }
                   break;
                   case RESET:
                      currentState = RESET_START;
                   break;
              }
              delete cmd;
            }else{
              //ждем
            }
          }else{
            //ругань
            int incomingByte = 0;
             while (Serial.available() > 0) {  //если есть доступные данные
                 // считываем байт
                incomingByte = Serial.read();
             }
             pendingCommand = new Status(false,0,stepperX.currentPosition(),stepperY1.currentPosition());
             currentState = ANSWERING;
          }
        /*char*cMoveCoord = (char*)moveCoord;
        for(int i=0;i<PACKETSIZE;i++){
          cMoveCoord[i]=Serial.read();
        }
        Serial.print(moveCoord[0],DEC);
        Serial.print(moveCoord[1],DEC);
        Serial.println(moveCoord[2],DEC);
        */
      
      }
      else{      
      //ждем
     }
  }
void onAnswering(){
    if(pendingCommand!=NULL){
     ByteArray serialized = pendingCommand->serialize();
     Serial.write(serialized.data(),serialized.length());
     delete pendingCommand;
     pendingCommand = NULL;
    }else{
       currentState = WAITING_INCOMING;
    }
  }
void onMoving(){
   if(!plot.run()){
      pendingCommand = new Status(true,0,stepperX.currentPosition(),stepperY1.currentPosition());
      currentState = ANSWERING;
   }else{
      plot.runSpeedToPosition();
   }
}

/*setup*/ 
void setup()
{  
  prepareStepper(stepperX);
  prepareStepper(stepperY1);
  prepareStepper(stepperY2);
    
  plot.addStepper(stepperX);
  plot.addStepper(stepperY1);
  plot.addStepper(stepperY2);
  
  Serial.begin(9600);
  for(int i=RIGHT;i<=TOPLEFT;i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);  
  }

    pinMode(TOPRIGHT, INPUT_PULLUP);
    digitalWrite(TOPRIGHT, HIGH);  

  
  /*
  pinMode(TOPRIGHT, INPUT);
 // digitalWrite(TOPRIGHT, HIGH);  
  pinMode(BOTTOMRIGHT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(TOPLEFT, INPUT);
  pinMode(BOTTOMLEFT, INPUT);
  pinMode(LEFT, INPUT);
  */
  
  //plot.moveTo(moveCoord);
  //  currentState = WAITING_INCOMING;
 

}
IncomingCommand* currentCmd=0;

void loop()
{  
  //Serial.println("running");
    
    doButtons();
  
  //onResetStart();
    switch(currentState){
      case RESET_START: onResetStart(); break;
      case RESET_END: onResetEnd(); break;
      case WAITING_INCOMING: onWaitingIncoming(); break;
      case ANSWERING: onAnswering(); break;
      case MOVING: onMoving(); break;
    }

    
  /*if(!plot.run()){
      //int packetSize = 
     
       //plot.moveTo(moveCoord);
   }else{
    plot.moveTo(moveCoord);
   }*/
  
   //plot.runSpeedToPosition();
   /*   if (stepperX.distanceToGo() == 0)
        stepperX.moveTo(-stepperX.currentPosition());
   stepperX.run();*/
}
