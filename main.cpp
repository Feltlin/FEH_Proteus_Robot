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
FEHMotor FLWheelMotor0 (FEHMotor::Motor0, 9.0);
FEHMotor FRWheelMotor1 (FEHMotor::Motor1, 9.0);
//declares an analog input pin on P0_3
AnalogInputPin CdS_cell(FEHIO::P0_3);
//declares a digital input pin on P2_1
DigitalInputPin bump_switch(FEHIO::P2_1);

int main(void)
{
    //read the value of the CdS_cell and store in x
    float cdsValue = CdS_cell.Value();
    //read the value of the digital input into x
    bool switchy = bump_switch.Value();
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Left Forward");
    FLWheelMotor0.SetPercent(100.);
    Sleep(1.);
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Left Reverse");
    FLWheelMotor0.SetPercent(-100.);
    Sleep(1.);
    FLWheelMotor0.Stop();
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Right Forward");
    FRWheelMotor1.SetPercent(100.);
    Sleep(1.);
    LCD.Clear(BLACK);
    LCD.WriteLine("Testing Front Right Reverse");
    FRWheelMotor1.SetPercent(-100.);
    Sleep(1.);
    FRWheelMotor1.Stop();
	return 0;
}
