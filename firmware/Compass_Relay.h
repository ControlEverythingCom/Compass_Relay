class MAG3110{
public:
    bool init();
    float getHeading();
    void calibrate();
    
    //Variables used for calibration
    float xOffset = 0;
    float yOffset = 0;
    float zOffset = 0;
    int minX = 537000000;
    int maxX = 0;

    int minY = 537000000;
    int maxY = 0;

    int minZ = 537000000;
    int maxZ = 0;
    
    
    bool runCalibration = false;
    bool calibrated = false;
private:
    bool getBaring(int buffer[]);
    bool checkStoredVals();
    
    int address = 0x0E;
    //Config registers
    int CTRL_REG1 = 0x10;
    int CTRL_REG2 = 0x11;
    
    //X axis registers
    int OUT_X_MSB = 0x01;
    int OUT_X_LSB = 0x02;
    
    //Y axis registers
    int OUT_Y_MSB = 0x04;
    int OUT_Y_LSB = 0x05;
    
    //Z axis registers
    int OUT_Z_MSB = 0x06;
    int OUT_Z_LSB = 0x07;
    
};