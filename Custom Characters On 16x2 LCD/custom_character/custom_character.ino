
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 9, 10, 11, 12, 13);

byte b[8] = {
 B00000, 
 B11111, 
 B10001, 
 B10001,  
 B10001, 
 B11111,
 B00000,
 B00000 
};

byte s[8] = {
 B00000, 
 B00000, 
 B01010, 
 B00000,  
 B10001, 
 B01110,
 B00000,
 B00000 
};

byte a[8] = {
 B00100, 
 B01010, 
 B10001, 
 B11011,  
 B01010, 
 B01010,
 B01010,
 B01110 
};


void setup() {
  lcd.createChar(0,b);
  lcd.createChar(1,s);
  lcd.createChar(2,a);

  

  lcd.begin(16, 2);  
    
}

void loop() {
   lcd.setCursor(0,0);
   lcd.write(byte(0));
   
   
   lcd.setCursor(2,0);
   lcd.write(byte(1));
   
   
   lcd.setCursor(4,0);
   lcd.write(byte(2));
  
  }
