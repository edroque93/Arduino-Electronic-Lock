////////////////////////
//  Programar el tag  // 
////////////////////////
void _activate(const byte *argv){
  int limit = argv[0];
  
  for(int i=1;i<=limit;i++)
    Serial.write(argv[i]);
}

/////////////////////////////////////////////
//  Response may arrive with ~500ms delay  //
/////////////////////////////////////////////
void _sendBlockWrite(byte bBlock, byte *data){ // La tarjeta tiene que estar más tiempo para escribir
  byte csum = 0x01+0x06+0x20+bBlock;

  _sendHeader();
  Serial.write(0x06); // Length
  Serial.write(0x20); // Command
  Serial.write(bBlock); // Block number
  
  for(int i = 0; i<4; i++){
    Serial.write(data[i]); // Each byte from *data
    csum += data[i];
  }

  Serial.write(csum);
}

//////////////////////////
//  On success:         // 
//  ¿¿¿¿¿¿              // 
//////////////////////////
boolean _getTag(){
  byte simon = 0x01+0x1d+0x10;
  
  if(Serial.read()==0xFF){
    Serial.println("imreading");
    card[0].part[1]=Serial.read();  //01
    card[0].part[2]=Serial.read();  //1d
    card[0].part[3]=Serial.read();  //10
    for(int i=1;i<8;i++){
      for(int j=0;j<4;j++){
        card[i].part[j]=Serial.read();
        simon+=card[i].part[j];
      }
    }
    byte checksum = Serial.read();
    
    // DEPURAR
    if(checksum == simon){
      Serial.print("Card read with checksum ");
      Serial.println(checksum);

      printCard();

      Serial.flush();

      return true;
    }
  }
  return false; 
}

///////////////
//  Private  //
///////////////

void _sendHeader(){
  Serial.write(0xFF);
  Serial.write(0x01);
}
