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
//declares a digital input pin on P0_0
DigitalInputPin front_left(FEHIO::P3_7);
DigitalInputPin front_right(FEHIO::P0_0);
DigitalInputPin back_left(FEHIO::P3_6);
DigitalInputPin back_right(FEHIO::P0_1);

bool FLSwitch = true;
bool FRSwitch = true;
bool BLSwitch = true;
bool BRSwitch = true;

void Left(float degrees)
{
    LeftWheelMotor.SetPercent(-25);
    RightWheelMotor.SetPercent(25);
    Sleep(0.5 * (degrees/90.));
    LeftWheelMotor.Stop();
    RightWheelMotor.Stop();
}

void Right(float degrees)
{
    LeftWheelMotor.SetPercent(25);
    RightWheelMotor.SetPercent(-25);
    Sleep(0.5 * (degrees/90.));
    LeftWheelMotor.Stop();
    RightWheelMotor.Stop();
}

void InchForward()
{
    LeftWheelMotor.SetPercent(25);
    RightWheelMotor.SetPercent(25);
    Sleep(0.2);
}

void InchBackwards()
{
    LeftWheelMotor.SetPercent(-25);
    RightWheelMotor.SetPercent(-25);
    Sleep(0.2);
}

void Forward()
{
    RightWheelMotor.SetPercent(25);
    LeftWheelMotor.SetPercent(25);
}

void Backwards()
{
    RightWheelMotor.SetPercent(-25);
    LeftWheelMotor.SetPercent(-25);
}

void SetBools()
{
    FLSwitch = front_left.Value();
    FRSwitch = front_right.Value();
    BLSwitch = back_left.Value();
    BRSwitch = back_right.Value();
}

void WaitForSwitches(bool & switch1, bool & switch2)
{
    while(switch1 && switch2)
    {
        SetBools();
    }
}

int main(void)
{
    Forward();
    WaitForSwitches(FRSwitch, FLSwitch);
    InchBackwards();
    Left(90);
    Forward();
    WaitForSwitches(FRSwitch, FLSwitch);
    Backwards();
    WaitForSwitches(BLSwitch, BRSwitch);
    InchForward();
    Left(90);
    Forward();
    WaitForSwitches(FRSwitch, FLSwitch);
    Backwards();
    WaitForSwitches(BLSwitch, BRSwitch);
    RightWheelMotor.Stop();
    LeftWheelMotor.Stop();
    
	return 0;
}
