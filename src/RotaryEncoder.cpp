#include <RotaryEncoder.h>
#include <Definitions.h>
#include <Encoder.h>
#include <JC_Button.h>

RotaryEncoder::RotaryEncoder() {
    button = new Button(ENC_SW, 100, true, true);
    encoder = new Encoder(ENC_CLK, ENC_DT);
    button->begin();
}

bool RotaryEncoder::wasTurnedLeft() {
    if (encoder->read() < 0 - ENC_TOL) {
        encoder->write(0);
        return true;
    }
    return false;
}

bool RotaryEncoder::wasTurnedRight() {
    if (encoder->read() > 0 + ENC_TOL) {
        encoder->write(0);
        return true;
    }
    return false;
}

bool RotaryEncoder::isPressed() {
    button->read();
    if (button->isPressed()) {
        return true;
    }
    return false;
}

bool RotaryEncoder::isReleased() {
    button->read();
    if (button->isReleased()) {
        return true;
    }
    return false;
}

bool RotaryEncoder::wasPressed() {
    if (!wasLongPressed() && button->isPressed()) {
        return true;
    }
    return false;
}

bool RotaryEncoder::wasLongPressed() {
    button->read();
    if (button->pressedFor(ENC_SW_LONG_PRESS_DUR)) {
        return true;
    }
    return false;
}