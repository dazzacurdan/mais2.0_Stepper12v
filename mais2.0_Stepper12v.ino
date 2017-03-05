/*
 * M@is 2.0
 * 
 *    WIRING:
 * WHITE:   Coin data   2   
 * GREEN:    Relè data   4
 * YELLOW:   Relè data   5   
 * RED:     +5
 * BLACK:   GND
 * 
 */

#include <Servo.h>

Servo myservo;

struct TIME
{
  int servo;
  long cottura;
  int _delay;
};

struct PIN
{
  int rele0;
  int rele1;
  byte interrupt0;
};

static const float targetValue = 2.0; 

struct COIN
{
  volatile float value;
  volatile bool isChanged;
};

TIME time_t;                   
PIN pin;
COIN coin;

void setup()
{
  time_t = {4000,180000,20};
  pin = {4,5,2};
  resetCoin();
  
  Serial.begin(9600);                 
  
  pinMode(pin.interrupt0, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin.interrupt0), coinInserted, RISING); 
  
  pinMode(pin.rele0, OUTPUT);
  digitalWrite(pin.rele0, HIGH);

  pinMode(pin.rele1, OUTPUT);
  digitalWrite(pin.rele1, HIGH);
  
}

void coinInserted()    
//The function that is called every time it recieves a pulse
{
  if(!coin.isChanged)
  {
    coin.value = coin.value + 0.50;
    if(coin.value == targetValue)
    {
      //As we set the Pulse to represent 5p or 5c we add this to the coinsValue
      coin.isChanged = true;                           
      //Flag that there has been a coin inserted
    }  
  }
  Serial.print("Coins insered: ");
  Serial.println(coin.value);
}

void resetCoin()
{
  coin = {0.00, false};
}

void loop()
{
  if(coin.isChanged)
  {  
    Serial.print("Credit: £");
    Serial.println(coin.value);
    
    digitalWrite(pin.rele0, LOW);  
    delay(time_t.servo);  
    digitalWrite(pin.rele0, HIGH);
    delay(time_t._delay);
    
    digitalWrite(pin.rele1, LOW);  
    delay(time_t.cottura);  
    digitalWrite(pin.rele1, HIGH);
    delay(time_t._delay);
    Serial.println("FINISH");
    
    resetCoin();
    Serial.print("isChanged: ");
    Serial.println(coin.isChanged);
  }
}
