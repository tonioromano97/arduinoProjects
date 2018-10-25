#include <LiquidCrystal.h>
#define brightness 10
#define setHMS 6
#define plus 7
#define less 8
#define valBrightness A0

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char h,hh,m,mm,s,ss;

void setup() {
  Serial.begin(9600);
  analogWrite(brightness,255);
  lcd.begin(16, 2);
  lcd.home();
  lcd.print("Orologio  Romano");
  delay(2000);
  lcd.clear();
  pinMode(brightness,OUTPUT);
  pinMode(setHMS,INPUT_PULLUP);
  pinMode(plus,INPUT_PULLUP);
  pinMode(less,INPUT_PULLUP);
  settingHMS();
}

void settingHMS(){
  lcd.home();
  lcd.print("IMPOSTA ORA:    ");
  lcd.setCursor(0,1);
  lcd.print("00 : 00 : 00");
  lcd.blink();
  h = setNumber(0,'2');
  hh = setNumber(1,'9');
  m = setNumber(5,'5');
  mm = setNumber(6,'9');
  s = setNumber(10,'5');
  ss = setNumber(11,'9');
  lcd.noBlink();
  lcd.home();
  lcd.print("Orologio  Romano");
}

char setNumber(short int column, char maxN){
  lcd.setCursor(column,1);
  char number = '0';
  while(digitalRead(setHMS)){
    if(!digitalRead(plus)) {
      if(number>=maxN) number = '0';
      else number ++;
      lcd.write(number);
      lcd.setCursor(column,1);
      delay(200); //tempo di lasciaere il dito dal pulsante;
    }
    else if(!digitalRead(less)){
      if(number!='0') number--;
      lcd.write(number);
      lcd.setCursor(column,1);
      delay(200); //tempo di lasciaere il dito dal pulsante;
  }
  }
  delay(500); //tempo di lasciaere il dito dal pulsante;
  return number;
}

void loop() {
  //imposto la luminosità del display
  setBrightness();
  //se viene premuto il pulsante di set vado a settare l'ora.
  if(!digitalRead(setHMS)) { delay(500); settingHMS(); }
  //se sono le 23 e 59 riporto a 00:00:00
  if(h=='2'&&hh=='3'&&m=='5'&mm=='9'&s=='5'&ss=='9'){
    h=hh=m=mm=s=ss='0';
    lcd.setCursor(0,1);
    lcd.print("00 : 00 : 00");
  }
  if(aggiornoOrario(11,&ss,'9'))
    if(aggiornoOrario(10,&s,'5'))
      if(aggiornoOrario(6,&mm,'9'))
        if(aggiornoOrario(5,&m,'5'))
          if(aggiornoOrario(1,&hh,'9'))
            if(aggiornoOrario(0,&h,'2'));
   
  delay(1000);
}

//Ritorno true se devo cambiare colonna. False altrimenti.
bool aggiornoOrario(short int column, char *c, char maxC){
  lcd.setCursor(column,1);
  if(*c<maxC){
    (*c)++;
    lcd.write(*c);
    return false;
  }
  else{
    (*c) = '0';
    lcd.write(*c);
    return true;
  }
}

unsigned int b;
void setBrightness(){
  b = map(analogRead(valBrightness), 0, 1023, 1, 255);
  analogWrite(brightness,b); //imposto luminosità al display
}
