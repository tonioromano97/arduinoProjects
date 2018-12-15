#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define next 6
#define more 7
#define less 8

double totale;
double diffMontepremi;
double ambo,terna,quaterna,cinquina,tombola;

short int stato = 1, statoPrecedente=0;

void setup() {
  pinMode(next, INPUT_PULLUP);
  pinMode(more, INPUT_PULLUP);
  pinMode(less, INPUT_PULLUP);
  lcd.begin(16, 2);   
  printLCD("CALCOLO  VINCITE", "< PREMI  NEXT  >");
  while(!isPressedButton(next));
  inizializza();
}

void loop() {
  if(isPressedButton(next)) inizializza();
  visualizzaPremi();
}

void inizializza(){
  printLCD("INSERISCI TOTALE", "E. "+String(totale)+"    Next>");
  delay(500);
  while(!isPressedButton(next)){
    if(isPressedButton(more)&&totale<99.80){
      delay(200);
      while(isPressedButton(more)&&totale<98) {
        totale++;
        aggiornaTotale();
        delay(200);
      }
      totale = totale + 0.10;
      aggiornaTotale();
    }
    else if(isPressedButton(less)&&totale>0.1){
      delay(200);
      while(isPressedButton(less)&&totale>1) {
        totale--;
        aggiornaTotale();
        delay(200);
      }
      totale = totale - 0.10;
      aggiornaTotale();
    }
  }
  printLCD("Diff. Vinc:  "+String(diffMontepremi), "Calcola premi ->");
  delay(500);
  while(!isPressedButton(next)){
    if(isPressedButton(more)&&(totale/(diffMontepremi+0.1)>10)){
      diffMontepremi=diffMontepremi+0.1;
      aggiornaDiffMontepremi();
      delay(200);
    }
    else if(isPressedButton(less)&&diffMontepremi>0.1){
      diffMontepremi=diffMontepremi-0.1;
      aggiornaDiffMontepremi();
      delay(200);
    }
  }
  calcolaPremi();
  delay(500);
  String msg = "Tot: "+String(totale)+" d:"+String(diffMontepremi);
  printLCD(msg, "                ");  
  stato = 1; statoPrecedente = 0;
}

void printLCD(String primaRiga, String secondaRiga){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(primaRiga);
  lcd.setCursor(0,1);
  lcd.print(secondaRiga);
}

void checkStato(short int statoMore, short int statoLess){
  if(isPressedButton(more)){
    stato=statoMore;
    delay(200);
  }
  else if(isPressedButton(less)){
    stato=statoLess;
    delay(200);
  }  
}

void mostraPremi(String premio, double valore){
  if(statoPrecedente==stato) return;
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("<");
  lcd.print(premio);
  lcd.print(": ");
  lcd.print(valore);
  lcd.setCursor(15,1);
  lcd.print(">");
  statoPrecedente = stato;
}

void visualizzaPremi(){
  switch(stato){
    case 1:{
      mostraPremi("Ambo",ambo);
      checkStato(2,5);
      break;
    }
    case 2:{
      mostraPremi("Terna",terna);
      checkStato(3,1);
      break;
    }
    case 3:{
      mostraPremi("Quaterna",quaterna);
      checkStato(4,2);
      break;
    }
    case 4:{
      mostraPremi("Cinquina",cinquina);
      checkStato(5,3);
      break;
    }
    case 5:{
      mostraPremi("Tombola",tombola);
      checkStato(1,4);
      break;
    }
  }
}

void aggiornaTotale(){
  if(totale>10){
    lcd.setCursor(3,1);
    lcd.print(totale);
  }
  else{
    lcd.setCursor(3,1);
    lcd.print(totale);
    lcd.print("  ");    
  }
}

void aggiornaDiffMontepremi(){
  lcd.setCursor(13,0);
  lcd.print(diffMontepremi);
}

void calcolaPremi(){
  ambo = (totale - 10 * diffMontepremi) / 5;
  terna = ambo + diffMontepremi;
  quaterna = ambo + diffMontepremi * 2;
  cinquina = ambo + diffMontepremi * 3;
  tombola = ambo + diffMontepremi * 4;
  if(ambo>1){
    tombola = tombola + (ambo-int(ambo)) + (terna-int(terna)) + (quaterna-int(quaterna)) + (cinquina-int(cinquina));
    ambo = (int) ambo;
    terna = (int) terna;
    quaterna = (int) quaterna;
    cinquina = (int) cinquina;
  }
}

bool isPressedButton(short int pin){
  return !digitalRead(pin);
}
