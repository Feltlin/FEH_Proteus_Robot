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
FEHMotor FLWheelMotor0 (FEHMotor::Motor0, 5.0);
FEHMotor FRWheelMotor1 (FEHMotor::Motor1, 5.0);

int main(void)
{
    double time = TimeNow();
    double elapsedTime = 0;
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Left Forward");
    FLWheelMotor0.SetPercent(100.);
    while (time < 1.)
    {
        time = TimeNow();
    }
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Left Reverse");
    FLWheelMotor0.SetPercent(-100.);
    while (time < 2.)
    {
        time = TimeNow();
    }
    FLWheelMotor0.SetPercent(0.);
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Right Forward");
    FRWheelMotor1.SetPercent(100.);
    while (time < 3.)
    {
        time = TimeNow();
    }
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Right Reverse");
    FRWheelMotor1.SetPercent(-100.);
    while (time < 4.)
    {
        time = TimeNow();
    }
    FRWheelMotor1.SetPercent(0.);
	return 0;
}
