#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,20,4);
Servo servoL;
Servo servoR;
Servo servoT1;
Servo servoT2;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

int buttonPin = 12;
float length = 0;
float height = 0;

char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

byte rowPins[ROWS] = {8, 7, 13, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  lcd.init();
  servoL.attach(9);
  servoR.attach(10);
  servoT1.attach(6);
  servoT2.attach(11);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  servoL.write(0);
  servoR.write(180);
  servoT1.write(180);
  
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("====================");
  lcd.setCursor(7, 1); lcd.print("Hello,");
  lcd.setCursor(7, 2); lcd.print("KUSMP");
  lcd.setCursor(0, 3); lcd.print("====================");
  delay(2000);
  lcd.noBacklight();
  lcd.clear();
}

void loop()
{
  servoT1.write(180);
  lcd.clear();
  lcd.backlight();
  getLength();
  getHeight();
  float angle = atan(height/length) * 57.2957795131;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Angle : ");
  lcd.setCursor(10, 0); lcd.print("----------");
  lcd.setCursor(9, 0); lcd.print("|");
  lcd.setCursor(0, 1); lcd.print(angle);
  lcd.setCursor(9, 1); lcd.print("|");
  lcd.setCursor(10, 1); lcd.print("READY TO");
  lcd.setCursor(9, 2); lcd.print("|");
  lcd.setCursor(10, 2); lcd.print("FIRE");
  lcd.setCursor(10, 3); lcd.print("__________");
  lcd.setCursor(9, 3); lcd.print("|");
  Serial.println(angle);
  servoL.write(angle);
  servoR.write(180 - angle);
  while (true) {
    char key;
    key = keypad.getKey();
    Serial.println(digitalRead(buttonPin));
    if(digitalRead(buttonPin) == 1){
      servoT1.write(0);
      servoT2.writeMicroseconds(2500);
      delay(1850);
      servoT2.writeMicroseconds(1500);
      
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("====================");
      lcd.setCursor(7, 1); lcd.print("FIRED");
      lcd.setCursor(7, 2); lcd.print("!!!");
      lcd.setCursor(0, 3); lcd.print("====================");
      delay(3000);
      break; 
    } 
    if(key == '*') {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("====================");
      lcd.setCursor(7, 1); lcd.print("ABORTED");
      lcd.setCursor(9, 2); lcd.print("###");
      lcd.setCursor(0, 3); lcd.print("====================");
      delay(3000);
      break;
    }
  }
}


void getLength() {
  length = 0; // Reset input number
  char key;
  lcd.setCursor(0, 0);
  lcd.print("Length : ");
  while (true) {
    key = keypad.getKey(); // Get the key pressed
    if (key != NO_KEY) {
      if (key == '#') {
        // Submit the number when '#' is pressed
        break;
      } else if (key >= '0' && key <= '9') {
        // Append the digit to the number
        length = length * 10 + (key - '0');
      }
      lcd.setCursor(0, 1);
      lcd.print(length);
      Serial.println(length);
    }
  }
}

void getHeight() {
  height = 0; // Reset input number
  char key;
  lcd.setCursor(0, 2);
  lcd.print("Height : ");
  while (true) {
    key = keypad.getKey(); // Get the key pressed
    if (key != NO_KEY) {
      if (key == '#') {
        // Submit the number when '#' is pressed
        break;
      } else if (key >= '0' && key <= '9') {
        // Append the digit to the number
        height = height * 10 + (key - '0');
      }
      lcd.setCursor(0, 3);
      lcd.print(height);
      Serial.println(height);
    }
  }
}