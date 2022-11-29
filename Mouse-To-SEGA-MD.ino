/* Подключение PS/2 мыши к SEGA MD вместо штатного геймпада

   (C)HotPixel, 2022
   https://www.youtube.com/hotpixel

  **********************
  Пины Arduino для геймпада SEGA MD
  2 - UP
  3 - DOWN
  4 - LEFT
  5 - RIGHT  
  6 - A/B
  7 - START/C
  8 - Select
  GND и +5 ардуины соединить с соответствующими пинами геймпада
*/

#include "PS2MouseHandler.h"
#include "Gamepad.h"

// Пины мыши
#define MOUSE_DATA A5
#define MOUSE_CLOCK A4

#define SENS 3  // Чувствительность срабатывания перемещения мыши

PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_STREAM);
Gamepad pad = Gamepad();
Gamepad::PadState padState;



long lastTime;


/**
   Setup
*/
void setup() {
  mouse.initialise();
  mouse.set_remote_mode();  // Включаем ROMOTE режим на мышке, чтоб забирать данные только по нашему требованию
}


void loop() {

  // Читаем данные с мыша с периодом +-5мкс
  if (millis() - lastTime > 8) {
    mouse.get_data();
    lastTime = millis();

    padState.UP = mouse.y_movement() > SENS;
    padState.DOWN = mouse.y_movement() < 0 - SENS;
    padState.LEFT = mouse.x_movement() < 0 - SENS;
    padState.RIGHT = mouse.x_movement() > SENS;

    padState.A = mouse.button(0) ? 1 : 0;
    padState.B = mouse.button(1) ? 1 : 0;
    padState.C = mouse.button(2) ? 1 : 0;

    padState.START = mouse.z_movement() < 0 ? 1 : 0;
    padState.MODE = mouse.z_movement() > 0 ? 1 : 0;
    if (mouse.z_movement() > 0)
      pad.toggleMode();
    
    pad.writeData(&padState);
  }



}
