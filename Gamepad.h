#ifndef Gamepad_h

#define Gamepad_h

class Gamepad {


public:
  typedef struct {
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
    bool A;
    bool B;
    bool C;
    bool START;
    bool MODE;
  } PadState;

  Gamepad();

  int getData();
  void writeData(PadState *);
  int isWritting();
  void reset(PadState *);
  void toggleMode();



private:
  int _port;
  PadState dataPad;
  void writeLo(unsigned int);
  void writeHi(unsigned int);
  bool isSelect();
  void waitSelect(int);
};



#endif Gamepad_h
