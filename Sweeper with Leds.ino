#include<Servo.h>

/**
 * Simple class to manage led in a blynk system
 */
class LedMachine {
  int ledPin;
  long timeOn;
  long timeOff;
  String label;
  
  int ledState;
  unsigned long previousMillis;
  
public:
  /**
   * Constructor
   * 
   * @param pin:int - Pin number where the led is connected
   * @param t_on:long - Time in millis where the Led must be on
   * @param t_off:long - Time in millis where the Led must be off
   * @param name:String - Label or name of led, oftentimes the color of led.
   */
  LedMachine(int pin, long t_on, long t_off, String name) {
    this->ledPin        = pin;
    this->timeOn        = t_on;
    this->timeOff       = t_off;
    this->ledState      = LOW;
    this->previousMillis = 0;
    this->label          = name;
    
    pinMode(ledPin, OUTPUT);
  }

  /**
   * Update the state of led, will check the t_on and t_off attributes and change the state 
   */
  void Update() {
    /// Doc: https://www.arduino.cc/reference/pt/language/functions/time/millis/
    unsigned long currentMillis = millis();

    if((this->ledState == HIGH) && ((currentMillis - this->previousMillis) >= this->timeOn)) {
      this->ledState = LOW; 
      this->previousMillis = currentMillis;
      digitalWrite(this->ledPin, this->ledState);
    }
    else if((this->ledState == LOW) && ((currentMillis - this->previousMillis) >= this->timeOff)) {
      this->ledState = HIGH;
      this->previousMillis = currentMillis;
      digitalWrite(this->ledPin, this->ledState);
    }
  }
};

class Sweeper {
  Servo servo;
  int currentPosition;
  int increment;
  int updateInterval;
  unsigned long lastUpdate;

public:
  Sweeper(int interval) {
    this->updateInterval = interval;
    increment = 1;
  }

  void Attach(int pin) { this->servo.attach(pin); }
  void Detach() { this->servo.detach(); }

  void Update() {
    unsigned long currentMillis = millis();

    if((currentMillis - this->lastUpdate) > updateInterval) {
      this->lastUpdate = currentMillis;
      this->currentPosition += increment;
      this->servo.write(this->currentPosition);

      if((this->currentPosition >= 180) || (this->currentPosition <= 0)) this->increment = -this->increment;
    }
  }
};


LedMachine ledRed(8, 100, 400, "Vermelho");
LedMachine ledYellow(7, 350, 350, "Amarelo");
LedMachine ledGreen(6, 700, 300, "Verde");

Sweeper sweeper1(3);
Sweeper sweeper2(4);

  

void setup() {
  Serial.begin(11520);
  pinMode(LED_BUILTIN, OUTPUT);
  sweeper1.Attach(10);
  sweeper2.Attach(9);
}

void loop() {
  ledRed.Update();
  ledYellow.Update();
  ledGreen.Update();

  sweeper1.Update();
  sweeper2.Update();
}