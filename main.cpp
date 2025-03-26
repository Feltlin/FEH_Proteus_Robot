#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHRCS.h>
#include <FEHSD.h>

#include <FEHBuzzerNoSleep.h>
#include <text.h>

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

//FEHFile *fptr = SD.FOpen("Test.txt", "w");

void fiveRevolution(){
    FEHMotor frontMotor = FEHMotor(FEHMotor::Motor0, 9.0);
    DigitalEncoder frontEncoder = DigitalEncoder(FEHIO::P0_0);

    frontMotor.SetPercent(20.);

    while(frontEncoder.Counts() <= 318 * 5){
        LCD.WriteLine(frontEncoder.Counts());
    }
    frontMotor.Stop();
}

// class _State{
//     public:
//         DigitalEncoder encoder[3] = {
//             DigitalEncoder(FEHIO::P0_0),
//             DigitalEncoder(FEHIO::P0_1),
//             DigitalEncoder(FEHIO::P0_2),
//         };
//         FEHMotor motor[3] = {
//             FEHMotor(FEHMotor::Motor0, 9.0),
//             FEHMotor(FEHMotor::Motor1, 9.0),
//             FEHMotor(FEHMotor::Motor2, 9.0),
//         };
//         AnalogInputPin CdS = AnalogInputPin(FEHIO::P0_3);
//         double inchPerCount = 2.5 * M_PI / 318;

//         //{Front, Left, Right}
//         double newError[3] = {0, 0, 0}, oldError[3] = {0, 0, 0}, errorSum[3] = {0, 0, 0}, errorDel[3] = {0, 0, 0};
//         double PTerm[3] = {0, 0, 0}, ITerm[3] = {0, 0, 0}, DTerm[3] = {0, 0, 0};
//         double PConst[3] = {0.3, 0.3, 0.3}, IConst[3] = {0, 0, 0}, DConst[3] = {0, 0, 0};

//         double actualSpeed[3] = {0, 0, 0}, expectedSpeed[3] = {0, 0, 0};
//         double power[3] = {0, 0, 0};
//         int direction[3] = {0, 0, 0};

//         int newCount[3] = {0, 0, 0}, oldCount[3] = {0, 0, 0};

//         unsigned long newTime[3] = {0, 0, 0}, oldTime[3] = {0, 0, 0};

//         std::string state = "startMenu";
//         bool init = true;
//         bool switchingState = false;
//         int sweepState = 0;
//         int sweepCount = 0;
//         float minCDS = 3;
//         std::string lightColor = "nothing";
        
//         Text text;

//     _State(){
//         for(int i = 0; i < 3; ++i){
//             encoder[i].ResetCounts();
//         }
//     }

//     void PID(){
//         Sleep(0.01);
//         for(int i = 0; i < 3; ++i){
//             updateTime(i);
//             oldCount[i] = newCount[i];
//             newCount[i] = encoder[i].Counts();
//             actualSpeed[i] = inchPerCount * (newCount[i] - oldCount[i]) / (newTime[i] - oldTime[i]) * 1000;
//             oldError[i] = newError[i];
//             newError[i] = expectedSpeed[i] - actualSpeed[i];
//             errorDel[i] = (newError[i] - oldError[i]) / (newTime[i] - oldTime[i]) * 1000;
//             errorSum[i] += newError[i];
//             PTerm[i] = newError[i] * PConst[i];
//             ITerm[i] = errorSum[i] * IConst[i];
//             DTerm[i] = errorDel[i] * DConst[i];
//             power[i] = std::min(std::max(power[i] + PTerm[i] + ITerm[i] + DTerm[i], 0.), 50.);
//             motor[i].SetPercent(power[i] * direction[i]);
//         }
//     }

//     // Rotational PID but ensure 3 wheel have the same encoder count with a min power.
//     void RPID(){
//         for(int i = 0; i < 3; ++i){
//             oldCount[i] = newCount[i];
//             newCount[i] = encoder[i].Counts();
//             motor[i].SetPercent(power[i] * direction[i]);
//         }
//     }

//     //Rotational PID but front wheel regular PID, left and right use encoder count as error.
//     void RPID2(){
//         for(int i = 0; i < 3; ++i){
//             updateTime(i);
//             oldCount[i] = newCount[i];
//             newCount[i] = encoder[i].Counts();
//             oldError[i] = newError[i];
//             if(i == 0){
//                 actualSpeed[i] = inchPerCount * (newCount[i] - oldCount[i]) / (newTime[i] - oldTime[i]) * 1000;
//                 newError[i] = expectedSpeed[i] - actualSpeed[i];
//             }
//             // actualSpeed[i] = inchPerCount * (newCount[i] - oldCount[i]) / (newTime - oldTime) * 1000;
//             else{
//                 newError[i] = newCount[0] - newCount[i];
//             }
//             // newError[i] = expectedSpeed[i] - actualSpeed[i];
//             errorDel[i] = (newError[i] - oldError[i]) / (newTime[i] - oldTime[i]) * 1000;
//             errorSum[i] += newError[i];
//             PTerm[i] = newError[i] * PConst[i];
//             ITerm[i] = errorSum[i] * IConst[i];
//             DTerm[i] = errorDel[i] * DConst[i];
//             // power[i] = std::min(std::max(power[i] + PTerm[i] + ITerm[i] + DTerm[i], 0.), 50.);
//             power[i] = std::min(std::max(power[i] + PTerm[i] + ITerm[i] + DTerm[i], power[0] / 2), 50.);
//             motor[i].SetPercent(power[i] * direction[i]);
            
//         }
//     }

//     void zero(){

//         // SD.FPrintf(fptr, "%s\n", state);
//         // SD.FPrintf(fptr, "Real values:\n");
//         // SD.FPrintf(fptr, "%i\n", encoder[0].Counts());
//         // SD.FPrintf(fptr, "%i\n", encoder[1].Counts());
//         // SD.FPrintf(fptr, "%i\n", encoder[2].Counts());
//         // SD.FPrintf(fptr, "NewCount values:\n");
//         // SD.FPrintf(fptr, "%i\n", newCount[0]);
//         // SD.FPrintf(fptr, "%i\n", newCount[0]);
//         // SD.FPrintf(fptr, "%i\n", newCount[0]);

//         for(int i = 0; i < 3; ++i){
//             motor[i].SetPercent(0);
//             motor[i].Stop();
//             encoder[i].ResetCounts();
//             newError[i] = 0;
//             oldError[i] = 0;
//             errorSum[i] = 0;
//             errorDel[i] = 0;
//             PTerm[i] = 0;
//             ITerm[i] = 0;
//             DTerm[i] = 0;

//             actualSpeed[i] = 0;
//             expectedSpeed[i] = 0;
//             power[i] = 0;
//             direction[i] = 0;

//             newCount[i] = 0;
//             oldCount[i] = 0;

//             newTime[i] = 0;
//             oldTime[i] = 0;
//         }
//     }

//     void updateTime(int i){
//         oldTime[i] = newTime[i];
//         newTime[i] = TimeNowMSec();
//         //LCD.Write("oldTime: ");
//         //LCD.WriteLine(int(oldTime[i]));
//         //LCD.Write("newTime: ");
//         //LCD.WriteLine(int(newTime[i]));
//     }

//     void updateState(){
//         if(state == "startMenu"){
//             startMenu();
//         }
//         else if(state == "start"){
//             start(); 
//         }
//         else if(state == "rotate"){
//             rotate();
//         }
//         else if(state == "moveToButton"){
//             moveToButton();
//         }
//         else if(state == "sweepForLight"){
//             sweepForLight();
//         }
//         else if(state == "touchButton"){
//             touchButton();
//         }
//         else if(state == "moveAwayFromButton"){
//             moveAwayFromButton();
//         }
//         else if(state == "rotateBack"){
//             rotateBack();
//         }
//         else if(state == "moveDownSlope"){
//             moveDownSlope();
//         }
//         else if(state == "final"){
//             final();
//         }
//     }

//     void startMenu(){
//         if(init){
//             LCD.Clear();
//             init = false;
//             zero();
//         }
//         else if(text.button("Touch me", 0xffffff, 136, 100)){
//             text.display("YOU WILL GET TOUCHED.", 0xff0000, 100, 58);
//             Sleep(3.0);
//             state = "start";
//             init = true;
//             zero();
//         }
//         else if(text.button("Rotate", 0xffffff, 136, 120)){
//             Sleep(3.0);
//             state = "rotate";
//             init = true;
//             zero();
//         }
//         else if(text.button("Touch Button", 0xffffff, 136, 140)){
//             state = "touchButton";
//             init = true;
//             zero();
//         }
//     }

//     void start(){
//         if(init){
//             init = false;
//             expectedSpeed[1] = 6;
//             expectedSpeed[2] = 6;
//             direction[1] = -1;
//             direction[2] = 1;
//             power[1] = 20;
//             power[2] = 20;
//             motor[1].SetPercent(power[1] * direction[1]);
//             motor[2].SetPercent(power[2] * direction[2]);

//         }
//         else if((newCount[1] + newCount[2]) / 2 * inchPerCount < 35){
//             PID();
//         }
//         else{
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//             state = "rotate";
//             init = true;
//             zero();
//         }
//     }

//     float CDS()
//     {
//         return CdS.Value();
//     }

//     void rotate(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             for(int i = 0; i < 3; ++i){
//                 expectedSpeed[i] = 4;
//                 direction[i] = 1;
//                 power[i] = 15;
//                 motor[i].SetPercent(power[i] * direction[i]);
//             }
//         }
//         else if((encoder[0].Counts() + encoder[1].Counts() + encoder[2].Counts()) / 3 * inchPerCount < 2.65 * M_PI / 2){
//             RPID();
//         }
//         else{
//             motor[0].Stop();
//             motor[1].Stop();
//             motor[2].Stop();
//             state = "moveToButton";
//             init = true;
//             zero();
//         }
//     }

//     void moveToButton(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             expectedSpeed[1] = 6;
//             expectedSpeed[2] = 6;
//             direction[1] = -1;
//             direction[2] = 1;
//             power[1] = 20;
//             power[2] = 20;
//             motor[0].Stop();
//             motor[1].SetPercent(power[1] * direction[1]);
//             motor[2].SetPercent(power[2] * direction[2]);
//         }
//         else if((newCount[1] + newCount[2]) / 2 * inchPerCount < 11){
//             PID();
//         }
//         else{
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//             state = "sweepForLight";
//             init = true;
//             zero();
//         }
//     }

//     void sweepForLight(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             expectedSpeed[1] = 6;
//             direction[1] = -1;
//             power[1] = 10;
//             motor[1].SetPercent(power[1] * direction[1]);
//             sweepState = 1;
//         }
//         else if(sweepCount <= 7){
//             if(newCount[1] * inchPerCount < 0.5 && sweepState == 1){
//                 minCDS = std::min(minCDS, CDS());
//                 PID();
//             }
//             else if(sweepState == 1){
//                 motor[1].Stop();
//                 zero();
//                 sweepState = 2;
//                 ++sweepCount;
//                 expectedSpeed[2] = 6;
//                 direction[2] = 1;
//                 power[2] = 10;
//                 motor[2].SetPercent(power[2] * direction[2]);
//             }
//             if(newCount[2] * inchPerCount < 0.5 && sweepState == 2){
//                 minCDS = std::min(minCDS, CDS());
//                 PID();
//             }
//             else if(sweepState == 2){
//                 motor[2].Stop();
//                 zero();
//                 sweepState = 1;
//                 ++sweepCount;
//                 expectedSpeed[1] = 6;
//                 direction[1] = -1;
//                 power[1] = 10;
//                 motor[1].SetPercent(power[1] * direction[1]);
//             }
//         }
//         else{
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//             state = "touchButton";
//             init = true;
//             zero();
//         }
//     }

//     void touchButton(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             // expectedSpeed[1] = 2;
//             // expectedSpeed[2] = 2;
//             // direction[1] = -1;
//             // direction[2] = 1;
//             // power[1] = 20;
//             // power[2] = 20;
//             // motor[1].SetPercent(power[1] * direction[1]);
//             // motor[2].SetPercent(power[2] * direction[2]);
//             LCD.Clear();
//             text.display(std::to_string(CDS()), 0xffffff, 100, 160);
//             if(minCDS < 0.48){
//                 lightColor = "red";
//                 text.display("I SAW DAMN RED", 0xffffff, 100, 80);
//             }
//             else if(minCDS < 0.9){
//                 lightColor = "blue";
//                 text.display("I SAW DAMN BLUE", 0xffffff, 100, 80);
//             }
//             else{
//                 text.display("DIDN'T SEE THE DAMN LIGHT", 0xffffff, 100, 80);
//                 text.display("MINIMUM VALUE WAS", 0xffffff, 100, 100);
//                 text.display(std::to_string(minCDS), 0xffffff, 100, 120);
//             }
//             direction[1] = -1;
//             direction[2] = 1;
//             motor[0].Stop();
//             if(lightColor == "blue"){
//                 expectedSpeed[1] = 4;
//                 expectedSpeed[2] = 5;
//                 power[1] = 5;
//                 power[2] = 20;
//             }
//             else{
//                 expectedSpeed[1] = 5;
//                 expectedSpeed[2] = 4;
//                 power[1] = 20;
//                 power[2] = 5;
//             }
//             motor[1].SetPercent(power[1] * direction[1]);
//             motor[2].SetPercent(power[2] * direction[2]);
//         }
//         else if((newCount[1] + newCount[2]) * inchPerCount < 12){
//             PID();
//         }
//         else{
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//             state = "moveAwayFromButton";
//             init = true;
//             zero();
//         }
//     }

//     void moveAwayFromButton(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             expectedSpeed[1] = 6;
//             expectedSpeed[2] = 6;
//             direction[1] = 1;
//             direction[2] = -1;
//             power[1] = 20;
//             power[2] = 20;
//             motor[0].Stop();
//             motor[1].SetPercent(power[1] * direction[1]);
//             motor[2].SetPercent(power[2] * direction[2]);
//         }
//         else if((newCount[1] + newCount[2]) / 2 * inchPerCount < 17){
//             PID();
//         }
//         else{
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//             state = "rotateBack";
//             init = true;
//             zero();
//         }
//     }

//     void rotateBack(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             for(int i = 0; i < 3; ++i){
//                 expectedSpeed[i] = 5;
//                 direction[i] = 1;
//                 power[i] = 15;
//                 motor[i].SetPercent(power[i] * direction[i]);
//             }
//         }
//         else if((encoder[0].Counts() + encoder[1].Counts() + encoder[2].Counts()) / 3 * inchPerCount < 2.65 * M_PI / 2){
//             RPID();
//         }
//         else{
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//             state = "moveDownSlope";
//             init = true;
//             zero();
//         }
//     }

//     void moveDownSlope(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             expectedSpeed[1] = 6;
//             expectedSpeed[2] = 6;
//             direction[1] = -1;
//             direction[2] = 1;
//             power[1] = 20;
//             power[2] = 20;
//             motor[1].SetPercent(power[1] * direction[1]);
//             motor[2].SetPercent(power[2] * direction[2]);

//         }
//         else if((newCount[1] + newCount[2]) / 2 * inchPerCount < 35){
//             PID();
//         }
//         else{
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//             state = "final";
//             init = true;
//             zero();
//         }
//     }

//     void final(){
//         if(init){
//             Sleep(1.);
//             init = false;
//             for(int i = 0; i < 3; ++i){
//                 motor[i].Stop();
//             }
//         }
//         else{
//             if(lightColor == "blue"){
//                 LCD.SetBackgroundColor(BLUE);
//             }
//             else{
//                 LCD.SetBackgroundColor(RED);
//             }
//             LCD.Clear();
//         }
//     }

// };

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
FEHServo armServo(FEHServo::Servo0);
AnalogInputPin CdS = AnalogInputPin(FEHIO::P0_3);

double inchPerCount = 2.5 * M_PI / 318;

//{Front, Left, Right}
double newError[3] = {0, 0, 0}, oldError[3] = {0, 0, 0}, errorSum[3] = {0, 0, 0}, errorDel[3] = {0, 0, 0};
double PTerm[3] = {0, 0, 0}, ITerm[3] = {0, 0, 0}, DTerm[3] = {0, 0, 0};
double PConst[3] = {0.2, 0.2, 0.2}, IConst[3] = {0.003, 0.003, 0.003}, DConst[3] = {0.01, 0.01, 0.01};

double actualSpeed[3] = {0, 0, 0}, expectedSpeed[3] = {0, 0, 0};
double power[3] = {0, 0, 0};
int direction[3] = {0, 0, 0};
double vx = 0, vy = 0;

int newCount[3] = {0, 0, 0}, oldCount[3] = {0, 0, 0};

unsigned long newTime[3] = {0, 0, 0}, oldTime[3] = {0, 0, 0};

bool switchingState = false;
int sweepState = 0;
int sweepCount = 0;
float minCDS = 3;
std::string lightColor = "nothing";

Text text;

void zero(){
    for(int i = 0; i < 3; ++i){
        motor[i].Stop();
        encoder[i].ResetCounts();
        newError[i] = 0;
        oldError[i] = 0;
        errorSum[i] = 0;
        errorDel[i] = 0;
        PTerm[i] = 0;
        ITerm[i] = 0;
        DTerm[i] = 0;

        actualSpeed[i] = 0;
        expectedSpeed[i] = 0;
        power[i] = 0;
        direction[i] = 0;

        newCount[i] = 0;
        oldCount[i] = 0;
    }
}

void speedPID(){
    Sleep(15);
    for(int i = 0; i < 3; ++i){
        oldTime[i] = newTime[i];
        newTime[i] = TimeNowMSec();
        oldCount[i] = newCount[i];
        newCount[i] = encoder[i].Counts();
        actualSpeed[i] = inchPerCount * (newCount[i] - oldCount[i]) / (newTime[i] - oldTime[i]) * 1000;
        oldError[i] = newError[i];
        newError[i] = expectedSpeed[i] - actualSpeed[i];
        errorDel[i] = (newError[i] - oldError[i]) / (newTime[i] - oldTime[i]) * 1000;
        errorSum[i] += newError[i];
        PTerm[i] = newError[i] * PConst[i];
        ITerm[i] = errorSum[i] * IConst[i];
        DTerm[i] = errorDel[i] * DConst[i];
        power[i] = std::min(std::max(power[i] + PTerm[i] + ITerm[i] + DTerm[i], 0.), 50.);
        motor[i].SetPercent(power[i] * direction[i]);
    }
}

void vectorDirection(double x, double y){
    double vx = x;
    double vy = y;
    expectedSpeed[0] = fabs(vx);
    expectedSpeed[1] = fabs(vx * cos(M_PI / 3) - vy * sin(M_PI / 3));
    expectedSpeed[2] = fabs(vx * cos(M_PI / 3) + vy * sin(M_PI / 3));
    direction[0] = vx >= 0 ? -1 : 1;
    direction[1] = (vx * cos(M_PI / 3) - vy * sin(M_PI / 3) >= 0) ? 1 : -1;
    direction[2] = (vx * cos(M_PI / 3) + vy * sin(M_PI / 3) >= 0) ? 1 : -1;
    // for(int i = 0; i < 3; ++i){
    //     power[i] = expectedSpeed[i] * 4.5 * direction[i];
    //     motor[i].SetPercent(expectedSpeed[i] * 4.5 * direction[i]);
    // }
    // For some reason, trying to initialize the power makes the whole thing super inconsistent, safer to let it ramp up to speed
}

void motorSetPercent(double initPower){
    for(int i = 0; i < 3; ++i){
        power[i] = initPower;
        motor[i].SetPercent(power[i] * direction[i]);
    }
}

void motorStop(){
    for(int i = 0; i < 3; ++i){
        motor[i].Stop();
    }
}

int currentDebugSection = 0;
void DebugLogSection(FEHFile *overview, FEHFile *detailed, int sectionNumber)
{
    if (currentDebugSection != sectionNumber)
    {
        SD.FPrintf(overview, "Beginning Section %d\nCurrent Time: %f\n\n", sectionNumber, TimeNow());
        SD.FPrintf(detailed, "Beginning Section %d\nCurrent Time: %f\n\n", sectionNumber, TimeNow());
        currentDebugSection = sectionNumber;
    }
    SD.FPrintf(detailed, "Front: Expected - %f, Actual - %f\n", expectedSpeed[0], actualSpeed[0]);
    SD.FPrintf(detailed, "Left: Expected - %f, Actual - %f\n", expectedSpeed[1], actualSpeed[1]);
    SD.FPrintf(detailed, "Right: Expected - %f, Actual - %f\n\n", expectedSpeed[2], actualSpeed[2]);
}

void FinalizeDebugging(FEHFile *overview, int sectionNumber)
{
    SD.FPrintf(overview, "Section stats:\n\n");
    SD.FPrintf(overview, "Front Wheel:\n");
    SD.FPrintf(overview, "Distance Travelled: %f\n", (double)encoder[0].Counts() * inchPerCount);
    SD.FPrintf(overview, "Final Speed: %f\n\n", actualSpeed[0]);

    SD.FPrintf(overview, "Left Wheel:\n");
    SD.FPrintf(overview, "Distance Travelled: %f\n", (double)encoder[1].Counts() * inchPerCount);
    SD.FPrintf(overview, "Final Speed: %f\n\n", actualSpeed[1]);

    
    SD.FPrintf(overview, "Right Wheel:\n");
    SD.FPrintf(overview, "Distance Travelled: %f\n", (double)encoder[2].Counts() * inchPerCount);
    SD.FPrintf(overview, "Final Speed: %f\n\n", actualSpeed[2]);

    SD.FPrintf(overview, "Ending Section %d\nCurrent Time: %f\n\n", sectionNumber, TimeNow());
}

int main(){
    FEHFile *detailedFptr = SD.FOpen("Detailed.txt","w");
    FEHFile *overviewFptr = SD.FOpen("Overview.txt","w");

    armServo.SetMin(500);
    armServo.SetMax(2500);

    SD.FPrintf(overviewFptr, "Power on. Battery voltage: %f\nEstimated Percentage: %f\n\n", Battery.Voltage(), (Battery.Voltage()-10.)/1.5);

    //start menu
    zero();
    LCD.Clear();
    while(!text.button("Touch me", 0xffffff, 136, 100)){}
    text.display("Touch Detected, Starting Soon", 0xff0000, 50, 58);
    Sleep(3.0);

    //initial move up
    armServo.SetDegree(180.);
    zero();
    vectorDirection(0, 6);

    while((newCount[1] + newCount[2]) / 2 * inchPerCount < 3){
        speedPID();
        if(power[1] > 45 || power[2] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    motorStop();

    //move left
    zero();
    vectorDirection(-6, 0);

    while(newCount[0] * inchPerCount < 13){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 1);
        if(power[0] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 1);
    motorStop();
    

    //move up to the trunk
    zero();
    vectorDirection(0, 6);

    while((newCount[1] + newCount[2]) / 2 * inchPerCount < 8){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 2);
        if(power[1] > 45 || power[2] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 2);
    motorStop();

    //arm up
    armServo.SetDegree(153.);

    //left translation to pick up bucket
    zero();
    vectorDirection(-6, 0);

    while(newCount[0] * inchPerCount < 7){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 3);
        if(power[0] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 3);
    motorStop();

    //arm up pick up bucket
    armServo.SetDegree(110.);
    Sleep(0.5);

    //move back after pick up bucket
    zero();
    vectorDirection(0, -6);

    while(newCount[0] * inchPerCount < 5){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 4);
        if(power[0] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 4);
    motorStop();

    //move right to the ramp
    zero();
    vectorDirection(6, 0);

    while(newCount[0] * inchPerCount < 13){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 5);
        if(power[0] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 5);
    motorStop();

    //move up the ramp
    zero();
    vectorDirection(0, 6);

    while(newCount[0] * inchPerCount < 10){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 6);
        if(power[0] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 6);
    motorStop();

    //rotate 90 degrees to align the arm to the table
    zero();
    expectedSpeed[0] = 4;
    expectedSpeed[1] = 4;
    expectedSpeed[2] = 4;
    direction[0] = -1;
    direction[1] = -1;
    direction[2] = -1;

    while((encoder[0].Counts() + encoder[1].Counts() + encoder[2].Counts()) / 3 * inchPerCount < 2.65 * M_PI / 2){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 7);
        if(power[0] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 7);
    motorStop();

    //arm down to put down bucket
    armServo.SetDegree(115.);
    Sleep(0.5);

    //backwards translation, leave bucket on table
    zero();
    vectorDirection(0, -6);

    while(newCount[0] * inchPerCount < 3.5){
        speedPID();
        DebugLogSection(overviewFptr, detailedFptr, 8);
        if(power[0] > 45){
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }
    FinalizeDebugging(overviewFptr, 8);
    motorStop();

    //left translation to align with fertilizer levers
    int lever = RCS.GetLever();
    SD.FPrintf(overviewFptr, "Lever: %f\n\n", lever);



    // //backward touching the window
    // zero();
    // expectedSpeed[1] = 6;
    // expectedSpeed[2] = 6;
    // direction[1] = 1;
    // direction[2] = -1;
    // power[1] = 20;
    // power[2] = 20;
    // motor[1].SetPercent(power[1] * direction[1]);
    // motor[2].SetPercent(power[2] * direction[2]);
    // while((newCount[1] + newCount[2]) / 2 * inchPerCount < 2){
    //     speedPID();
    //     if(power[0] > 45){
    //         break;
    //     }
    // }
    // motorStop();

    // //left translation, open the window
    // zero();
    // vx = -4;
    // vy = 0;
    // expectedSpeed[0] = fabs(vx);
    // expectedSpeed[1] = fabs(vx * cos(M_PI / 3) - vy * sin(M_PI / 3));
    // expectedSpeed[2] = fabs(vx * cos(M_PI / 3) + vy * sin(M_PI / 3));
    // direction[0] = vx >= 0 ? -1 : 1;
    // direction[1] = vx * cos(M_PI / 3) - vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    // direction[2] = vx * cos(M_PI / 3) + vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    
    // motorSetPercent(15);

    // while(newCount[0] * inchPerCount < 8){
    //     speedPID();
    //     if(power[0] > 45){
    //         LCD.SetBackgroundColor(RED);
    //         LCD.Clear();
    //         break;
    //     }
    // }
    // motorStop();

    // //after open window, go forward
    // zero();
    // expectedSpeed[1] = 6;
    // expectedSpeed[2] = 6;
    // direction[1] = -1;
    // direction[2] = 1;
    // power[1] = 20;
    // power[2] = 20;
    // motor[1].SetPercent(power[1] * direction[1]);
    // motor[2].SetPercent(power[2] * direction[2]);
    // while((newCount[1] + newCount[2]) / 2 * inchPerCount < 2){
    //     speedPID();
    //     if(power[0] > 45){
    //         break;
    //     }
    // }
    // for(int i = 0; i < 3; ++i){
    //     motor[i].Stop();
    // }

    // //after open window, left translation
    // zero();
    // vx = -4;
    // vy = 0;
    // expectedSpeed[0] = fabs(vx);
    // expectedSpeed[1] = fabs(vx * cos(M_PI / 3) - vy * sin(M_PI / 3));
    // expectedSpeed[2] = fabs(vx * cos(M_PI / 3) + vy * sin(M_PI / 3));
    // direction[0] = vx >= 0 ? -1 : 1;
    // direction[1] = vx * cos(M_PI / 3) - vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    // direction[2] = vx * cos(M_PI / 3) + vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    
    // for(int i = 0; i < 3; ++i){
    //     power[i] = 15;
    //     motor[i].SetPercent(power[i] * direction[i]);
    // }

    // while(newCount[0] * inchPerCount < 0.5){
    //     speedPID();
    //     if(power[0] > 45){
    //         LCD.SetBackgroundColor(RED);
    //         LCD.Clear();
    //         break;
    //     }
    // }
    // for(int i = 0; i < 3; ++i){
    //     motor[i].Stop();
    // }

    // //after open window, backward touching the window
    // zero();
    // expectedSpeed[1] = 6;
    // expectedSpeed[2] = 6;
    // direction[1] = 1;
    // direction[2] = -1;
    // power[1] = 20;
    // power[2] = 20;
    // motor[1].SetPercent(power[1] * direction[1]);
    // motor[2].SetPercent(power[2] * direction[2]);
    // while((newCount[1] + newCount[2]) / 2 * inchPerCount < 2){
    //     speedPID();
    //     if(power[0] > 45){
    //         break;
    //     }
    // }
    // for(int i = 0; i < 3; ++i){
    //     motor[i].Stop();
    // }

    // //right translation, close the window
    // zero();
    // vx = 4;
    // vy = 0;
    // expectedSpeed[0] = fabs(vx);
    // expectedSpeed[1] = fabs(vx * cos(M_PI / 3) - vy * sin(M_PI / 3));
    // expectedSpeed[2] = fabs(vx * cos(M_PI / 3) + vy * sin(M_PI / 3));
    // direction[0] = vx >= 0 ? -1 : 1;
    // direction[1] = vx * cos(M_PI / 3) - vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    // direction[2] = vx * cos(M_PI / 3) + vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    
    // for(int i = 0; i < 3; ++i){
    //     power[i] = 15;
    //     motor[i].SetPercent(power[i] * direction[i]);
    // }

    // while(newCount[0] * inchPerCount < 6){
    //     speedPID();
    //     if(power[0] > 45){
    //         LCD.SetBackgroundColor(RED);
    //         LCD.Clear();
    //         break;
    //     }
    // }
    // for(int i = 0; i < 3; ++i){
    //     motor[i].Stop();
    // }

    // //after close window, go forward
    // zero();
    // expectedSpeed[1] = 6;
    // expectedSpeed[2] = 6;
    // direction[1] = -1;
    // direction[2] = 1;
    // power[1] = 20;
    // power[2] = 20;
    // motor[1].SetPercent(power[1] * direction[1]);
    // motor[2].SetPercent(power[2] * direction[2]);
    // while((newCount[1] + newCount[2]) / 2 * inchPerCount < 2){
    //     speedPID();
    //     if(power[0] > 45){
    //         break;
    //     }
    // }
    // for(int i = 0; i < 3; ++i){
    //     motor[i].Stop();
    // }

    // //right translation
    // zero();
    // vx = 4;
    // vy = 0;
    // expectedSpeed[0] = fabs(vx);
    // expectedSpeed[1] = fabs(vx * cos(M_PI / 3) - vy * sin(M_PI / 3));
    // expectedSpeed[2] = fabs(vx * cos(M_PI / 3) + vy * sin(M_PI / 3));
    // direction[0] = vx >= 0 ? -1 : 1;
    // direction[1] = vx * cos(M_PI / 3) - vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    // direction[2] = vx * cos(M_PI / 3) + vy * sin(M_PI / 3) >= 0 ? 1 : -1;
    
    // for(int i = 0; i < 3; ++i){
    //     power[i] = 15;
    //     motor[i].SetPercent(power[i] * direction[i]);
    // }

    // while(newCount[0] * inchPerCount < 12){
    //     speedPID();
    //     if(power[0] > 45){
    //         LCD.SetBackgroundColor(RED);
    //         LCD.Clear();
    //         break;
    //     }
    // }
    // for(int i = 0; i < 3; ++i){
    //     motor[i].Stop();
    // }

    // //down slope back to start
    // zero();
    // expectedSpeed[1] = 6;
    // expectedSpeed[2] = 6;
    // direction[1] = 1;
    // direction[2] = -1;
    // power[1] = 20;
    // power[2] = 20;
    // motor[1].SetPercent(power[1] * direction[1]);
    // motor[2].SetPercent(power[2] * direction[2]);
    // while((newCount[1] + newCount[2]) / 2 * inchPerCount < 35){
    //     speedPID();
    //     if(power[0] > 45){
    //         break;
    //     }
    // }
    for(int i = 0; i < 3; ++i){
        motor[i].Stop();
    }

    SD.FClose(overviewFptr);
    SD.FClose(detailedFptr);
    return 0;
}