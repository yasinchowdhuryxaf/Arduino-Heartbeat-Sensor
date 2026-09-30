#define USE_ARDUINO_INTERRUPTS true   // Set-up low-level interrupts for most accurate BPM math.
#include <PulseSensorPlayground.h>   // Includes the PulseSensorPlayground Library.
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal.h>

// F Malpartida's NewLiquidCrystal library
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, ct = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables
const int PulseWire = 0;  // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;    // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;    // Determine which Signal to "count as a beat" and which to ignore.
                       // Use the "Getting Started Project" to fine-tune Threshold Value beyond the default setting.
                      // Otherwise, leave the default "550" value.
                      
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
SoftwareSerial bt(0, 1);           // (Rx, Tx)

void setup() {
  analogWrite(ct, 50); // Adjust this pin if needed for PWM

  lcd.begin(16, 2);  
  lcd.print("Heart Rate");
  lcd.setCursor(0, 1);
  lcd.print("Monitor");
  
  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);  // Auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  
  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  // This prints one time at Arduino power-up, or on Arduino reset.
  }

  bt.begin(9600); // Initialize Bluetooth communication
  Serial.begin(9600);
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
  // "myBPM" holds this BPM value now.
  if (pulseSensor.sawStartOfBeat()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HeartBeat: ");  // If test is "true", print a message "a heartbeat happened".
    lcd.setCursor(0, 1);
    lcd.print(myBPM);
    lcd.print(" BPM");
    
    // Print BPM value over Bluetooth
    Serial.print("BPM: ");
    Serial.println(myBPM);
  }

  delay(1000);  // Considered best practice in a simple sketch.
}
