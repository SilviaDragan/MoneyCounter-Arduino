#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define out 6
#define prox 8

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Output from the color sensor
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

//Formatted color values:
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

//Values used for calibration
int redMin;
int redMax;
int greenMin;
int greenMax;
int blueMin;
int blueMax;

int inputVal = 0;

int Red=0, Blue=0, Green=0;  //RGB values 

// total money counted
int total = 0;
int color = 0;
int start = 0;


int detectedBill = 0;
int billValue = 0;

void setup() {
  Wire.begin();

  display.begin();

  //Declarations:
   pinMode(S0, OUTPUT);
   pinMode(S1, OUTPUT);
   pinMode(S2, OUTPUT);
   pinMode(S3, OUTPUT);
   pinMode(out, INPUT);
   pinMode(prox,INPUT);    //Pin 8 is connected to the output of proximity sensor

   Serial.begin(9600);//begin serial communication
   
   digitalWrite(S0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
   digitalWrite(S1,LOW); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
}

void loop() {
  display.display();
  
  if (start == 0) {
   Serial.println("start");
   writeOnDisplayStart(); 
  }
   
  checkProximitySensor();

   if (detectedBill == 1) {
      Serial.println("am detectat");
      start = 1;
      delay(2000);
      readColorsFromSensor();
      detectBill();
      delay(1000);
      detectedBill = 0;  
   }

   if (detectedBill == 0 && start == 1) {
//       Serial.println("scriu totalul");
       total += billValue;
       billValue = 0;
//       Serial.print("totalul e ");
//       Serial.println(total);
       
       writeOnDisplayTotal();
       delay(1500);  
   }

//  printColors();
}

void checkProximitySensor() {
  inputVal = digitalRead(prox);
  if(inputVal==HIGH)      //Check the sensor output
  {  
    digitalWrite(13, LOW);    // set the LED off
  }
  else
  {
    digitalWrite(13, HIGH);   // set the LED on
    detectedBill = 1;
  }
}


void writeOnDisplayStart() {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Insert money to count");
    delay(3000);
}

void readColorsFromSensor() {
 //red:
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);
 redFrequency = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
// redColor = map(redFrequency, redMin, redMax, 255, 0);
 delay(100);

 //green:
 digitalWrite(S2, HIGH);
 digitalWrite(S3, HIGH);
 greenFrequency = pulseIn(out,  digitalRead(out) == HIGH ? LOW : HIGH);
// greenColor = map(greenFrequency, greenMin, greenMax, 255, 0);
 delay(100);

 //blue:
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 blueFrequency = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
 delay(100);
}

void detectBill() {
 Serial.print("R = ");
 Serial.print(redFrequency);
 Serial.print(" G = ");
 Serial.print(greenFrequency);
 Serial.print(" B = ");
 Serial.print(blueFrequency);
 Serial.println();

// 10 euro
 if (   40 <= redFrequency && redFrequency < 70
    &&  75 < greenFrequency && greenFrequency < 105 
    &&  75 < blueFrequency && blueFrequency < 100
    ) {
   billValue = 10;
   Serial.println("10 euro");
 }

}

void writeOnDisplayBillValue() {
    Serial.println("valoarea bancnotei");
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Bill Value= ");
//    display.print(billValue);
    delay(2000);
}


void writeOnDisplayTotal() {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Money count= ");
    display.print(total);
}

void printColors() {
 Serial.print("R = ");
 Serial.print(redFrequency);
 Serial.print(" G = ");
 Serial.print(greenFrequency);
 Serial.print(" B = ");
 Serial.print(blueFrequency);
 Serial.println();

 delay(2000);
}
