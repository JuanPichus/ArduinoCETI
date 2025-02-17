#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4;
const byte COLS = 4;
char key[ROWS][COLS] = {
  { '1', '2', '3', ':' },
  { '4', '5', '6', '#' },
  { '7', '8', '9', '#' },
  { '*', '0', '#', '#' }
};

byte rowPins[ROWS] = { 6, 7, 8, 9 };
byte colPins[COLS] = { 10, 11, 12, 13 };

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad kpd = Keypad(makeKeymap(key), rowPins, colPins, ROWS, COLS);

const int motor = 3;
char pressKey;
int iKey;

int acceleration, hold, deceleration;

void getOptions();

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(motor, OUTPUT);
  analogWrite(motor, 0);
}

void loop() {
  int i=1;
  float timeAce, timeDes;
  analogWrite(motor, 0);
  getOptions();

  lcd.clear();
  lcd.print("Aceleracion");
  timeAce = ((float)acceleration / (float)255) * 1000;
  while (i < 255) {
    lcd.setCursor(0, 1);
    lcd.print(i);
    analogWrite(motor, i);
    delay(timeAce);
    i++;
  }

  lcd.clear();
  lcd.print("Mantenimiento");
  delay(hold*1000);

  lcd.clear();
  lcd.print("Desaceleracion");
  timeDes = ((float)deceleration / (float)255) * 1000;
  while (i > 1) {
    lcd.setCursor(0, 1);
    lcd.print("   ");
    lcd.setCursor(0,1);
    lcd.print(i);
    analogWrite(motor, i);
    delay(timeDes);
    i--;
  }
  lcd.clear();
}

void getOptions () {
  lcd.print("A:");

  do{
    // kpd.waitForKey();
    pressKey = kpd.getKey();
    acceleration = pressKey - 48;
  }while(acceleration < 4 || acceleration > 10);
  lcd.print(acceleration);
  delay(1000);

  lcd.clear();
  lcd.print("M:");

  do{
    // kpd.waitForKey();
    pressKey = kpd.getKey();
    hold = pressKey - 48;
  }while(hold < 4 || hold > 10);
  lcd.print(hold);
  delay(1000);

  lcd.clear();
  lcd.print("D:");

  do{
    // kpd.waitForKey();
    pressKey = kpd.getKey();
    deceleration = pressKey - 48;
  }while(deceleration < 4 || deceleration > 10);
  lcd.print(deceleration);
  delay(1000);
}