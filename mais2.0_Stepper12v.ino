/*
 * M@is 2.0
 * 
 *    WIRING:
 * WHITE:   Coin data   2   
 * GREEN:    Relè data   4
 * YELLOW:   Relè data   5   
 * LED       Red 7 Green 8 Blue 9
 * RED:     +5
 * BLACK:   GND
 * 
 */

#include <Servo.h>

Servo myservo;

struct TIME
{
  long servo;
  long cottura;
  long _delay;
  long cooling;
};

struct PIN
{
  int rele0;
  int rele1;
  byte interrupt0;
};

struct LED
{
  int red;
  int green;
  int blue; 
};

static const float TARGET_VALUE = 4.0; 

struct COIN
{
  volatile float value;
  volatile bool isChanged;
};

TIME time_t;                   
PIN pin;
COIN coin;
LED led;

void setGreenLed(LED led)
{
  analogWrite( led.red, 0 );
  analogWrite( led.green, 0 );
  analogWrite( led.blue, 255 );
}

void setRedLed(LED led)
{
  analogWrite( led.red, 255 );
  analogWrite( led.green, 0 );
  analogWrite( led.blue, 0 );
}

void setBluLed(LED led)
{
  analogWrite( led.red, 0 );
  analogWrite( led.green, 255 );
  analogWrite( led.blue, 0 );
}

void switchOffLed(LED led)
{
  analogWrite( led.red, 0 );
  analogWrite( led.green, 0 );
  analogWrite( led.blue, 0 );
}

void setup()
{
  time_t = {3600,180000,20,60000};
  pin = {4,5,2};
  led ={9,10,11};
  
  resetCoin();
  
  Serial.begin(9600);                 
  
  pinMode(pin.interrupt0, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin.interrupt0), coinInserted, RISING); 
  
  pinMode(pin.rele0, OUTPUT);
  digitalWrite(pin.rele0, HIGH);

  pinMode(pin.rele1, OUTPUT);
  digitalWrite(pin.rele1, HIGH);

  pinMode(led.red, OUTPUT);
  pinMode(led.green, OUTPUT);
  pinMode(led.blue, OUTPUT);
  analogWrite( led.green , 255 );
  setGreenLed(led);
}

void coinInserted()    
//The function that is called every time it recieves a pulse
{
  if(!coin.isChanged)
  {
    coin.value = coin.value + 0.50;
    if(coin.value == TARGET_VALUE)
    {
      //As we set the Pulse to represent 5p or 5c we add this to the coinsValue
      coin.isChanged = true;                           
      //Flag that there has been a coin inserted
    }  
  }
  //Serial.print("Coins insered: ");
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

    setBluLed(led);
    
    digitalWrite(pin.rele0, LOW);  
    delay(time_t.servo);  
    digitalWrite(pin.rele0, HIGH);
    delay(time_t._delay);
    
    digitalWrite(pin.rele1, LOW);  
    delay(time_t.cottura);  
    digitalWrite(pin.rele1, HIGH);
    delay(time_t._delay);
    
    Serial.println("COOLING");
    setRedLed(led);
    //digitalWrite(pin.rele0, LOW);  
    delay(time_t.cooling);  
    //digitalWrite(pin.rele0, HIGH);
    setGreenLed(led);
    
    resetCoin();
    Serial.print("isChanged: ");
    Serial.println(coin.isChanged);

    Serial.println("FINISH");
  }
}
