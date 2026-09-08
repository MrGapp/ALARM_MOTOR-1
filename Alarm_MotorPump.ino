#include <Wire.h>
#include<EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 
RTC_DS3231 RTC;
int tmp,Inc,hor,mIn,add=11; 
int set=8;  
int cge=9;   
int mod=10; 
int off=0;
int motorA1 = 3;
int motorA2 = 5;
int motorB1 = 6;
int motorB2 = 11;
int pump = 12;
int Hor,Min,Sec; 
///////////////////////////////////////Function to adjust the time//////////////////////////////////
void time()                             
{
    int tmp=1,mins=0,hors=0,secs=0;
    while(tmp==1)
    {
     off=0;
     if(digitalRead(cge)==0)
     {
      Hor++;
      if(Hor==24)
      {
       Hor=0;
      }
     }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Set Alarm Time ");

    lcd.setCursor(0,1);
    if(Hor<=9)
    lcd.print("0");
    lcd.print(Hor);
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);
    lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);
    if(digitalRead(set)==0)
    {
      hor=Hor;
      EEPROM.write(add++,hor);
     tmp=2;
     while(digitalRead(set)==0);
    }
    }
    
    while(tmp==2)
    {
     if(digitalRead(cge)==0)
     {
      Min++;
      if(Min==60)
      {Min=0;}
     }
    lcd.setCursor(0,1);
    lcd.print(Hor);
    lcd.print(":");
     if(Min<=9)
    lcd.print("0");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);
     lcd.setCursor(0,1);
    lcd.print(Hor);
    lcd.print(":");
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);
      if(digitalRead(set)==0)
      {
       mIn=Min;
       EEPROM.write(add++, mIn);
       tmp=0;
       while(digitalRead(set)==0);
      }
    }
    off=1;
    delay(10);
}
///////////////////////////////////////////function to sound the MotorA1zer//////////////////////////////////
void MotorA1()
{
  if(digitalRead(set)==0)
  off=0;
  if(off==1)
  {
    digitalWrite(pump, HIGH);
    delay(25000);

    digitalWrite(pump, LOW);
    delay(25000);

    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    delay(50000);

    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    delay(50000);

    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
  }
}
//////////////////////////////////////////function to compare the alarm time with current RTC time//////////////////
void TimeCheck()
{
  int tem[17];
  for(int i=11;i<17;i++)
  {
    tem[i]=EEPROM.read(i);
  }
  if(Hor == tem[11] && Min == tem[12] && off==1) 
  {
   add=11;
   MotorA1();
   MotorA1();
   lcd.clear();
   lcd.print("alarm...........");
   lcd.setCursor(0,1);
   lcd.print("...........alarm");
   MotorA1();
   MotorA1();
    }
}
////////////////////////////////////////////////////////////setup///////////////////////////
void setup()
{
 Wire.begin();
 RTC.begin();
 lcd.init();
 lcd.backlight();
 pinMode(cge, INPUT);
 pinMode(set, INPUT);
 pinMode(mod, INPUT);
 pinMode(motorA1, OUTPUT);
 pinMode(motorA2, OUTPUT);
 pinMode(pump, OUTPUT);
 digitalWrite(motorA1, LOW);
 digitalWrite(motorA2, LOW);
 digitalWrite(set, HIGH);
 digitalWrite(mod, HIGH);
 digitalWrite(cge, HIGH);
 
   lcd.setCursor(0,0);
   lcd.print("ELECTRONICS HUB");
   lcd.setCursor(0,1);
   lcd.print("  Alarm Clock  ");
   delay(2000);
    
// RTC.adjust(DateTime(__DATE__,__TIME__));
}
////////////////////////////////////////////////////////////loop/////////////////////////////////////
void loop()
{
   DateTime now = RTC.now();
   if(digitalRead(mod) == 0)      
   { 
    current();
    time();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Alarm On");
    delay(2000);
   }
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Time:");
 lcd.setCursor(6,0);
 Hor=now.hour(),DEC; 
 if(Hor<=9)
 {
  lcd.print("0");
  lcd.print(Hor=now.hour(),DEC);
 }
 else
 lcd.print(Hor=now.hour(),DEC);
 lcd.print(":");
 Min=now.minute(),DEC;
 if(Min<=9)
 {
 lcd.print("0");
 lcd.print(Min=now.minute(),DEC);
 }
 else
 lcd.print(Min=now.minute(),DEC);
 
 lcd.print(":");
 Sec=now.second(),DEC;
 if(Sec<=9)
 {
  lcd.print("0");
  lcd.print(Sec=now.second(),DEC);
 }
 else
 lcd.print(Sec=now.second(),DEC);
 lcd.setCursor(0,1);
 lcd.print("Date: ");
 lcd.print(now.day(),DEC);
 lcd.print("/");
 lcd.print(now.month(),DEC);
 lcd.print("/");
 lcd.print(now.year(),DEC);
 TimeCheck();
 delay(200);
}
///////////////////////////////////////////////////////function to get current RTC time//////////////////////////////
void current()
{
  lcd.setCursor(0,1);
  lcd.print(Hor);
  lcd.print(":");
  lcd.print(Min);
  lcd.print(":");
  lcd.print(Sec);
}