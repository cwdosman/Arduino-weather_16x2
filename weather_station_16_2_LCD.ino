/*
  Wetterstation mit 16x2 LCD mit I2C-Backpack
  Entwickelt von Michael Kuhn
  Arduino Uno R3:
  Display Anschluss: VCC-5v, GND-GND, SDA-A4, SCL-A5 , oder parallel zu BME280
  BME280 Anschluss: VIN-5v, GND-GND, SCK-SCL(od.A5), SDI-SDA(od.A4)
*/

//--------------------------------------------------------------------------------------------

#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//--------------------------------------------------------------------------------------------

LiquidCrystal_I2C lcd(0x27, 16, 2); //setzt Adresse des LCd zu 0x27 und deffiniert LCD Grösse.
Adafruit_BME280 bme; // I2C
unsigned long previousMillis = 0;
unsigned long previousbacklight = 0;
long interval = 10000; //Interval für Temp. auslesen
long backLightoff = 30000; //Backlight-Zeit
const int lightbutton = 7; //Button für LCD-Backlight an
const int buttonLED = 6; //ButtonLED (LOW= LED an / HIGH= LED aus
//const int chipSelect = 4; //mögliche SD-Karte für Datalogging

//--------------------------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600); //für debug only
  pinMode(lightbutton, INPUT);
  pinMode(buttonLED, OUTPUT);
  digitalWrite(buttonLED, HIGH);
  lcd.init();       //LCD Initiiren
  lcd.backlight();  //LCD-Backlight an
  unsigned long currentMillis = millis();
  bool status;

  status = bme.begin();
  if (!status) {
    lcd.setCursor(0, 0);
    lcd.print("Could not find");
    lcd.setCursor(0, 1);
    lcd.print("valid Temp Sens.");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("Angela's");
  lcd.setCursor(0, 1);
  lcd.print("Vogelwetter");
  lcd.setCursor(0, 1);

  delay(10000);
  lcd.clear();
  digitalWrite(lightbutton, HIGH);
}

//--------------------------------------------------------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:
  printData();
  backLightonoff();
}

void printData() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    lcd.setCursor(0, 0);
    lcd.print("Temp.= ");
    lcd.print(bme.readTemperature()- 1.00); // -1 Grad (passt besser)
    lcd.print("*C");

    lcd.setCursor(0, 1);
    lcd.print("Druck= ");
    lcd.print(bme.readPressure() / 100.0F);
    lcd.print("hPa");
    previousMillis = currentMillis;
  }
}

//--------------------------------------------------------------------------------------------

void backLightonoff() {
  unsigned long backlightMillis = millis();
  int lightbuttonState = digitalRead(lightbutton);


  if (backlightMillis - previousbacklight >= backLightoff ) {
    lcd.noBacklight(); //LCD-Backlight aus
    lcd.noDisplay(); //LCD OFF
    digitalWrite(buttonLED, LOW);
    }
  if (lightbuttonState == LOW) {
    //Serial.println(lightbuttonState); //für debug
    lcd.backlight(); //LCD-Backlight an
    lcd.display(); //LCD ON
    digitalWrite(buttonLED, HIGH);
    previousbacklight = backlightMillis;
  }
}

//--------------------------------------------------------------------------------------------
