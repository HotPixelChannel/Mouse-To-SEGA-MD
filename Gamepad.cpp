#include "HardwareSerial.h"
#include "Gamepad.h"
#include "Arduino.h"

bool isSix = false;
long mils = 0;
short state = 0;

Gamepad::Gamepad() {
  /*
  2 - UP
  3 - DOWN
  4 - LEFT
  5 - RIGHT  
  6 - A/B 
  7 - START/C
  8 - Select
  */
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);

}


void Gamepad::reset(PadState *data) {
  data->UP = 0;
  data->DOWN = 0;
  data->LEFT = 0;
  data->RIGHT = 0;
  /* data->A = 0;
  data->B = 0;
  data->C = 0;
  data->START = 0;*/
}

void Gamepad::writeData(PadState *data) {

  if (isSix) {
    state = 0;
    waitSelect(1);

    bool done = false;
    while (!done) {
      data->START ? writeLo(7) : writeHi(7);
      data->A ? writeLo(6) : writeHi(6);


      switch (state) {
        case 0:
          waitSelect(0);
          data->B ? writeLo(6) : writeHi(6);
          data->C ? writeLo(7) : writeHi(7);
          waitSelect(1);
          writeHi(6);
          writeHi(7);
          state = 1;

        case 1:
          waitSelect(1);
          data->START ? writeLo(7) : writeHi(7);
          data->A ? writeLo(6) : writeHi(6);
          waitSelect(0);

          writeHi(6);
          writeHi(7);
          waitSelect(1);

          state = 2;
          break;

        case 2:
          writeHi(2);
          writeHi(3);
          writeHi(4);
          writeHi(5);

          waitSelect(0);
          data->B ? writeLo(6) : writeHi(6);
          data->C ? writeLo(7) : writeHi(7);

          waitSelect(1);
          writeHi(6);
          writeHi(7);
          done = true;
          break;
      }
    }

    data->UP ? writeLo(2) : writeHi(2);
    data->DOWN ? writeLo(3) : writeHi(3);
    data->LEFT ? writeLo(4) : writeHi(4);
    data->RIGHT ? writeLo(5) : writeHi(5);
    data->B ? writeLo(6) : writeHi(6);
    data->C ? writeLo(7) : writeHi(7);

  } else {

    waitSelect(1);
    mils = micros();
    data->B ? writeLo(6) : writeHi(6);
    data->C ? writeLo(7) : writeHi(7);

    waitSelect(0);
    data->UP ? writeLo(2) : writeHi(2);
    data->DOWN ? writeLo(3) : writeHi(3);
    data->LEFT ? writeLo(4) : writeHi(4);
    data->RIGHT ? writeLo(5) : writeHi(5);
    data->START ? writeLo(7) : writeHi(7);
    data->A ? writeLo(6) : writeHi(6);
  
  }
}


void Gamepad::toggleMode() {
  isSix = !isSix;
 /* if (isSix)
    Serial.println("Set 6");
  else
    Serial.println("Set 3");*/
}

// DigitalRead() работает очень тормознуто, юзаем прямой доступ к портам

void Gamepad::waitSelect(int state) {
  if (state > 0)
    while ((PINB & (1 << PINB0))) {}
  else
    while ((PINB & (1 << PINB0)) != 1) {}
}


bool Gamepad::isSelect() {
  return (PINB & (1 << PINB0)) == 1;
}

void Gamepad::writeLo(unsigned int port) {
  PORTD &= ~(1 << port);
}

void Gamepad::writeHi(unsigned int port) {
  PORTD |= (1 << port);
}
