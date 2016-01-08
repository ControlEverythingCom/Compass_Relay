#include "MAG3110.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"
#include "spark_wiring.h"
#include "spark_wiring_eeprom.h"
#include <cmath>

bool MAG3110::init(){
    Wire.begin();
    
    Wire.beginTransmission(address);
    Wire.write(CTRL_REG2);          
    Wire.write(0x80);            
    byte status1 = Wire.endTransmission();     
    if(status1 != 0){
        Serial.println("write config reg 2 failed");
        return false;
    }
  
    delay(15);
  
    Wire.beginTransmission(address);
    Wire.write(CTRL_REG1);        
    Wire.write(1);             
    byte status2 = Wire.endTransmission();       
    if(status2 != 0){
        Serial.println("write config reg 1 failed");
        return false;
    }
    if(!checkStoredVals()){
        return false;
    }
    return true;
}

bool MAG3110::getBaring(int buffer[]){
    int readings[3];
    int axisReadings[6];
    
    for(int i = 0; i < 6; i++){
        Wire.beginTransmission(address);
        Wire.write(i+1);
        byte status = Wire.endTransmission();
        if(status != 0){
            Serial.println("read axis's failed");
            return false;
        }
        Wire.requestFrom(address, 1);
        axisReadings[i] = Wire.read();
        delay(5);
    }
    
    readings[0] = (axisReadings[0]*256)+axisReadings[1];
    readings[1] = (axisReadings[2]*256)+axisReadings[3];
    readings[2] = (axisReadings[4]*256)+axisReadings[5];
    // buffer = readings;
    buffer[0] = readings[0];
    buffer[1] = readings[1];
    buffer[2] = readings[2];
    return true;
}

float MAG3110::getHeading(){
    int rawReadings[3];
    if(!getBaring(rawReadings)){
        return 181.00;
    }
    double xDouble = (rawReadings[1] - yOffset);
    double yDouble = (rawReadings[0] - xOffset);
    double heading = atan2(yDouble,xDouble);
    double headingDegrees = heading*180/M_PI;
    return headingDegrees;
}

void MAG3110::calibrate(){
    int readings[3];
    while(runCalibration){
        if(getBaring(readings)){
            if(readings[0] < minX){
                minX = readings[0];
                Serial.print("min x: ");
                Serial.println(minX);
            }
            if(readings[0] > maxX){
                maxX = readings[0];
                Serial.print("max x: ");
                Serial.println(maxX);
            }
            if(readings[1] < minY){
                minY = readings[1];
                Serial.print("min y: ");
                Serial.println(minY);
            }
            if(readings[1] > maxY){
                maxY = readings[1];
                Serial.print("max y: ");
                Serial.println(maxY);
            }
            if(readings[2] < minZ){
                minZ = readings[2];
                Serial.print("min z: ");
                Serial.println(minZ);
            }
            if(readings[2] > maxZ){
                maxZ = readings[2];
                Serial.print("max z: ");
                Serial.println(maxZ);
            }
        }else{
            Serial.println("Error getting barings in calibration function");
        }
        delay(10);
    }
    Serial.println("Calibration stopped");
    //Calculate X,Y,Z offsets
    xOffset = (maxX + minX)/2;
    yOffset = (maxY + minY)/2;
    zOffset = (maxZ + minZ)/2;
    //Store X,Y,Z offsets.
    
    EEPROM.put(8, xOffset);
    EEPROM.put(12, yOffset);
    EEPROM.put(16, zOffset);
    calibrated = true;
    
    
    Serial.print("X Offset: ");
    Serial.println(xOffset);
    Serial.print("Y Offset: ");
    Serial.println(yOffset);
    Serial.print("Z Offset: ");
    Serial.println(yOffset);
    
}

bool MAG3110::checkStoredVals(){
    if(!(EEPROM.read(8) == 255 && EEPROM.read(9) == 255 && EEPROM.read(10) == 255 && EEPROM.read(11) == 255)){
        EEPROM.get(8, xOffset);
        Serial.print("X Offset: ");
        Serial.println(xOffset);
    }else{
        Serial.println("no stored X Offset");
        return false;
    }
    if(!(EEPROM.read(12) == 255 && EEPROM.read(13) == 255 && EEPROM.read(14) == 255 && EEPROM.read(15) == 255)){
        EEPROM.get(12, yOffset);
        Serial.print("Y Offset: ");
        Serial.println(yOffset);
    }else{
        Serial.println("no stored Y Offset");
        return false;
    }
    if(!(EEPROM.read(16) == 255 && EEPROM.read(17) == 255 && EEPROM.read(18) == 255 && EEPROM.read(19) == 255)){
        EEPROM.get(16, zOffset);
        Serial.print("Z Offset: ");
        Serial.println(zOffset);
    }else{
        Serial.println("no stored Z Offset");
        return false;
    }
    calibrated = true;
    return true;
    
}