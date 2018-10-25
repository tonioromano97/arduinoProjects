#include <LiquidCrystal.h>

#define BUTTON 6
#define DIFFICULT 100

bool inGame = true;

short int bulletPosCol = 15, bulletPosRow = 0, manPosRow = 0, manPosCol = 0;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte bullet[8] = {0b00000, 0b00000, 0b00000, 0b01110, 0b01110, 0b00000, 0b00000, 0b00000 };

byte empty[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };

byte man[8] = { 0b00100, 0b01010, 0b00100, 0b00100, 0b01110, 0b10101, 0b00100, 0b01010 };

void setup() {
  pinMode(BUTTON,INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.createChar(0, empty);
  lcd.createChar(1, bullet);
  lcd.createChar(2, man);
  lcd.setCursor(0, 0);
  lcd.print("Game Bullet");
  lcd.write(byte(0));//empty
  lcd.write(byte(1));//bullet
  lcd.write(byte(0));//empty
  lcd.write(byte(2));//man
  lcd.setCursor(0,1);
  lcd.print("- PRESS  START -");
  while(!isPressedButton(BUTTON)); //Aspetto che preme start
  //disegno il man
  lcd.clear();
  lcd.setCursor(manPosCol,manPosRow);
  lcd.write(byte(2));
}

void loop() {
  if(inGame){
    printBullets();
    if(isPressedButton(BUTTON)) changeManPosition();
    if(!isWinner()) stopGame();
  }
  else if(isPressedButton(BUTTON)) {
    lcd.clear();
    inGame = true;
  }
}

bool isWinner(){
  if(manPosCol==bulletPosCol&&manPosRow==bulletPosRow) return false;
  else return true;
}

void stopGame(){
  inGame = false;
  lcd.clear();
  lcd.home();
  lcd.print("!!! YOU LOSE !!!");
  lcd.setCursor(0,1);
  lcd.print("Tempo: ");
  lcd.print(millis()/1000);
  lcd.print("s");
}

void changeManPosition(){
  lcd.setCursor(manPosCol,manPosRow);
  lcd.write(byte(0));
  manPosRow = !manPosRow; //inverto riga
  manPosCol = random(10); //>10 troppo vicino al proiettile
  lcd.setCursor(manPosCol,manPosRow);
  lcd.write(byte(2));
  delay(200);
}

void printBullets(){
  if(inGame){
    currentMillis = millis();
    if (currentMillis - previousMillis >= DIFFICULT) {
      previousMillis = currentMillis;
      //se ho finito lo sparo
      if(bulletPosCol<0) {
      //cancello l'ultimo bullet
      lcd.setCursor(0,bulletPosRow);
      lcd.write(byte(0));
      //mi preparo per un nuovo sparo
      bulletPosCol = 15;
      bulletPosRow = random(2);
      }
      //Sto sparando...
      //cancello il bullet precedente
      lcd.setCursor(bulletPosCol+1,bulletPosRow);
      lcd.write(byte(0));
      //stampo il nuovo bullet
      lcd.setCursor(bulletPosCol,bulletPosRow);
      lcd.write(byte(1));
      //decremento la colonna per stampare il prossimo bullet
      bulletPosCol--;
      //delay(100);
    }
  }  
}

bool isPressedButton(short int button){
  return !digitalRead(button);
}

