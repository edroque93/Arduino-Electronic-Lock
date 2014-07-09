// Main file

typedef struct{
  byte part[4];
} Block;

const byte SPEAKER = 9;
const byte LRED = 5;
const byte LGREEN = 6;
const byte LORANGE = 7;

Block card[8];
int WAITING = 5000;
char *user = NULL;
char *pwd = NULL;
const byte initCode[] = {13,0xFF,0x01,0x09,0x87,0x01,0x01,0x07,0x00,0x10,0x20,0x30,0x40,0x3A};

void setup(){
  Serial.begin(19200); // Comunicación serial RFID
  sLCD_init(2, 3, 9600); // Comunicación sw serial LCD

  pinMode(LRED, OUTPUT);
  pinMode(LGREEN, OUTPUT);
  pinMode(LORANGE, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  
  digitalWrite(LORANGE, HIGH);
  
  setupCard();
  _activate(initCode);  
  delay(500);
  Serial.flush();
  sLCD_light(true);
  sLCD_clear();
  sLCD_print("Cerradura iniciada");
  delay(3000);
 // byte b[] = {0, 0, 0, 0};
 // _sendBlockWrite(0x06, b);
}

void loop(){
  _init();
  _waitTag();
  
  user = _printName();
  pwd = _waitPassword();
  
  setLED(false);
  
  (_checkPwd(pwd))?bien(true):mal(true);
  free(pwd);
  delay(3000);
}

//////////////////////////////////////////////

void setupCard(){
  for(byte i=0;i<8;i++)
    for(byte j=0;j<4;j++)
      card[i].part[j] = 0;
}

void bien(boolean ask){ // Si queremos que solo suene, false
  if(ask){
    sLCD_clear();
    sLCD_cursorHome();
    sLCD_print("Access granted");
  }
  
  digitalWrite(LGREEN,HIGH);
  tone(SPEAKER,330,100);
  delay(100);
  tone(SPEAKER,440,100);
  
  if(ask)
    if(_userNewPassword())
      _setNewPassword();
}

void mal(boolean msg){ // Si queremos mensaje, true
  if(msg){
    sLCD_clear();
    sLCD_cursorHome();
    sLCD_print("An error ocurred");
  }
  
  digitalWrite(LRED,HIGH);
  tone(SPEAKER,330,100);
  delay(100);
  tone(SPEAKER,220,100);
}

//////////////////////////////////////////////

void printByte(byte data){
  int aux_1 = 0;
  int aux_2 = 0;

  aux_1=data/16;
  aux_2=data%16;
  if (aux_1<10) Serial.print((char)(aux_1 + 48));
  else Serial.print((char)(aux_1+55));
  
  if (aux_2<10) Serial.print((char)(aux_2 + 48));
  else Serial.print((char)(aux_2 + 55));

  Serial.print(" ");
}

void printCard(){
  byte checksum = 0;
  Serial.println("-------");
  Serial.println("Card Information");
  for(int i=0;i<8;i++){
    Serial.print("Block ");
    Serial.println(i);
    for(int j=0;j<4;j++){
      printByte(card[i].part[j]);
      checksum+=card[i].part[j];
    }
    Serial.println();
  }
  Serial.println("-------");
}
