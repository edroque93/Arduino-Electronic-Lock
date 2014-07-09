void _init(){
  sLCD_hideCursor();
  sLCD_light(false);
  sLCD_clear();

  setLED(false);
  digitalWrite(LORANGE,HIGH);
}

void _waitTag(){
  while(true){
    if(_getTag())
      return;
      
    delay(500);
  }
}

char* _printName(){
  tone(SPEAKER, 440, 450);
  sLCD_light(true);
  sLCD_cursorHome();
  static char name[16]; // cada vez que se llame se machaca
  
  for(int i = 0;i < 4;i++)
    for(int j = 0;j < 4;j++)
      name[i*4+j] = card[i+2].part[j];
      
  sLCD_print("Welcome Home");
  sLCD_newLine();
  sLCD_print(name);
  sLCD_newLine();
  
  delay(1000);
  
  return name;
}

char* _waitPassword(){
  sLCD_setCursor(1,3);
  sLCD_print("Enter Password:");
  sLCD_setCursor(1,4);
  char *pwd = (char*) malloc(4*sizeof(char));
  char c;
  
  for(int i=0;i<5;i++){
    c = _waitInput();

    if(c == '#'){
      if(i>0){
        i -= 2;
        sLCD_backspace();
      }else{
        i--;
      }
    }else{ 
      if(i<4 && c!='*'){
        pwd[i]=c;
        sLCD_print("*");
      }else{
        if(i==4 && c=='*'){
          delay(700);
          return pwd;
        }else{
          i--;
        }
      }
    }
  }
}

char _waitInput(){
  byte state = LOW;
  char c;

  while(true){
    digitalWrite(LORANGE,state);
    state=!state;
    delay(200);
    c = sLCD_getKeypad();

    if(c!='x'){
      setLED(true);

      while(sLCD_getKeypad()!='x') 
        delay(10); // cuidadito con sobrecargar el sw serial

      setLED(false);
      return c;
    }
  }
}

boolean _userNewPassword(){
  sLCD_setCursor(1,3);
  sLCD_print("To change current   password, press #"); // Cabe bien en el LCD

  unsigned long max_time = millis()+2000; // tiempo actual + 2 segundos
  boolean state = false;

  while(millis() < max_time){
    if(sLCD_getKeypad()=='#'){
      delay(200); // slowdown cowboy
      setLED(false);
      return true;
    }
    
    delay(10); // Captura rápida de tecla
  }
  
  _init(); // al salir estará segundos sin hacer nada
  return false;  
}

void _setNewPassword(){
  sLCD_clear();
  sLCD_cursorHome();
  sLCD_print("Please, follow the  instructions"); // 2 Líneas y ready para reciclar

  char* pwd_1 = _waitPassword();

  sLCD_clear();
  sLCD_cursorHome();
  sLCD_print("Please, repeat the  password"); // 2 Líneas y ready para reciclar

  char* pwd_2 = _waitPassword();

  sLCD_clear();
  sLCD_cursorHome();

  // Check de contraseñas

  for(int i=0; i<4; i++){
    if(pwd_1[i] != pwd_2[i]){
      mal(false);
      free(pwd_1);
      free(pwd_2);
      sLCD_print("Password doesn't    match confirmation");
      return; 
    }
  }

  // La contraseña está bien, pedir tarjeta

  bien(false);
  sLCD_print("Hold the card above the card reader and wait");

  while(!_getTag()) {
    delay(500);
  }
  
  // Ey, comprueba que la tarjeta es del mismo dueño...
  
  for(int i = 0;i < 4;i++)
    for(int j = 0;j < 4;j++)
      if(user[i*4+j] != card[i+2].part[j]){
        setLED(false);
        mal(false);
        free(pwd_1);
        free(pwd_2);
        sLCD_clear();
        sLCD_print("Wrong card");
        return;  
      }
  
  // Escribe
  
  byte b[] = {pwd_1[0]-48, pwd_1[1]-48, pwd_1[2]-48, pwd_1[3]-48};
  _sendBlockWrite(0x06, b);
  delay(500); // no queremos que se retire la tarjeta prematuramente

  bien(false);
  sLCD_clear();
  sLCD_print("Success, have a niceday!");
  free(pwd_1);
  free(pwd_2);
}

boolean _checkPwd(char * pwd){
  for(int i=0;i<4;i++)
    if(card[6].part[i] != pwd[i]-48)
      return false;

  return true;
}

void setLED(boolean on){
  digitalWrite(LGREEN,on);
  digitalWrite(LRED,on);
  digitalWrite(LORANGE,on);
}






