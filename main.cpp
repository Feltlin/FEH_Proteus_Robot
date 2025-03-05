#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzerNoSleep.h>
#include <FEHRCS.h>
#include <FEHSD.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

void exploration_3(){
    FEHMotor LeftWheelMotor (FEHMotor::Motor0, 9.0);
    FEHMotor RightWheelMotor (FEHMotor::Motor1, 9.0);

    DigitalEncoder LeftEncoder(FEHIO::P0_2);
    DigitalEncoder RightEncoder(FEHIO::P0_1);

    DigitalInputPin distanceSensor(FEHIO::P0_0);

    //RCS.InitializeTouchMenu("1020C8WIE");

    bool objectWithinRange = false;
    LeftWheelMotor.SetPercent(-27);
    RightWheelMotor.SetPercent(-25);
    while (!objectWithinRange){
        objectWithinRange = !distanceSensor.Value();
        LCD.WriteLine("\u2207");
        LCD.WriteLine(objectWithinRange);
    }

    RightWheelMotor.SetPercent(25);
    LeftWheelMotor.SetPercent(25);
    Sleep(0.5);
    RightWheelMotor.Stop();
    LeftWheelMotor.Stop();


    //Turn left 90 degree.
    RightEncoder.ResetCounts();
    LeftEncoder.ResetCounts();
    RightWheelMotor.SetPercent(25);
    LeftWheelMotor.SetPercent(-25);
    while((LeftEncoder.Counts() + RightEncoder.Counts()) / 2. < 224);
    RightWheelMotor.Stop();
    LeftWheelMotor.Stop();

    objectWithinRange = false;
    LeftWheelMotor.SetPercent(-27);
    RightWheelMotor.SetPercent(-25);
    while (!objectWithinRange){
        objectWithinRange = !distanceSensor.Value();
        LCD.WriteLine(objectWithinRange);
    }

    RightWheelMotor.SetPercent(25);
    LeftWheelMotor.SetPercent(25);
    Sleep(0.5);
    RightWheelMotor.Stop();
    LeftWheelMotor.Stop();

    //Turn right 90 degree.
    RightEncoder.ResetCounts();
    LeftEncoder.ResetCounts();
    RightWheelMotor.SetPercent(-25);
    LeftWheelMotor.SetPercent(25);
    while((LeftEncoder.Counts() + RightEncoder.Counts()) / 2. < 224);
    RightWheelMotor.Stop();
    LeftWheelMotor.Stop();

    objectWithinRange = false;
    LeftWheelMotor.SetPercent(-26);
    RightWheelMotor.SetPercent(-25);
    while (!objectWithinRange){
        objectWithinRange = !distanceSensor.Value();
        LCD.WriteLine(objectWithinRange);
    }
    RightWheelMotor.Stop();
    LeftWheelMotor.Stop();
}

void colorSensor(){
    //Being Updated for Milestone 2, Originally from Exploration 01
    //This code uses no color filter for CdS Cell
    FEHMotor LeftWheelMotor (FEHMotor::Motor0, 9.0);
    FEHMotor RightWheelMotor (FEHMotor::Motor1, 9.0);
    //declares an analog input pin on P0_3
    AnalogInputPin CdS_cell(FEHIO::P0_3);
    //FEHServo servo(FEHServo::Servo0);
    //declares a digital input pin on P0_0
    //DigitalInputPin front_left(FEHIO::P3_7);
    //DigitalInputPin front_right(FEHIO::P0_0);
    //DigitalInputPin back_left(FEHIO::P3_6);
    //DigitalInputPin back_right(FEHIO::P0_1);

    float cds = CdS_cell.Value();
    //float angle = 0;
    float x;
    float y;
    float colorVal;
    //no light is 0, red is 1, blue is 2

    //servo.SetMin(500);
    //servo.SetMax(2419);
    //servo.SetDegree(0);
    Sleep(1.0);
    if (cds < 0.33){
        //Red Light
        colorVal = 1;
    }
    else if (cds < 0.66){
        //blue light
        colorVal = 2;
    }
    else{
        //no light
        colorVal = 0;
    }
    //servo.SetDegree(50);
    //Sleep(2.0);
    //servo.SetDegree(180);
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

    BuzzerNoSleep.Tone(BuzzerNoSleep.E4, 1.0);
    BuzzerNoSleep.Tone(BuzzerNoSleep.E4, 1.0);
    BuzzerNoSleep.Tone(BuzzerNoSleep.F4, 1.0);
    BuzzerNoSleep.Tone(BuzzerNoSleep.G5, 1.0);
    BuzzerNoSleep.Tone(BuzzerNoSleep.G5, 1.0);
    BuzzerNoSleep.Tone(BuzzerNoSleep.F5, 1.0);
    BuzzerNoSleep.Tone(BuzzerNoSleep.E5, 1.0);
    BuzzerNoSleep.Tone(BuzzerNoSleep.D5, 1.0);

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

struct Note{
    int duration, pitch;
};

enum{
    C8  = 4186,
    B7  = 3951, As7 = 3729, Bf7 = 3729, A7  = 3520, Gs7 = 3322, Af7 = 3322, G7  = 3136, Fs7 = 2960, Gf7 = 2960, F7  = 2794, E7  = 2637, Ds7 = 2489, Ef7 = 2489, D7  = 2349, Cs7 = 2217, Df7 = 2217, C7  = 2093,
    B6  = 1976, As6 = 1865, Bf6 = 1865, A6  = 1760, Gs6 = 1661, Af6 = 1661, G6  = 1568, Fs6 = 1480, Gf6 = 1480, F6  = 1397, E6  = 1319, Ds6 = 1245, Ef6 = 1244, D6  = 1175, Cs6 = 1109, Df6 = 1109, C6  = 1047,
    B5  = 988, As5 = 932, Bf5 = 932, A5  = 880, Gs5 = 831, Af5 = 831, G5  = 784, Fs5 = 740, Gf5 = 740, F5  = 698, E5  = 659, Ds5 = 622, Ef5 = 622, D5  = 587, Cs5 = 554, Df5 = 554, C5  = 523,
    B4  = 494, As4 = 466, Bf4 = 466, A4  = 440, Gs4 = 415, Af4 = 415, G4  = 392, Fs4 = 370, Gf4 = 370, F4  = 349, E4  = 330, Ds4 = 311, Ef4 = 311, D4  = 294, Cs4 = 277, Df4 = 277, C4  = 261,
    B3  = 247, As3 = 233, Bf3 = 233, A3  = 220, Gs3 = 208, Af3 = 208, G3  = 196, Fs3 = 185, Gf3 = 185, F3  = 175, E3  = 165, Ds3 = 156, Ef3 = 156, D3  = 147, Cs3 = 139, Df3 = 139, C3  = 131,
    B2  = 123, As2 = 117, Bf2 = 117, A2  = 110, Gs2 = 104, Af2 = 104, G2  = 98, Fs2 = 92, Gf2 = 92, F2  = 87, E2  = 82, Ds2 = 78, Ef2 = 78, D2  = 73, Cs2 = 69, Df2 = 69, C2  = 65,
    B1  = 62, As1 = 58, Bf1 = 58, A1  = 55, Gs1 = 52, Af1 = 52, G1  = 49, Fs1 = 46, Gf1 = 46, F1  = 44, E1  = 41, Ds1 = 39, Ef1 = 39, D1  = 37, Cs1 = 35, Df1 = 35, C1  = 33,
    B0  = 31, As0 = 29, Bf0 = 29, A0  = 28
};

void asyncMusic(){
    Note song[] = {
        {500, E4}, {500, E4}, {500, F4}, {500, G4},
        {500, G4}, {500, F4}, {500, E4}, {500, D4},
        {500, C4}, {500, C4}, {500, D4}, {500, E4},
        {750, E4}, {250, D4}, {1000, D4},
        {500, E4}, {500, E4}, {500, F4}, {500, G4},
        {500, G4}, {500, F4}, {500, E4}, {500, D4},
        {500, C4}, {500, C4}, {500, D4}, {500, E4},
        {750, D4}, {250, C4}, {1000, C4},
    };
    unsigned long t0 = TimeNowMSec(), t1 = t0, ta = 0;
    int i = 0;
    bool playing = false;
    while(true){
        t1 = TimeNowMSec() - t0;
        if(t1 < ta + song[i].duration){
            if(playing == false){
                BuzzerNoSleep.Tone(song[i].pitch, int(song[i].duration * 0.8));
                playing = true;
            }
        }
        else{
            ta += song[i].duration;
            ++i;
            playing = false;
        }
        LCD.WriteLine(int(t1));
    }
}

void milestone_1(){
    DigitalEncoder encoder[3] = {
        DigitalEncoder(FEHIO::P0_0),
        DigitalEncoder(FEHIO::P0_1),
        DigitalEncoder(FEHIO::P0_2),
    };
    FEHMotor motor[3] = {
        FEHMotor(FEHMotor::Motor0, 9.0),
        FEHMotor(FEHMotor::Motor1, 9.0),
        FEHMotor(FEHMotor::Motor2, 9.0),
    };
    for(int i = 0; i < 3; ++i){
        encoder[i].ResetCounts();
    }
    motor[1].SetPercent(-22.5);
    motor[2].SetPercent(20);
    while((encoder[1].Counts() + encoder[2].Counts()) / 2 * 2.5 * M_PI / 318 < 26){}
    motor[1].Stop();
    motor[2].Stop();

    float x, y;
    while(!LCD.Touch(&x, &y)){}
    for(int i = 0; i < 3; ++i){
        encoder[i].ResetCounts();
    }
    motor[1].SetPercent(-41.7);
    motor[2].SetPercent(40);
    while((encoder[1].Counts() + encoder[2].Counts()) / 2 * 2.5 * M_PI / 318 < 28){}
    motor[1].Stop();
    motor[2].Stop();

    Sleep(1.);
    for(int i = 0; i < 3; ++i){
        encoder[i].ResetCounts();
    }
    motor[1].SetPercent(40);
    motor[2].SetPercent(-41);
    while((encoder[1].Counts() + encoder[2].Counts()) / 2 * 2.5 * M_PI / 318 < 23){}
    motor[1].Stop();
    motor[2].Stop();
}

void fiveRevolution(){
    FEHMotor frontMotor = FEHMotor(FEHMotor::Motor0, 9.0);
    DigitalEncoder frontEncoder = DigitalEncoder(FEHIO::P0_0);

    frontMotor.SetPercent(20.);

    while(frontEncoder.Counts() <= 318 * 5){
        LCD.WriteLine(frontEncoder.Counts());
    }
    frontMotor.Stop();
}

class State{
    public:
        DigitalEncoder encoder[3] = {
            DigitalEncoder(FEHIO::P0_0),
            DigitalEncoder(FEHIO::P0_1),
            DigitalEncoder(FEHIO::P0_2),
        };
        FEHMotor motor[3] = {
            FEHMotor(FEHMotor::Motor0, 9.0),
            FEHMotor(FEHMotor::Motor1, 9.0),
            FEHMotor(FEHMotor::Motor2, 9.0),
        };
        double inchPerCount = 2.5 * M_PI / 318;

        //{Front, Left, Right}
        double newError[3] = {0, 0, 0}, lastError[3] = {0, 0, 0}, errorSum[3] = {0, 0, 0}, errorDel[3] = {0, 0, 0};
        double PTerm[3] = {0, 0, 0}, ITerm[3] = {0, 0, 0}, DTerm[3] = {0, 0, 0};
        double PConst[3] = {0.03, 0.03, 0.03}, IConst[3] = {0, 0, 0}, DConst[3] = {1e-3, 1e-3, 1e-3};

        double actualSpeed[3] = {0, 0, 0}, expectedSpeed[3] = {0, 0, 0};
        double power[3] = {0, 0, 0};
        double distance[3] = {0, 0, 0};
        int direction[3] = {1, 1, 1};

        int newCount[3] = {0, 0, 0}, lastCount[3] = {0, 0, 0};

        unsigned long newTime = 0, lastTime = 0;

        std::string state = "start";
        bool init = true;

    State(){
        for(int i = 0; i < 3; ++i){
            encoder[i].ResetCounts();
        }
    }

    void updateTime(){
        lastTime = newTime;
        newTime = TimeNowMSec();
    }

    void updateState(){
        if(state == "start"){
            start(); 
        }
    }

    void start(){
        if(init){
            init = false;
            expectedSpeed[0] = 0;
            expectedSpeed[1] = 2;
            expectedSpeed[2] = 2;
            direction[1] = -1;
            direction[2] = 1;
            power[1] = 20;
            power[2] = 20;
            motor[1].SetPercent(power[1] * direction[1]);
            motor[2].SetPercent(power[2] * direction[2]);

        }
        else if ((newCount[1] + newCount[2]) / 2 * inchPerCount < 200){
            for(int i = 0; i < 3; ++i){
                if(i == 1){
                    LCD.Write(" Left: ");
                    LCD.Write(power[i]);
                }
                else if(i == 2){
                    LCD.Write(" Right: ");
                    LCD.Write(power[i]);
                }
                
                lastCount[i] = newCount[i];
                newCount[i] = encoder[i].Counts();
                actualSpeed[i] = inchPerCount * (newCount[i] - lastCount[i]) / (newTime - lastTime) * 1000;
                lastError[i] = newError[i];
                newError[i] = expectedSpeed[i] - actualSpeed[i];
                errorDel[i] = (newError[i] - lastError[i]) / (newTime - lastTime) * 1000;
                errorSum[i] += newError[i];
                PTerm[i] = newError[i] * PConst[i];
                ITerm[i] = /*errorSum[i] * IConst[i]*/0;
                DTerm[i] = /*errorDel[i] * DConst[i]*/0;
                power[i] = std::min(std::max(power[i] + PTerm[i] + ITerm[i] + DTerm[i], 0.), 50.);
                if(i == 1){
                    LCD.Write(" Left ATTEMPT: ");
                    LCD.Write(power[i]);
                }
                else if(i == 2){
                    LCD.Write(" Right ATTEMPT: ");
                    LCD.Write(power[i]);
                }
                motor[i].SetPercent(power[i] * direction[i]);

                //Place to add CdS Code
            }
            Sleep(0.01);
        }
        else{
            for(int i = 0; i < 3; ++i){
                motor[i].Stop();
            }
            LCD.WriteLine("");
            LCD.Write("Front: ");
            LCD.WriteLine(encoder[0].Counts());
            LCD.Write("Left: ");
            LCD.WriteLine(encoder[1].Counts());
            LCD.Write("Right: ");
            LCD.WriteLine(encoder[2].Counts());
        }
    }
};

State state;

int main(){
    while(true){
        state.updateTime();
        state.updateState();
    }

	return 0;
}