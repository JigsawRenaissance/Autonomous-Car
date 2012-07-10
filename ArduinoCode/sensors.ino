// Sensors Modules

int getGyroRate() {        // ARef=3.3V, Gyro sensitivity=2mV/(deg/sec)
   GYR_Y = getGyroValues(); 
   
   return int((GYR_Y)/(lastLoopUsefulTime));          
}

float getAccAngle() {
  float r;
  int regAddress = 0x32;    //first axis-acceleration-data register on the accelerometer
  readFrom(ACC, regAddress, TO_READ, buff); //read the acceleration data from the accelerometer  
   ACC_X = (((int)buff[1]) << 8) | buff[0];
   ACC_Y = (((int)buff[3])<< 8) | buff[2];
   ACC_Z = (((int)buff[5]) << 8) | buff[4];
   //ACC_X = (ACC_X * 0.00402832)*9.8;


   return ACC_X, ACC_Z;
}

//Writes val to address register on device
void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device); //start transmission to device 
   Wire.write(address);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

//reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}

//---------------- gyroscope set up ------------------------------------------

int setupgyroscope(int scale){
  //From  Jim Lindblom of Sparkfun's code

  // Enable x, y, z and turn off power down:
  writeTo(gyroscope_Address, CTRL_REG1, 0b00001111);

  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
  writeTo(gyroscope_Address, CTRL_REG2, 0b00000000);

  // Configure CTRL_REG3 to generate data ready interrupt on INT2
  // No interrupts used on INT1, if you'd like to configure INT1
  // or INT2 otherwise, consult the datasheet:
  writeTo(gyroscope_Address, CTRL_REG3, 0b00001000);

  // CTRL_REG4 controls the full-scale range, among other things:

  if(scale == 250){
    writeTo(gyroscope_Address, CTRL_REG4, 0b00000000);
  }else if(scale == 500){
    writeTo(gyroscope_Address, CTRL_REG4, 0b00010000);
  }else{
    writeTo(gyroscope_Address, CTRL_REG4, 0b00110000);
  }

  // CTRL_REG5 controls high-pass filtering of outputs, use it
  // if you'd like:
  writeTo(gyroscope_Address, CTRL_REG5, 0b00000000);
}


// ------------------ read gyroscope angles ---------------------

float getGyroValues(){
  int gx, gy, gz;

  byte xMSB = readRegister(gyroscope_Address, 0x29);
  byte xLSB = readRegister(gyroscope_Address, 0x28);
  gx = ((xMSB << 8) | xLSB);
 
  byte yMSB = readRegister(gyroscope_Address, 0x2B);
  byte yLSB = readRegister(gyroscope_Address, 0x2A);
  gy = ((yMSB << 8) | yLSB);

 // byte zMSB = readRegister(gyroscope_Address, 0x2D);
 // byte zLSB = readRegister(gyroscope_Address, 0x2C);
//  gz = ((zMSB << 8) | zLSB);
  
  return gy;
}

void writeRegister(int deviceAddress, byte address, byte val) {
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

    int v;
    Wire.beginTransmission(deviceAddress);
    Wire.write(address); // register to read
    Wire.endTransmission();

    Wire.requestFrom(deviceAddress, 1); // read a byte

    while(!Wire.available()) {
        // waiting
    }

    v = Wire.read();
    return v;
}
