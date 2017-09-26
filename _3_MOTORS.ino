// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>
#include <MultiStepper.h>
#include "bytearray.h"
#include "commands.h"

AccelStepper stepperX(AccelStepper::FULL4WIRE,2,3,4,5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperY1(AccelStepper::FULL4WIRE,14,15,16,17); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperY2(AccelStepper::FULL4WIRE,6,7,8,9); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
MultiStepper plot;
#define OURSTEPPERS 3
#define PACKETSIZE OURSTEPPERS*4

 long moveCoord[OURSTEPPERS]={100,100,-100};
 
 void prepareStepper(AccelStepper& stepper){
   stepper.setMaxSpeed(400);
   stepper.setAcceleration(100);
   stepper.setCurrentPosition(0);
  
    
 }
void setup()
{  
  prepareStepper(stepperX);
  prepareStepper(stepperY1);
  prepareStepper(stepperY2);
    
  plot.addStepper(stepperX);
  plot.addStepper(stepperY1);
  plot.addStepper(stepperY2);
  
  Serial.begin(115200);
   //stepper.setMinPulseWidth(20);
   /*stepperX.setMaxSpeed(700);
   stepperX.setAcceleration(200);
   stepperX.setSpeed(-300);	
   stepperX.setCurrentPosition(0);
   stepperX.moveTo(-10000);*/
   //stepperX.move(10000);
   //stepperY1.setSpeed(300);	
   //stepperY2.setSpeed(300);	
   plot.moveTo(moveCoord);
    
   ByteArray arr1;
   ByteArray arr2("111");
   arr1=arr2;
   if(arr1==arr2){
     Serial.write("yeah");
   }

}

void loop()
{  
  //Serial.println("running");
  if(!plot.run()){
      //int packetSize = 
      if( Serial.available()>PACKETSIZE){
        char*cMoveCoord = (char*)moveCoord;
        for(int i=0;i<PACKETSIZE;i++){
          cMoveCoord[i]=Serial.read();
        }
        Serial.print(moveCoord[0],DEC);
        Serial.print(moveCoord[1],DEC);
        Serial.println(moveCoord[2],DEC);
        
      
      }
        else{      
      for(int i=0;i<OURSTEPPERS;i++){
         moveCoord[i]=-moveCoord[i];
       }
     }
       plot.moveTo(moveCoord);
   }else{
   }
  
   //plot.runSpeedToPosition();
   /*   if (stepperX.distanceToGo() == 0)
        stepperX.moveTo(-stepperX.currentPosition());
   stepperX.run();*/
}
