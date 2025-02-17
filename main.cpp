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

void colorSensor(){
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

    servo.SetMin(500);
    servo.SetMax(2419);
    servo.SetDegree(0);
    Sleep(1.0);
    servo.SetDegree(50);
    Sleep(2.0);
    servo.SetDegree(180);
    // while (!LCD.Touch(&x, &y))
    // {
    //     cds = CdS_cell.Value();
    //     angle = (180.0/3.3) * cds;
    //     servo.SetDegree(angle);
    //     LCD.WriteLine(cds);
    // }
}

void LineFollow(){
    enum LineState{
        middle = 0,
        right = 1,
        left = 2
    };

    FEHMotor LWMotor(FEHMotor::Motor0, 9.0);
    FEHMotor RWMotor(FEHMotor::Motor1, 9.0);
    AnalogInputPin ROSensor(FEHIO::P0_0);
    AnalogInputPin MOSensor(FEHIO::P0_1);
    AnalogInputPin LOSensor(FEHIO::P0_2);


    int state = middle;
    while(true){
        switch(state){
            case middle:
                LWMotor.SetPercent(25);
                RWMotor.SetPercent(25);
                if(MOSensor.Value() < 2){
                    if(LOSensor.Value() > 1.5){
                        state = left;
                    }
                    else if(ROSensor.Value() > 2.4){
                        state = right;
                    }
                }

                break;

            case right:
                LWMotor.SetPercent(25);
                RWMotor.SetPercent(5);
                if(ROSensor.Value() < 2.4){
                    if(MOSensor.Value() > 2){
                        state = middle;
                    }
                }

                break;

            case left:
                LWMotor.SetPercent(5);
                RWMotor.SetPercent(25);
                if(LOSensor.Value() < 1.5){
                    if(MOSensor.Value() > 2){
                        state = middle;
                    }
                }

                break;
        }
    }
}

void OSensorValue(){
    FEHMotor LWMotor(FEHMotor::Motor0, 9.0);
    FEHMotor RWMotor(FEHMotor::Motor1, 9.0);
    AnalogInputPin ROSensor(FEHIO::P0_0);
    AnalogInputPin MOSensor(FEHIO::P0_1);
    AnalogInputPin LOSensor(FEHIO::P0_2);
    int state = 0;
    int x, y;

    while(true){
        if(state == 0){
            LCD.WriteLine(MOSensor.Value());
            LCD.WriteLine("Middle");
            Sleep(1.0);
            if(LCD.Touch(&x, &y) == 1){
                state = 1;
            }
        }
        else if(state == 1){
            LCD.WriteLine(LOSensor.Value());
            LCD.WriteLine("Left");
            Sleep(1.0);
            if(LCD.Touch(&x, &y) == 1){
                state = 2;
            }
        }
        else if(state == 2){
            LCD.WriteLine(ROSensor.Value());
            LCD.WriteLine("Right");
            Sleep(1.0);
            if(LCD.Touch(&x, &y) == 1){
                state = 0;
            }
        }
    }
}

void ShaftEncoding(){
    DigitalEncoder right_encoder(FEHIO::P0_0);
    DigitalEncoder left_encoder(FEHIO::P0_1);
    FEHMotor right_motor(FEHMotor::Motor0,9.0);
    FEHMotor left_motor(FEHMotor::Motor1,9.0);
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Move 14 in.
    right_motor.SetPercent(25);
    left_motor.SetPercent(25);
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < 567);
    right_motor.Stop();
    left_motor.Stop();

    //Turn left 90 degree.
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(25);
    left_motor.SetPercent(-25);
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < 224);
    right_motor.Stop();
    left_motor.Stop();

    //Move 10 in.
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(25);
    left_motor.SetPercent(25);
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < 405);
    right_motor.Stop();
    left_motor.Stop();

    //Turn right 90 degree.
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(-25);
    left_motor.SetPercent(25);
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < 224);
    right_motor.Stop();
    left_motor.Stop();

    //Move 4 in.
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_motor.SetPercent(25);
    left_motor.SetPercent(25);
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < 162);
    right_motor.Stop();
    left_motor.Stop();


}

int main(){
    ShaftEncoding();
	return 0;
}