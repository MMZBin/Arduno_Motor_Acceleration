/* MOTOR_Bピンの操作(逆転)は実装されていません。 */

#include "Timer.h"
#include <Joystick.h>

Timer timer;
Joystick joy(A0, A1, A2);

const uint8_t ACCELERATION = 5;

const uint8_t MOTOR_A = 3;
const uint8_t MOTOR_B = 5;
const uint8_t ENABLE =  6;

const uint8_t LED_SLOW =   8;
const uint8_t LED_MIDDLE = 9;
const uint8_t LED_FAST =  10;

void setup() {
    pinMode(MOTOR_A, OUTPUT);
    pinMode(MOTOR_B, OUTPUT);
    pinMode(ENABLE,  OUTPUT);

    pinMode(LED_SLOW,   OUTPUT);
    pinMode(LED_MIDDLE, OUTPUT);
    pinMode(LED_FAST,   OUTPUT);

    timer.setInterval(150);
    joy.calibrate();
}

void loop() {
    static int16_t currentSpeed = 0;
    //uint8_t targetSpeed = map(joy.getDistance(), 0, 181, 0, 255);
    uint8_t targetSpeed = 0;
    uint16_t angle = joy.getAngle();

    digitalWrite(ENABLE, HIGH);

    if (joy.getDirection() == Joystick::Dir::CENTER) { targetSpeed = 0; }
    else { targetSpeed = 255; }

    if (timer.hasOccurred()) {
        if (targetSpeed > currentSpeed) {
            currentSpeed += ACCELERATION;
            if (currentSpeed > 255) { currentSpeed = 255; }
            if (currentSpeed > targetSpeed) { currentSpeed = targetSpeed; }
        } else {
            currentSpeed -= ACCELERATION;
            if (currentSpeed < 0) { currentSpeed = 0; }
            if (currentSpeed < targetSpeed) { currentSpeed = targetSpeed; }
        }

    }

    if (currentSpeed <= 100) {
        led(true, false, false);
    } else if (currentSpeed <= 200) {
        led(false, true, false);
    } else {
        led(false, false, true);
    }

    timer.start();

    analogWrite(MOTOR_A, currentSpeed);
}

inline void led(const bool slow, const bool middle, const bool fast) {
    digitalWrite(LED_SLOW, slow);
    digitalWrite(LED_MIDDLE, middle);
    digitalWrite(LED_FAST, fast);
}
