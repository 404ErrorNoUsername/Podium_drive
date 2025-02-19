//gotta have these bad bois
#include "le3dp_rptparser.h"
#include "Servo.h"

//I DECLARE MOTOR!
Servo leftMotor, rightMotor;

//nerd junk
JoystickReportParser::JoystickReportParser(JoystickEvents *evt) : joyEvents(evt) {}

//black magic and more nerd junk
void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
    bool match = true;
    for (uint8_t i = 0; i < RPT_GAMEPAD_LEN; i++) {
        if (buf[i] != oldPad[i]) {
            match = false;
            break;
        }
    }
    if (!match && joyEvents) {
        joyEvents->OnGamePadChanged((const GamePadEventData*)buf);
        memcpy(oldPad, buf, RPT_GAMEPAD_LEN);
    }
}

//meat and taters
void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt) {
    
    //grab needed joystick data
    int twist = evt->twist;
    int drive = evt->y;
    int slide = evt->slider;

    //declare constants for drive, twist and deadband.
    const int driveMin = 0, driveMax = 1023, driveNeutral = 508;
    const int twistMin = 0, twistMax = 255, twistNeutral = 127;
    const int deadband = 5;

    //need a floaty boy for decimals
    float percentage = map(slide, 255, 0, 25, 100) / 100.0;

    //declares pwm signal from y axis and applies percentage limit based off of the sliders position.
    int pwmDrive = constrain(map(drive, driveMin, driveMax, 1000, 2000), 1000, 2000);
    pwmDrive = 1500 + (pwmDrive - 1500) * percentage;

    //declares pwm signal for twist axis and applies percentage limit. Bunch of weird nerd magic going on here.
    int pwmTurn = constrain(map(twist, twistMin, twistMax, 250, -250), -250, 250) * sqrt(percentage);

    //declares final pwm signal and blends for smooth turning
    int pwmLeftMotor = constrain(pwmDrive + pwmTurn, 1000, 2000);
    int pwmRightMotor = constrain(pwmDrive - pwmTurn, 1000, 2000);

    //applies deadband so joystick doesnt wig out when no input is given.
    if (abs(drive - driveNeutral) < deadband) drive = driveNeutral;
    if (abs(twist - twistNeutral) < deadband) twist = twistNeutral;

    //WHERE ARE WE GOING?!?!
    leftMotor.attach(9);
    rightMotor.attach(6);

    //FULL SEND!!
    leftMotor.writeMicroseconds(pwmLeftMotor);
    rightMotor.writeMicroseconds(pwmRightMotor);

  //prints values for debugging
    Serial.print("Twist: "); Serial.print(evt->twist);
    Serial.print(" Slider: "); Serial.print(evt->slider);
    Serial.print(" Left Motor: "); Serial.print(pwmLeftMotor);
    Serial.print(" Right Motor: "); Serial.println(pwmRightMotor);
}
