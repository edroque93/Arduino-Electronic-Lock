////////////////
//  swSerial  //
////////////////

int _rx = 0;
int _tx = 0;
int _baudrate = 0;
int _bitperiod = 0;
int _bitdelay = 0;
int _skiphalf = 0;

//////////////
//  Public  //
//////////////

void swSerial_init(byte rx, byte tx, int baud){
  _rx = rx;
  _tx = tx;
  _baudrate = baud;
  
  // Cálculos
  
  _bitperiod = 1000000/_baudrate;
  _bitdelay = _bitperiod - clockCyclesToMicroseconds(50); // docs -> digWrite~50cycles
  _skiphalf = _bitdelay/2 - clockCyclesToMicroseconds(50);
  
  // Setup
  
  pinMode(_rx, INPUT_PULLUP);
  pinMode(_tx, OUTPUT);

  digitalWrite(_tx, HIGH);
  delayMicroseconds(_bitperiod);
}

byte swSerial_read(){
  if(!swSerial_ready) return -1;
  
  while(digitalRead(_rx)); // Alguien DEBE responder
  
  int res = 0;
  
  if(digitalRead(_rx)==LOW){ // Start bit
    delayMicroseconds(_skiphalf);
    
    for(int i = 0; i<8; i++){
      delayMicroseconds(_bitdelay);
      res |= digitalRead(_rx) << i;
    } 
    
    delayMicroseconds(_bitperiod);
    return res;
  }
  
  return -1; // Ruido
}

byte swSerial_read(byte b){
  swSerial_write(b);
  return swSerial_read();
}

void swSerial_write(byte b){
  if(!swSerial_ready) return;
  
  swSerial_sendBit(0);

  for(int i = 0; i<8; i++)
    swSerial_sendBit(b >> i);

  swSerial_sendBit(1);
}

///////////////
//  Private  //
///////////////

void swSerial_sendBit(byte b){
  digitalWrite(_tx, b & 1);
  delayMicroseconds(_bitdelay);
}

boolean swSerial_ready(){
  if(_tx == 0 || _rx == 0 || _baudrate == 0) return false;
  return true;
}

/////////////////////////////////////////
// Info -> http://code.google.com/p/arduino/source/browse/trunk/libraries/SoftwareSerial/SoftwareSerial.cpp?r=1119
// Info -> http://controls.ame.nd.edu/microcontroller/main/node24.html
// Info -> http://en.wikipedia.org/wiki/Asynchronous_serial_communication
/////////////////////////////////////////
