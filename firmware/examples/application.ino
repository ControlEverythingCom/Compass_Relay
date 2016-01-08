#include "Compass_Relay/Compass_Relay.h"
#include <cmath>

MAG3110 compass;
NCD8Relay relayController;

float minDegrees = 181.00f;
float maxDegrees = 181.00f;

double headingDegrees;
double range = 181.00;
double degreeDivision = 181.00;
double points[8];

int setMaxDegrees(String data);
int setMinDegrees(String data);
int calibrateSensor(String command);
void checkStoredVals();

void setup() {
    if(!compass.init()){
        Serial.println("Failed to init Compass");
    }else{
        Serial.println("Compass initialized");
    }
    Particle.variable("Heading", headingDegrees);
    Particle.function("SetMinD", setMinDegrees);
    Particle.function("SetMaxD", setMaxDegrees);
    Particle.function("Calibrate", calibrateSensor);
    relayController.setAddress(0,0,0);
    relayController.turnOffAllRelays();
    
    checkStoredVals();
}

void loop() {
    headingDegrees = compass.getHeading();
    
    
    if(minDegrees != 181.00f && maxDegrees != 181.00f && headingDegrees != 181.00){
        if(range == 181.00){
            range = maxDegrees - minDegrees;
            degreeDivision = range/8;
            for(int i = 0; i < 8; i++){
                points[i] = maxDegrees - (degreeDivision*(i+1));
            }
        }
        bool triggered = false;
        for(int i = 0; i < 8; i++){
            double difference = points[i] - headingDegrees;
            if(difference < 0){
                difference = difference * -1;
            }
            if(difference < degreeDivision/2){
                triggered = true;
                relayController.turnOffAllRelays();
                relayController.turnOnRelay(i+1);
            }
        }
        if(!triggered){
            relayController.turnOffAllRelays();
        }
    }
    delay(10);
}

int setMaxDegrees(String data){
    Serial.println("set max degrees function fired");
    maxDegrees = headingDegrees;
    EEPROM.put(4, maxDegrees);
    if(minDegrees != 181){
        Serial.println("Setting ranges and what not");
        range = maxDegrees - minDegrees;
        degreeDivision = range/8;
        for(int i = 0; i < 8; i++){
            points[i] = maxDegrees - (degreeDivision*(i+1));
        }
    }
    return 1;
}
int setMinDegrees(String data){
    Serial.println("set min degrees function fired");
    minDegrees = headingDegrees;
    EEPROM.put(0, minDegrees);
    if(maxDegrees != 181){
        Serial.println("Setting ranges and what not");
        range = maxDegrees - minDegrees;
        degreeDivision = range/8;
        for(int i = 0; i < 8; i++){
            points[i] = maxDegrees - (degreeDivision*(i+1));
        }
    }
    return 1;
}

int calibrateSensor(String command){
    if(command.equalsIgnoreCase("Start")){
        compass.runCalibration = true;
        compass.calibrate();
    }else{
        compass.runCalibration = false;
    }
    return 1;
}

void checkStoredVals(){
    if(!(EEPROM.read(0) == 255 && EEPROM.read(1) == 255 && EEPROM.read(2) == 255 && EEPROM.read(3) == 255)){
        EEPROM.get(0, minDegrees);
        Serial.print("Min Degrees: ");
        Serial.println(minDegrees);
    }
    if(!(EEPROM.read(4) == 255 && EEPROM.read(5) == 255 && EEPROM.read(6) == 255 && EEPROM.read(7) == 255)){
        EEPROM.get(4, maxDegrees);
        Serial.print("Max Degrees: ");
        Serial.println(maxDegrees);
    }
}
