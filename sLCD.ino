/////////////////////////////////
//  Funciones LCD serial mode  //
//  Serial mode por software   //
/////////////////////////////////

void sLCD_init(byte rx, byte tx, int baud){
  swSerial_init(rx, tx, baud); 
}

void sLCD_print(char *c){
  int FIFO = sLCD_FIFO();

  while(*c != '\0'){
    if(FIFO == 0){
      delay(15);
      FIFO = sLCD_FIFO();
    }

    swSerial_write(*c++);
    FIFO--;
  }
}

void sLCD_print(byte b){
  while(sLCD_FIFO()==0);
  swSerial_write(b);
}

byte sLCD_FIFO(){
  return swSerial_read(14);
}

void sLCD_clear(){
  swSerial_write(12);
}

void sLCD_clearColumn(){
  swSerial_write(17);
}

void sLCD_light(boolean b){
  if(b) swSerial_write(19);
  else swSerial_write(20);
}

void sLCD_setCursor(byte x, byte y){
  swSerial_write(3); 
  swSerial_write(y); 
  swSerial_write(x); 
}

void sLCD_cursorHome(){
  swSerial_write(1);
}

void sLCD_hideCursor(){
  swSerial_write(4);
}

void sLCD_cursorUnderline(){
  swSerial_write(5);
}

void sLCD_newLine(){
  //swSerial_write(10);
  swSerial_write(13);
}

void sLCD_backspace(){
  swSerial_write(8);
}

void sLCD_cursorBlink(){
  swSerial_write(6);
}

char sLCD_getKeypad(){
  swSerial_write(16);
  // Serial.print("Voy a leer, ");
  byte loki = swSerial_read();
  // Serial.print("leo LOKi, ");  
  byte hiki = swSerial_read();
  // Serial.println("leo HIKI.");
  
  char res = 'x';
  
  if(loki & 1) res='1';
  if(loki & 2) res='2';
  if(loki & 4) res='3';
  if(loki & 8) res='4';
  if(loki & 16) res='5';
  if(loki & 32) res='6';
  if(loki & 64) res='7';
  if(loki & 128) res='8';
  if(hiki & 1) res='9';
  if(hiki & 2) res='*';
  if(hiki & 4) res='0';
  if(hiki & 8) res='#';

  return res;
}

