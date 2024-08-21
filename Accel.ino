/* MOTOR_Bピンの操作(逆転)は実装されていません。 */
/* L293D */

#include "Timer.h"
#include <Joystick.h>

Timer timer;
Joystick joy(A0, A1, A2);

const uint8_t ACCELERATION = 5; //加速スピード

const uint8_t MOTOR_A = 3;      //ICのA端子
const uint8_t MOTOR_B = 5;      //ICのB端子
const uint8_t ENABLE =  6;      //ICのENABLE端子

const uint8_t LED_SLOW =   8;   //遅いときのLED
const uint8_t LED_MIDDLE = 9;   //中間のときのLED
const uint8_t LED_FAST =  10;   //早いときのLED

enum class Speed : uint8_t {
    SLOW =   0b001,
    MIDDLE = 0b010,
    FAST =   0b100
};

void setup() {
    pinMode(MOTOR_A, OUTPUT);
    pinMode(MOTOR_B, OUTPUT);
    pinMode(ENABLE,  OUTPUT);

    pinMode(LED_SLOW,   OUTPUT);
    pinMode(LED_MIDDLE, OUTPUT);
    pinMode(LED_FAST,   OUTPUT);

    timer.setInterval(150); //加速の間隔を指定
    joy.calibrate();        //ジョイスティックを初期化
}

void loop() {
    static int16_t currentSpeed = 0;
    //uint8_t targetSpeed = map(joy.getDistance(), 0, 181, 0, 255);
    uint8_t targetSpeed = 0;
    uint16_t angle = joy.getAngle();

    digitalWrite(ENABLE, HIGH);

    if (joy.getDirection() == Joystick::Dir::CENTER) { targetSpeed = 0; }
    else { targetSpeed = 255; }

    if (timer.hasOccurred()) { //加速する間隔を超えた時
        //現在の速度が目標速度より遅い場合 ->加速する
        if (targetSpeed > currentSpeed) {
            currentSpeed += ACCELERATION;
            if (currentSpeed > 255) { currentSpeed = 255; }                 //最大の値を超えている場合は最大値に設定する
            if (currentSpeed > targetSpeed) { currentSpeed = targetSpeed; } //目標速度を超えている場合は目標速度に設定する
        } else { //現在の速度が目標速度より速い場合 -> 減速する
            currentSpeed -= ACCELERATION;
            if (currentSpeed < 0) { currentSpeed = 0; }                     //最小の値を下回っている場合は最小値に設定する
            if (currentSpeed < targetSpeed) { currentSpeed = targetSpeed; } //目標速度を下回っている場合は目標速度に設定する
        }

    }

    //速度に応じてLEDを点灯させる
    if (currentSpeed <= 100) {
        led(Speed::SLOW);
    } else if (currentSpeed <= 200) {
        led(Speed::MIDDLE);
    } else {
        led(Speed::FAST);
    }

    timer.start();

    analogWrite(MOTOR_A, currentSpeed);
}

inline void led(Speed sp) {
    uint8_t speed = static_cast<uint8_t>(sp);

    digitalWrite(LED_SLOW,   (speed & 0b001));
    digitalWrite(LED_MIDDLE, (speed & 0b010));
    digitalWrite(LED_FAST,   (speed & 0b100));
}
