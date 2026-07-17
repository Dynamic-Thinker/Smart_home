#include "IRremote.h"
#include <TimerOne.h>
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

int State;
int m = 0;
int dim = 0;                  // Dimming level (0-128)  0 = on, 128 = 0ff
bool f_1 = 0;
bool r_1 = 0;
bool r_2 = 0;
bool r_3 = 0;
int LastState;
bool dis2 = 0;
char state[29];
int f_state = 2;
volatile int i=0;
int freqStep = 75;
char wStr[16] = {0};
unsigned long t = 0;
unsigned long nt = 0;
unsigned long f_t = 0;
unsigned long f_nt = 0;
volatile boolean zero_cross=0;

IRrecv irrecv(ir_pin);
decode_results results;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
BfButton btn(BfButton::STANDALONE_DIGITAL, sw, true, LOW);

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);
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
  digitalWrite(r1, r_1);
  digitalWrite(r2, r_2);
  digitalWrite(r3, r_3);
  LastState = digitalRead(rg1);
  btn.onPress(button)
  .onDoublePress(button) // default timeout
  .onPressFor(button, 3000); // custom timeout for 1 second

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome Ashraq!!");
  delay(3000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("TB:OFF  PP:OFF");
  lcd.setCursor(0, 1);
  lcd.print("FAN:OFF SPEED->5");
}

void zero_cross_detect()
{
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i=0;
  digitalWrite(AC_pin, LOW);
}
// Turn on the TRIAC at the appropriate time

void dim_check()
{
  if(zero_cross == true)
  {
   if(i>=dim)
   {
      digitalWrite(AC_pin, HIGH);  // turn on fan
      i=0;  // reset time step counter
      zero_cross=false;    // reset zero cross detection
   }
   else
   {
    i++;  // increment time step counter
   }
  }
}

void wifi()
{
  // Read the full state string sent from ESP‑01, e.g. "M:0,T:1,P:0,F:1,S:64,W:0"
  if (Serial.available() > 0)
  {
    String packet = Serial.readStringUntil('\n');
    packet.trim();
    if (packet.length() == 0) return;
    if(packet == "?"){
      Serial.print(state);
      return;
    }

    int mVal = 0, tVal = 0, pVal = 0, fVal = 0, sVal = 0, cVal = 0;
    sscanf(packet.c_str(), "M:%d,T:%d,P:%d,F:%d,S:%d,C:%d,W:%15s", &mVal, &tVal, &pVal, &fVal, &sVal, &cVal, wStr);

    if (wStr[0] == '0' && wStr[1] == '\0')return;

    if (mVal == 1) {
      r_1 = r_2 = r_3 = true;
    } else if (mVal == 0) {
      r_1 = r_2 = r_3 = false;
    }
    // Individual device states (override master if they differ)
    r_1 = (tVal == 1);
    r_2 = (pVal == 1);
    r_3 = (fVal == 1);

    digitalWrite(r1, r_1);
    digitalWrite(r2, r_2);
    digitalWrite(r3, r_3);

    // Dim level (0‑128)
    dim = sVal;

    f_state = cVal;
    
    sprintf(state, "M:%d,T:%d,P:%d,F:%d,S:%d,C:%d\n", (r_1 || r_2 || r_3), r_1, r_2, r_3, dim, f_state);
    Serial.print(state); //M:0,T:1,P:0,F:1,S:64
  }
}

void ir_switch()
{
  if(irrecv.decode(&results))
  {
   if(results.value == 0x1FE50AF)
   {
    r_1 = !r_1;
    digitalWrite(r1, r_1);
   }
   else if(results.value == 0x1FED827)
   {
    r_2 = !r_2;
    digitalWrite(r2, r_2);
   }
   else if(results.value == 0x1FEF807)
   {
    r_3 = !r_3;
    digitalWrite(r3, r_3);
   }
   else if(results.value == 0x1FE30CF)
   {
    if(f_state > 0)f_state--;
    else f_state = 2;
   }
   else if(results.value == 0x1FE48B7)
   {
    if(r_1 || r_2 || r_3)
    {
     digitalWrite(r1, 0);
     digitalWrite(r2, 0);
     digitalWrite(r3, 0);
     r_1 = r_2 = r_3 = 0;
    }
    else
    {
     digitalWrite(r1, 1);
     digitalWrite(r2, 1);
     digitalWrite(r3, 1);
     r_1 = r_2 = r_3 = 1;
    }
   }
   else if(results.value == 0x1FEA05F)
   {
    if(dim < 100)
    {
     dim = dim + 25;
    }
    else
    {
     dim=128;
    }
   }
   else if(results.value == 0x1FE609F)
   {
    if(dim == 128)
    {
     dim = 100;
    }
    else if(dim > 25)
    {
     dim = dim - 25;
    }
    else
    {
     dim = 0;
    }
   }
   irrecv.resume();

   sprintf(state, "M:%d,T:%d,P:%d,F:%d,S:%d,C:%d\n", (r_1 || r_2 || r_3), r_1, r_2, r_3, dim, f_state);
   Serial.print(state);
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
     if(dim >= 5)dim -= 5;
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
     if(dim < 125)
     {
      dim += 5;
      r_3 = 1;
     }
     else
     {
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

   sprintf(state, "M:%d,T:%d,P:%d,F:%d,S:%d,C:%d\n", (r_1 || r_2 || r_3), r_1, r_2, r_3, dim, f_state);
   Serial.print(state);
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
    }
    else
    {
     digitalWrite(r1, 1);
     digitalWrite(r2, 1);
     digitalWrite(r3, 1);
     r_1 = r_2 = r_3 = 1;
    }
    m = 0;
    
    sprintf(state, "M:%d,T:%d,P:%d,F:%d,S:%d,C:%d\n", (r_1 || r_2 || r_3), r_1, r_2, r_3, dim, f_state);
    Serial.print(state);
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
    lcd.clear();
    
    lcd.setCursor(0, 0);
    if((wStr[0] == '0') && (wStr[1] == '\0'))lcd.print("Connecting...!");
    else lcd.print(wStr);

    lcd.setCursor(2, 1);
    lcd.print("Cooler: ");
    lcd.setCursor(10, 1);
    if(f_state == 2)lcd.print("AUTO");
    else if(f_state == 1)lcd.print("ON  ");
    else lcd.print("OFF ");
    
    return;
  }

  if(m > 0)nt = millis();
  if(nt - t >= 6000)m = 0;

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
   digitalWrite(r3, r_3);
   if(dim >= 100)
   {
    lcd.setCursor(15, 1);
    lcd.print("1");
   }
   else if(dim >= 75)
   {
    lcd.setCursor(15, 1);
    lcd.print("2");
   }
   else if(dim >= 50)
   {
    lcd.setCursor(15, 1);
    lcd.print("3");
   }
   else if(dim >= 25)
   {
    lcd.setCursor(15, 1);
    lcd.print("4");
   }
   else
   {
    lcd.setCursor(15, 1);
    lcd.print("5");
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
  if(f_state == 2){
    f_nt = millis();
    if((f_nt - f_t >= 600000) && f_1 == 0)
    {
    f_1 = 1;
    f_t = millis();
    digitalWrite(f, HIGH);
    }
    else if((f_nt - f_t >= 300000) && f_1 == 1)
    {
    f_1 = 0;
    f_t = millis();
    digitalWrite(f, LOW);
    }
  }
  else if(f_state == 1){
    if(f_1 == 0){
      f_1 = 1;
      digitalWrite(f, HIGH);
    }
  }
  else{
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
}
