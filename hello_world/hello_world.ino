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

int data=0;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int total = 0;
//Output from the sensor:
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

int color = 0;
int start = 0;

int Red=0, Blue=0, Green=0;  //RGB values 

void setup() {
  Wire.begin();

  display.begin();

  //Declarations:
   pinMode(S0, OUTPUT);
   pinMode(S1, OUTPUT);
   pinMode(S2, OUTPUT);
   pinMode(S3, OUTPUT);

   pinMode(out, INPUT);
   Serial.begin(9600);//begin serial communication

   digitalWrite(S0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
   digitalWrite(S1,LOW); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
   
//   calibrate();//calibrate sensor (look at serial monitor)

}

void myfunc() {
  if (start == 1) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Money count= ");
    display.print(total);
  }
  else if (start == 0) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Start counting now");
  }

  
 
}

void loop() {
  myfunc();
  display.display();

  GetColors();                                     //Execute the GetColors function to get the value of each RGB color
                                                   //Depending of the RGB values given by the sensor we can define the color and displays it on the monitor

  if (Red <=15 && Green <=15 && Blue <=15)         //If the values are low it's likely the white color (all the colors are present)
      Serial.println("White");                    
      
  else if (Red<Blue && Red<=Green && Red<23)      //if Red value is the lowest one and smaller thant 23 it's likely Red
      Serial.println("Red");

  else if (Blue<Green && Blue<Red && Blue<20)    //Same thing for Blue
      Serial.println("Blue");

  else if (Green<Red && Green-Blue<= 8)           //Green it was a little tricky, you can do it using the same method as above (the lowest), but here I used a reflective object
      Serial.println("Green");                    //which means the blue value is very low too, so I decided to check the difference between green and blue and see if it's acceptable

  else
     Serial.println("Unknown");                  //if the color is not recognized, you can add as many as you want


  delay(2000);                                   //2s delay you can modify if you want

//  digitalWrite(S2,LOW);        //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
//   digitalWrite(S3,LOW);
//   Serial.print("Red value= "); 
//   GetData();                   //Executing GetData function to get the value
//
//   digitalWrite(S2,LOW);
//   digitalWrite(S3,HIGH);
//   Serial.print("Blue value= ");
//   GetData();
//
//   digitalWrite(S2,HIGH);
//   digitalWrite(S3,HIGH);
//   Serial.print("Green value= ");
//   GetData();
//
//   Serial.println();
//
//   delay(2000);
  
//   readColor();//read sensor
//   decideColor();//format color values
//   printColor();//print values
}

void GetColors()  
{    
  digitalWrite(S2, LOW);                                           //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green 
  digitalWrite(S3, LOW);                                           
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);       //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again, if you have trouble with this expression check the bottom of the code
  delay(20);  
  digitalWrite(S3, HIGH);                                         //Here we select the other color (set of photodiodes) and measure the other colors value using the same techinque
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
  digitalWrite(S2, HIGH);  
  Green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
}

//void GetData(){
//   data=pulseIn(sensorOut,LOW);       //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again
//   Serial.print(data);          //it's a time duration measured, which is related to frequency as the sensor gives a frequency depending on the color
//   Serial.print("\t");          //The higher the frequency the lower the duration
//   delay(20);
//}

//void decideColor() {//format color values
// //Limit possible values:
// redColor = constrain(redColor, 0, 255);
// greenColor = constrain(greenColor, 0, 255);
// blueColor = constrain(blueColor, 0, 255);
//
// //find brightest color:
// int maxVal = max(redColor, blueColor);
// maxVal = max(maxVal, greenColor);
// //map new values
// redColor = map(redColor, 0, maxVal, 0, 255);
// greenColor = map(greenColor, 0, maxVal, 0, 255);
// blueColor = map(blueColor, 0, maxVal, 0, 255);
// redColor = constrain(redColor, 0, 255);
// greenColor = constrain(greenColor, 0, 255);
// blueColor = constrain(blueColor, 0, 255);
//
//// //light led
//// analogWrite(redLED, redColor);
//// analogWrite(greenLED, greenColor);
//// analogWrite(blueLED, blueColor);
// //decide which color is present (you may need to change some values here):
// if (redColor > 250 && greenColor > 250 && blueColor > 250) {
//   color = 1;//white
// }
// else if (redColor < 25 && greenColor < 25 && blueColor < 25) {
//   color = 2;//black
// }
// else if (redColor > 200 &&  greenColor > 200 && blueColor < 100) {
//   color = 4;//yellow
// }
// else if (redColor > 200 &&  greenColor > 25 /*&& blueColor < 100*/) {
//   color = 3;//orange
// }
// else if (redColor > 200 &&  greenColor < 100 && blueColor > 200) {
//   color = 5;//purple
// }
// else if (redColor > 250 && greenColor < 200 && blueColor < 200) {
//   color = 6;//red
// }
// else if (redColor < 200 && greenColor > 250 && blueColor < 200) {
//   color = 7;//green
// }
// else if (redColor < 200 /*&& greenColor < 200*/ && blueColor > 250) {
//   color = 8;//blue
// }
// else {
//   color = 0;//unknown
// }
//}

//void calibrate() {
// Serial.println("Calibrating...");
// Serial.println("White");//aim sensor at something white
// //set calibration vaues:
//
// digitalWrite(13, HIGH);
// delay(2000);
// digitalWrite(S2, LOW);
// digitalWrite(S3, LOW);
// redMin = pulseIn(sensorOut, LOW);
// delay(100);
// digitalWrite(S2, HIGH);
// digitalWrite(S3, HIGH);
// greenMin = pulseIn(sensorOut, LOW);
// delay(100);
// digitalWrite(S2, LOW);
// digitalWrite(S3, HIGH);
// blueMin = pulseIn(sensorOut, LOW);
// delay(100);
// Serial.println("next...");//aim sensor at something black
// digitalWrite(13, LOW);
// delay(2000);
// Serial.println("Black");
//
// //set calibration values:
// digitalWrite(13, HIGH);
// delay(2000);
// digitalWrite(S2, LOW);
// digitalWrite(S3, LOW);
// redMax = pulseIn(sensorOut, LOW);
// delay(100);
// digitalWrite(S2, HIGH);
// digitalWrite(S3, HIGH);
// greenMax = pulseIn(sensorOut, LOW);
// delay(100);
// digitalWrite(S2, LOW);
// digitalWrite(S3, HIGH);
// blueMax = pulseIn(sensorOut, LOW);
// delay(100);
// Serial.println("Done calibrating.");
// digitalWrite(13, LOW);
//}

//void printColor() {//print data
// Serial.print("R = ");
// Serial.print(redColor);
// Serial.print(" G = ");
// Serial.print(greenColor);
// Serial.print(" B = ");
// Serial.print(blueColor);
// Serial.print(" Color: ");
// switch (color) {
//   case 1: Serial.println("WHITE"); break;
//   case 2: Serial.println("BLACK"); break;
//   case 3: Serial.println("ORANGE"); break;
//   case 4: Serial.println("YELLOW"); break;
//   case 5: Serial.println("PURPLE"); break;
//   case 6: Serial.println("RED"); break;
//   case 7: Serial.println("GREEN"); break;
//   case 8: Serial.println("BLUE"); break;
//   default: Serial.println("unknown"); break;
// }
//}
//
//void readColor() {//get data from sensor
// //red:
// digitalWrite(S2, LOW);
// digitalWrite(S3, LOW);
// redFrequency = pulseIn(sensorOut, LOW);
// redColor = map(redFrequency, redMin, redMax, 255, 0);
// delay(100);
//
// //green:
// digitalWrite(S2, HIGH);
// digitalWrite(S3, HIGH);
// greenFrequency = pulseIn(sensorOut, LOW);
// greenColor = map(greenFrequency, greenMin, greenMax, 255, 0);
// delay(100);
//
// //blue:
// digitalWrite(S2, LOW);
// digitalWrite(S3, HIGH);
// blueFrequency = pulseIn(sensorOut, LOW);
// blueColor = map(blueFrequency, blueMin, blueMax, 255, 0);
// delay(100);
//}
