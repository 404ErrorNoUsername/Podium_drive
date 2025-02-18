//gotta have these bad bois
#include "le3dp_rptparser.h"
#include "Servo.h"

//I DECLARE MOTORS
Servo leftMotor, rightMotor;

//nerd stuff to parse joystick data
JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
	joyEvents(evt)
{}

//more nerd stuff
void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
	bool match = true;

	// Checking if there are changes in report since the method was last called
	for (uint8_t i=0; i<RPT_GAMEPAD_LEN; i++) {
		if( buf[i] != oldPad[i] ) {
			match = false;
			break;
		}
  }
  	// Calling Game Pad event handler
	if (!match && joyEvents) {
		joyEvents->OnGamePadChanged((const GamePadEventData*)buf);

		for (uint8_t i=0; i<RPT_GAMEPAD_LEN; i++) oldPad[i] = buf[i];
	}
}


//The meat and 'taters
void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{
// grab joystick data and set variables.
int twist =  evt->twist;
int drive =  evt->y;

// Define joystick twist range
const int driveMin = 0;
const int driveMax = 1023;
const int driveNeutral = 508;

const int twistRevMin = 0;
const int twistRevMax = 255;
const int twistRevNeutral = 127;

const int twistMin = 0;
const int twistMax = 255;
const int twistNeutral = 127;
const int deadband = 5;

//map y axis to drive
int pwmDrive = map(drive, driveMin, driveMax, 1000, 2000);
pwmDrive = constrain(pwmDrive, 1000, 2000);

// Map twist to PWM signal (1000-2000 µs)
int pwmTwistValue = map(twist, twistMin, twistMax, 1000, 2000);
pwmTwistValue = constrain(pwmTwistValue, 1000, 2000);
int pwmTwistRev = map(twist, twistMin, twistMax, 2000, 1000);
pwmTwistRev = constrain(pwmTwistRev, 1000, 2000);
    
// Creates a turning offset
int pwmTurn = map(twist, twistMin, twistMax, 250, -250); 
pwmTurn = constrain(pwmTurn, -250, 250);

// blend y and twist pwm signals for smooth turning. USE THESE TO WRITE PWM SIGNAL!
int pwmLeftMotor = pwmDrive + pwmTurn;
int pwmRightMotor = pwmDrive - pwmTurn;
    
    
// Apply deadband correction
if (abs(drive - driveNeutral) < deadband) {
  drive = driveNeutral;
}
if (abs(twist - twistRevNeutral) < deadband) {
  twist = twistRevNeutral;
}
    
  if (abs(twist - twistNeutral) < deadband) {
      twist = twistNeutral;
}

    
//makes sure pwm signal doesn't exceed expected range.
pwmLeftMotor = constrain(pwmLeftMotor, 1000, 2000);
pwmRightMotor = constrain(pwmRightMotor, 1000, 2000);



//IDK what's going on here but it's important
Serial.print("X: ");
PrintHex<uint16_t>(evt->x, 0x80);
Serial.print(" Y: ");
PrintHex<uint16_t>(evt->y, 0x80);
Serial.print(" Hat Switch: ");
PrintHex<uint8_t>(evt->hat, 0x80);
Serial.print(" Twist: ");
PrintHex<uint16_t>(evt->twist, 0x80);
Serial.print(" Slider: ");
PrintHex<uint8_t>(evt->slider, 0x80);
Serial.print(" Buttons A: ");
PrintHex<uint8_t>(evt->buttons_a, 0x80);
Serial.print(" Buttons B: ");
PrintHex<uint8_t>(evt->buttons_b, 0x80);
Serial.println("");
    
// Send PWM signal to Victor SPX using Servo library I'm sending the pwm signal out of pin 9 to two different victor spx because thats sketchy and on par with my opperations. 
leftMotor.attach(9);
rightMotor.attach(6);
    
//sends signal to motor controller.
leftMotor.writeMicroseconds(pwmLeftMotor);
rightMotor.writeMicroseconds(pwmRightMotor);   

// debuging console prints pwm values going to the left and right side of the drive train.
Serial.print("PWM Left Motor");
Serial.print(pwmLeftMotor);
Serial.print("PWM Right Motor");
Serial.print(pwmRightMotor);
}
