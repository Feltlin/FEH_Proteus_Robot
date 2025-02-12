#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHRCS.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>

// Front wheel drive, front wheels are normal, back wheels are omni-wheels
// Tank drive
FEHMotor LeftWheelMotor (FEHMotor::Motor0, 9.0);
FEHMotor RightWheelMotor (FEHMotor::Motor1, 9.0);
//declares an analog input pin on P0_3
AnalogInputPin CdS_cell(FEHIO::P0_3);
FEHServo servo(FEHServo::Servo0);
//declares a digital input pin on P0_0
DigitalInputPin front_left(FEHIO::P3_7);
DigitalInputPin front_right(FEHIO::P0_0);
DigitalInputPin back_left(FEHIO::P3_6);
DigitalInputPin back_right(FEHIO::P0_1);

float cds = CdS_cell.Value();
float angle = 0;
float x;
float y;

int main(void)
{
    servo.Min();
    while (!LCD.Touch(&x, &y))
    {
        cds = CdS_cell.Value();
        angle = (180.0/3.3) * cds;
        servo.SetDegree(angle);
        LCD.WriteLine(cds);
    }
    
	return 0;
}
