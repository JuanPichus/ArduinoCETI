#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Inicializando variables
const byte ROWS = 4;
const byte COLS = 4;
char key[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 6, 7, 8, 9 };
byte colPins[COLS] = { 10, 11, 12, 13 };

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad kpd = Keypad(makeKeymap(key), rowPins, colPins, ROWS, COLS);

const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

 int secuenciaMedio[ 8 ][ 4 ] =
    {  {1, 0, 0, 0},
       {1, 1, 0, 0},
       {0, 1, 0, 0},
       {0, 1, 1, 0},
       {0, 0, 1, 0},
       {0, 0, 1, 1},
       {0, 0, 0, 1},
       {1, 0, 0, 1}
     };

//vuelta con 8 medios pasos = 8 * 64 = 512

int secuenciaCompleto[4][4] = {
  { 1, 0, 0, 0 },
  { 0, 1, 0, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 0, 1 }
};

//vuelta con 4 pasos = 4 * 64 = 256

int nPaso = 0;
int totalPasos;

int direccion = 0; // 1 = horario, 2 = anti horario
int tipoPaso = 0; // 1 = Completo, 2 = medio

void getDireccion();
void getPaso();

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  direccion = 0;
  tipoPaso = 0;
  nPaso = 0;
  // put your main code here, to run repeatedly:

  getPaso();
  getDireccion();
  lcd.clear();

  if (tipoPaso == 1) {
    if (direccion == 1) {
      totalPasos = 0;
      while (totalPasos < 4096) {
          pasoCompleto(nPaso);
          delay(8);
          nPaso++;
          totalPasos++;
          
          if (nPaso > 3) {
              nPaso = 0;
          }
      }
    }
    else {
      totalPasos = 4096;
      while (totalPasos > 0) {
        pasoCompleto(nPaso);
        delay(8);
        nPaso--;
        totalPasos--;

        if (nPaso < 1) {
          nPaso = 3;
        }
      }
    }
  }
  else {
    if (direccion == 1) {
      totalPasos = 0;
      while(totalPasos < 4096) {
        pasoMedio(nPaso);
        delay(1);
        nPaso++;
        totalPasos++;

        if (nPaso > 7) {
          nPaso = 0;
        }
      }
    }
    else {
      totalPasos = 4096;
      while (totalPasos > 0) {
        pasoMedio(nPaso);
        delay(1);
        nPaso--;
        totalPasos--;

        if (nPaso < 1) {
          nPaso = 7;
        }
      }
    }
  }
}

void pasoCompleto(int nPaso) {
  digitalWrite(IN1, secuenciaCompleto[nPaso][0]);
  digitalWrite(IN2, secuenciaCompleto[nPaso][1]);
  digitalWrite(IN3, secuenciaCompleto[nPaso][2]);
  digitalWrite(IN4, secuenciaCompleto[nPaso][3]);
}

void pasoMedio(int nPaso) {
  digitalWrite(IN1, secuenciaMedio[nPaso][0]);
  digitalWrite(IN2, secuenciaMedio[nPaso][1]);
  digitalWrite(IN3, secuenciaMedio[nPaso][2]);
  digitalWrite(IN4, secuenciaMedio[nPaso][3]);
}

void getDireccion () {
  char ckey;
  lcd.clear();
  lcd.print("1. horario");
  lcd.setCursor(0, 1);
  lcd.print("2. Anti Horario");
  delay(2000);
  lcd.clear();
  lcd.print("Opcion:");
  do {
    ckey = kpd.getKey();
    if (ckey) {
      direccion = ckey - 48;
    }
  }while (direccion != 1 && direccion != 2);
  lcd.print(direccion);
  delay(1000);
}

void getPaso () {
  char ckey;
  lcd.clear();
  lcd.print("1. Completo");
  lcd.setCursor(0, 1);
  lcd.print("2. Medio");
  delay(3000);
  lcd.clear();
  lcd.print("Opcion:");
  do {
    ckey = kpd.getKey();
    if (ckey) {
      tipoPaso = ckey - 48;
    }
  }while (tipoPaso != 1 && tipoPaso != 2);
  lcd.print(tipoPaso);
  delay(1000);
}
