#pragma once

class Button;
class Encoder;

class RotaryEncoder
{
  private:
    Button *button;
    Encoder *encoder;
  
  public:
    RotaryEncoder();
    bool wasTurnedLeft();
    bool wasTurnedRight();
    bool isPressed();
    bool isReleased();
    bool wasPressed();
    bool wasLongPressed();
};