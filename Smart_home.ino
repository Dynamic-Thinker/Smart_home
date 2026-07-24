#include <IRremote.h>
#include <EEPROM.h>
#include <BfButton.h>
#include <LiquidCrystal.h>

#define rg1 3                 // outputA CLK
#define rg2 4                 // outputB DT
#define AC_pin 5
#define ir_pin 6
#define f 7
#define r1 8
#define r2 9
#define r3 10
#define sw 11

byte State;
byte m = 0;
byte dim = 0;                  // Dimming level (0-128)  0 = on, 128 = 0ff
bool f_1 = 0;
bool r_1 = 0;
bool r_2 = 0;
bool r_3 = 0;
byte LastState;
bool dis2 = 0;
byte f_state = 2;
volatile int error_acc = 0;
char wStr[16] = {'0','\0'};
unsigned long t = 0;
unsigned long f_t = 0;
unsigned long i_t = 0;
bool stateChanged = false;
bool last_r1 = 0, last_r2 = 0, last_r3 = 0;
byte last_dim = 0, last_f_state = 2;
IRrecv irrecv(ir_pin);
decode_results results;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
BfButton btn(BfButton::STANDALONE_DIGITAL, sw, true, LOW);

void button(BfButton *btn, BfButton::press_pattern_t pattern);
void sendState();

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(false);
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection

  f_t = millis();
  pinMode(sw, INPUT);
  pinMode(rg1, INPUT);
  pinMode(rg2, INPUT);
  pinMode(f, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(AC_pin, OUTPUT);

  if (EEPROM.read(5) != 0xA5) {
    EEPROM.write(0, 0); // r_1
    EEPROM.write(1, 0); // r_2
    EEPROM.write(2, 0); // r_3
    EEPROM.write(3, 128); // dim (128 = off)
    EEPROM.write(4, 2); // f_state auto
    EEPROM.write(5, 0xA5);
  }
  
  r_1 = EEPROM.read(0);
  r_2 = EEPROM.read(1);
  r_3 = EEPROM.read(2);
  dim = EEPROM.read(3);
  f_state = EEPROM.read(4);

  last_r1 = r_1;
  last_r2 = r_2;
  last_r3 = r_3;
  last_dim = dim;
  last_f_state = f_state;

  digitalWrite(r1, r_1);
  digitalWrite(r2, r_2);
  digitalWrite(r3, r_3);
  LastState = digitalRead(rg1);
  btn.onPress(button)
  .onDoublePress(button) // default timeout
  .onPressFor(button, 3000); // custom timeout for 1 second

  lcd.begin(16, 2);
  lcd.clear();
}

void saveState()
{
  EEPROM.update(0, r_1);
  EEPROM.update(1, r_2);
  EEPROM.update(2, r_3);
  EEPROM.update(3, dim);
  EEPROM.update(4, f_state);
}

void zero_cross_detect()
{
  if (!r_3 || dim == 128) {
    digitalWrite(AC_pin, LOW);
    return;
  }
  if (dim == 0) {
    digitalWrite(AC_pin, HIGH);
    return;
  }
  
  int active = 128 - dim;
  error_acc += active;
  if (error_acc >= 128) {
    error_acc -= 128;
    digitalWrite(AC_pin, HIGH);
  } else {
    digitalWrite(AC_pin, LOW);
  }
}

void sendState() {
  Serial.print("M:"); Serial.print(r_1 || r_2 || r_3 ? 1 : 0);
  Serial.print(",T:"); Serial.print(r_1 ? 1 : 0);
  Serial.print(",P:"); Serial.print(r_2 ? 1 : 0);
  Serial.print(",F:"); Serial.print(r_3 ? 1 : 0);
  Serial.print(",S:"); Serial.print(dim);
  Serial.print(",C:"); Serial.print(f_state);
  Serial.print('\n');
}

void wifi()
{
  if (Serial.available() > 0)
  {
    char buffer[40];
    int len = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
    buffer[len] = '\0'; // Null-terminate
    if (len > 0 && buffer[len - 1] == '\r') {
        buffer[len - 1] = '\0';
        len--;
    }

    if (len == 0) return;
    
    if (buffer[0] == '?' && buffer[1] == '\0') {
      saveState();
      sendState();
      return;
    }

    stateChanged = false;
    
    // Parse using strtok
    char *pair = strtok(buffer, ",");
    while (pair != NULL) {
      char *colon = strchr(pair, ':');
      if (colon != NULL) {
        *colon = '\0'; // Split into key and value
        char *key = pair;
        char *value = colon + 1;
        
        if (key[0] == 'T') { r_1 = atoi(value); stateChanged = true; }
        else if (key[0] == 'P') { r_2 = atoi(value); stateChanged = true; }
        else if (key[0] == 'F') { r_3 = atoi(value); stateChanged = true; }
        else if (key[0] == 'S') { dim = atoi(value); stateChanged = true; }
        else if (key[0] == 'C') { f_state = atoi(value); stateChanged = true; }
        else if (key[0] == 'W') { 
            strncpy(wStr, value, sizeof(wStr) - 1); 
            wStr[sizeof(wStr) - 1] = '\0'; 
        }
      }
      pair = strtok(NULL, ",");
    }

    if (stateChanged) {
      digitalWrite(r1, r_1);
      digitalWrite(r2, r_2);
      digitalWrite(r3, r_3);
    }
  }
}

void ir_switch()
{
  stateChanged = false;
  if(irrecv.decode(&results)){
    
    if(millis() - i_t >= 500){
      if(results.value == 0xE12440BF)
      {
        r_1 = !r_1;
        digitalWrite(r1, r_1);
        stateChanged = true;
      }
      else if(results.value == 0xE12428D7)
      {
        r_2 = !r_2;
        digitalWrite(r2, r_2);
        stateChanged = true;
      }
      else if(results.value == 0xE1246897)
      {
        r_3 = !r_3;
        digitalWrite(r3, r_3);
        stateChanged = true;
      }
      else if(results.value == 0xE124609F)
      {
        if(f_state > 0)f_state--;
        else f_state = 2;
        stateChanged = true;
      }
      else if(results.value == 0xE12448B7)
      {
        if(r_1 || r_2 || r_3)
        {
        digitalWrite(r1, 0);
        digitalWrite(r2, 0);
        digitalWrite(r3, 0);
        r_1 = r_2 = r_3 = 0;
        Serial.print("M:0,T:0,P:0,F:0\n");
        last_r1 = last_r2 = last_r3 = 0;
        }
        else
        {
        digitalWrite(r1, 1);
        digitalWrite(r2, 1);
        digitalWrite(r3, 1);
        r_1 = r_2 = r_3 = 1;
        Serial.print("M:1,T:1,P:1,F:1\n");
        last_r1 = last_r2 = last_r3 = 1;
        }
        stateChanged = true;
      }
      else if(results.value == 0xE124A05F)
      {
        if(dim < 112)dim += 16;
        else dim = 128;
        stateChanged = true;
      }
      else if(results.value == 0xE12458A7)
      {
        if(dim > 16)dim -= 16;
        else dim = 0;
        stateChanged = true;
      }
      irrecv.resume();

      if(stateChanged == true){
        i_t = millis();
      }
    }
    else irrecv.resume();
  }
}

void manual()
{
  State = digitalRead(rg1);
  if(State != LastState)
  {
   if(digitalRead(rg2) != State)
   {
    if(dis2 == 1)
    {
      if(f_state > 0)f_state--;
      else f_state = 2;
    }
    else if(m == 0)
    {
     r_3 = 1;
     if(dim > 16)dim -= 16;
     else dim = 0;
    }
    else if(m == 1)
    {
     t = millis();
     r_1 = 1;
     digitalWrite(r1, r_1);
    }
    else if(m == 2)
    {
     t = millis();
     r_2 = 1;
     digitalWrite(r2, r_2);
    }
   }
   else
   {
    if(dis2 == 1)
    {
      if(f_state < 2)f_state++;
      else f_state = 0;
    }
    else if(m == 0)
    {
     if(dim < 112)
     {
      dim += 16;
      r_3 = 1;
     }
     else{
      dim = 128;
      r_3 = 0;
     }
    }
    else if(m == 1)
    {
     t = millis();
     r_1 = 0;
     digitalWrite(r1, r_1);
    }
    else if(m == 2)
    {
     t = millis();
     r_2 = 0;
     digitalWrite(r2, r_2);
    }
   }
  }
  LastState = State;
  btn.read();
}

void button(BfButton *btn, BfButton::press_pattern_t pattern)
{
  switch(pattern)
  {
   case BfButton::SINGLE_PRESS:
    m++;
    if(m > 2)m = 0;
    t = millis();
    break;

   case BfButton::DOUBLE_PRESS:
    if(r_1 || r_2 || r_3)
    {
     digitalWrite(r1, 0);
     digitalWrite(r2, 0);
     digitalWrite(r3, 0);
     r_1 = r_2 = r_3 = 0;
     Serial.print("M:0,T:0,P:0,F:0\n");
     last_r1 = last_r2 = last_r3 = 0;
    }
    else
    {
     digitalWrite(r1, 1);
     digitalWrite(r2, 1);
     digitalWrite(r3, 1);
     r_1 = r_2 = r_3 = 1;
     Serial.print("M:1,T:1,P:1,F:1\n");
     last_r1 = last_r2 = last_r3 = 1;
    }
    m = 0;
    break;

   case BfButton::LONG_PRESS:
    dis2 = !dis2;
    break;
  }
}

void dis()
{
  if(dis2 == 1)
  {    
    lcd.setCursor(0, 0);
    if((wStr[0] == '0') && (wStr[1] == '\0'))lcd.print(" Connecting...! ");
    else{
      lcd.print(wStr);
      lcd.print("    ");
    }

    lcd.setCursor(0, 1);
    lcd.print("  Cooler: ");
    lcd.setCursor(10, 1);
    if(f_state == 2)lcd.print("AUTO  ");
    else if(f_state == 1)lcd.print("ON    ");
    else lcd.print("OFF   ");
    
    return;
  }

  if(m > 0)if(millis() - t >= 5000)m=0;

  if(m == 1)
  {
   lcd.setCursor(0, 0);
   lcd.print("~");
  }
  else
  {
   lcd.setCursor(0, 0);
   lcd.print(" ");
  }
  if(r_1 == 0)
  {
   lcd.setCursor(1, 0);
   lcd.print("TB:OFF ");
  }
  else
  {
   lcd.setCursor(1, 0);
   lcd.print("TB:ON  ");
  }

  if(m == 2)
  {
   lcd.setCursor(8, 0);
   lcd.print("~");
  }
  else
  {
   lcd.setCursor(8, 0);
   lcd.print(" ");
  }
  if(r_2 == 0)
  {
   lcd.setCursor(9, 0);
   lcd.print("PP:OFF ");
  }
  else
  {
   lcd.setCursor(9, 0);
   lcd.print("PP:ON  ");
  }

  if(dim == 128)
  {
   lcd.setCursor(15, 1);
   lcd.print("0");
   digitalWrite(r3, 0);
  }
  else
  {
   lcd.setCursor(15, 1);
   digitalWrite(r3, r_3);
   if(dim >= 112)
   {
    lcd.print("1");
   }
   else if(dim >= 96)
   {
    lcd.print("2");
   }
   else if(dim >= 80)
   {
    lcd.print("3");
   }
   else if(dim >= 64)
   {
    lcd.print("4");
   }
   else if(dim >= 48)
   {
    lcd.print("5");
   }
   else if(dim >= 32)
   {
    lcd.print("6");
   }
   else if(dim >= 16)
   {
    lcd.print("7");
   }
   else
   {
    lcd.print("8");
   }
  }
  if(r_3 == 0)
  {
   lcd.setCursor(0, 1);
   lcd.print("FAN:OFF SPEED->");
  }
  else
  {
   lcd.setCursor(0, 1);
   lcd.print("FAN:ON  SPEED->");
  }
}

void fan()
{
  if (f_state == 2) {
    if (f_1 == 0 && millis() - f_t >= 600000) {
      f_1 = 1;
      f_t = millis();
      digitalWrite(f, HIGH);
    }
    else if (f_1 == 1 && millis() - f_t >= 300000) {
      f_1 = 0;
      f_t = millis();
      digitalWrite(f, LOW);
    }
  }
  else if (f_state == 1) {
    if (f_1 == 0) {
      f_1 = 1;
      digitalWrite(f, HIGH);
    }
  }
  else {
    f_1 = 0;
    digitalWrite(f, LOW);
  }
}

void loop()
{
  wifi();
  manual();
  ir_switch();
  dis();
  fan();

  if (r_1 != last_r1 || r_2 != last_r2 || r_3 != last_r3 || dim != last_dim || f_state != last_f_state) {
      saveState();
      
      if (r_1 != last_r1) { Serial.print("T:"); Serial.print(r_1 ? 1 : 0); Serial.print('\n'); last_r1 = r_1; }
      if (r_2 != last_r2) { Serial.print("P:"); Serial.print(r_2 ? 1 : 0); Serial.print('\n'); last_r2 = r_2; }
      if (r_3 != last_r3) { Serial.print("F:"); Serial.print(r_3 ? 1 : 0); Serial.print('\n'); last_r3 = r_3; }
      if (dim != last_dim) { Serial.print("S:"); Serial.print(dim); Serial.print('\n'); last_dim = dim; }
      if (f_state != last_f_state) { Serial.print("C:"); Serial.print(f_state); Serial.print('\n'); last_f_state = f_state; }
  }
}
