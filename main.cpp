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
// #include <text.h>

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
    DigitalEncoder(FEHIO::P0_1),
    DigitalEncoder(FEHIO::P0_2),
    DigitalEncoder(FEHIO::P0_3),
};
FEHMotor motor[3] = {
    FEHMotor(FEHMotor::Motor0, 9.0),
    FEHMotor(FEHMotor::Motor1, 9.0),
    FEHMotor(FEHMotor::Motor2, 9.0),
};
FEHServo armServo(FEHServo::Servo0);
FEHServo prongServo(FEHServo::Servo1);
AnalogInputPin CdS = AnalogInputPin(FEHIO::P0_0);


double inchPerCount = 2.5 * M_PI / 318;

//{Front, Left, Right}
double newError[3] = {0, 0, 0}, oldError[3] = {0, 0, 0}, errorSum[3] = {0, 0, 0}, errorDel[3] = {0, 0, 0};
double PTerm[3] = {0, 0, 0}, ITerm[3] = {0, 0, 0}, DTerm[3] = {0, 0, 0};
double PConst[3] = {0.4, 0.4, 0.4}, IConst[3] = {0.02, 0.02, 0.02}, DConst[3] = {0.01, 0.01, 0.01};

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

// Text text;

float CDS()
{
    return CdS.Value();
}

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



void motorStop(){
    for(int i = 0; i < 3; ++i){
        motor[i].Stop();
    }
}

std::string currentDebugSection = "";
void DebugLogSection(FEHFile *overview, FEHFile *detailed, std::string sectionName)
{
    if (currentDebugSection != sectionName)
    {
        SD.FPrintf(overview, "START Section:\n%s\nCurrent Time: %f\n\n", sectionName.c_str(), TimeNow());
        SD.FPrintf(detailed, "START Section:\n%s\nCurrent Time: %f\n\n", sectionName.c_str(), TimeNow());
        currentDebugSection = sectionName;
    }
    SD.FPrintf(detailed, "Front: Expected - %f, Actual - %f\n", expectedSpeed[0], actualSpeed[0]);
    SD.FPrintf(detailed, "Left: Expected - %f, Actual - %f\n", expectedSpeed[1], actualSpeed[1]);
    SD.FPrintf(detailed, "Right: Expected - %f, Actual - %f\n\n", expectedSpeed[2], actualSpeed[2]);
}

void FinalizeDebugging(FEHFile *overview, std::string sectionName)
{
    SD.FPrintf(overview, "Section stats:\n\n");
    SD.FPrintf(overview, "Front Wheel:\n");
    SD.FPrintf(overview, "Distance Travelled: %f\n", (double)newCount[0] * inchPerCount);
    SD.FPrintf(overview, "Final Speed: %f\n\n", actualSpeed[0]);

    SD.FPrintf(overview, "Left Wheel:\n");
    SD.FPrintf(overview, "Distance Travelled: %f\n", (double)newCount[1] * inchPerCount);
    SD.FPrintf(overview, "Final Speed: %f\n\n", actualSpeed[1]);

    
    SD.FPrintf(overview, "Right Wheel:\n");
    SD.FPrintf(overview, "Distance Travelled: %f\n", (double)newCount[2] * inchPerCount);
    SD.FPrintf(overview, "Final Speed: %f\n\n", actualSpeed[2]);

    SD.FPrintf(overview, "END Section:\n%s\nCurrent Time: %f\n\n\n", sectionName.c_str(), TimeNow());
}

void moveVectorDistance(double x, double y, double targetDistance, const std::string& debugName, FEHFile *overview, FEHFile *detailed) {
    zero();
    vectorDirection(x, y);

    double projx = 0;
    double projy = 0;

    while(projx * projx + projy * projy < targetDistance * targetDistance){
        speedPID();
        // projx = inchPerCount * (newCount[0] + newCount[1] * cos(M_PI / 3) + newCount[2] * cos(M_PI / 3));
        // projy = inchPerCount * (newCount[1] * sin(M_PI / 3) + newCount[2] * sin(M_PI / 3));
        projx += inchPerCount * (-2 * (newCount[0] - oldCount[0]) + (1 - 2 / sqrt(3)) * (newCount[1] - oldCount[1]) + (1 + 2 / sqrt(3)) * (newCount[2] - oldCount[2]));
        projy += inchPerCount * (1 / sqrt(3) * (newCount[1] - oldCount[1]) - 1 / sqrt(3) * (newCount[2] - oldCount[2]));
        DebugLogSection(overview, detailed, debugName);
        
        minCDS = std::min(minCDS, CdS.Value());



        if (power[0] > 45) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Front wheel: Power over 45");
            break;
        }
        if (power[1] > 45) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Left wheel: Power over 45");
            break;
        }
        if (power[2] > 45) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Right Wheel: Power over 45");
            break;
        }
    }

    // while (true) {
    //     totalWeightedDistance = 0;
    //     contributingWheels = 0;

    //     // Calculate weighted encoder distance
    //     for (int i = 0; i < 3; i++) {
    //         if (expectedSpeed[i] > 0) { // Avoid division by zero
    //             totalWeightedDistance += newCount[i] / expectedSpeed[i];
    //             contributingWheels++;
    //         }
    //     }

    //     if (contributingWheels > 0) {
    //         double avgDistance = (totalWeightedDistance / contributingWheels) * inchPerCount * 3;
    //         if (avgDistance >= targetDistance) break;
    //     }

    //     speedPID(); // Adjust motor speeds
    //     DebugLogSection(overview, detailed, debugName);
    //     // Emergency stop condition
    //     if (power[0] > 45 || power[1] > 45 || power[2] > 45) {
    //         LCD.SetBackgroundColor(RED);
    //         LCD.Clear();
    //         break;
    //     }
    // }
    FinalizeDebugging(overview, debugName);
    motorStop();
    Sleep(0.5);
}

void rotateDegrees(double degrees, FEHFile *overview, FEHFile *detailed) {
    zero();  // Reset encoders

    double robotRadius = 3.9375; // Adjust based on your robot's actual radius (in inches)
    double arcLength = (2 * M_PI * robotRadius * fabs(degrees)) / 360.0;

    // Set wheel directions for rotation (all wheels move the same way)
    expectedSpeed[0] = 5;
    expectedSpeed[1] = 5;
    expectedSpeed[2] = 5;
    
    int rotationDirection = (degrees > 0) ? -1 : 1; // Counterclockwise (-1) for positive, clockwise (+1) for negative
    direction[0] = rotationDirection;
    direction[1] = rotationDirection;
    direction[2] = rotationDirection;

    double totalDistance = 0;
    std::string debugName = std::to_string((int)degrees) + " degree rotation";

    while (true) {
        // Calculate total encoder distance traveled
        totalDistance = (newCount[0] + newCount[1] + newCount[2]) / 3.0 * inchPerCount;

        if (totalDistance >= arcLength) break;

        speedPID(); // Adjust motor speeds
        DebugLogSection(overview, detailed, debugName);

        // Emergency stop condition
        if (power[0] > 45 || power[1] > 45 || power[2] > 45) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            break;
        }
    }

    FinalizeDebugging(overview, debugName);
    motorStop();
    Sleep(0.5);
}

void servoSetDegree(int angle0, int angle1){
    if(angle0 < angle1){
        for (int i = angle0; i <= angle1; i++){
            prongServo.SetDegree(i);
        }
    }
    else{
        for (int i = angle0; i >= angle1; i--){
            prongServo.SetDegree(i);
        }
    }
    
    Sleep(1.);
}



int main(){
    FEHFile *detailedFptr = SD.FOpen("Detailed.txt","w");
    FEHFile *overviewFptr = SD.FOpen("Overview.txt","w");
    
    armServo.SetMin(500);
    armServo.SetMax(2500);
    prongServo.SetMin(820);
    prongServo.SetMax(2205);

    SD.FPrintf(overviewFptr, "Power on. Battery voltage: %f\nEstimated Percentage: %f\n\n", Battery.Voltage(), (Battery.Voltage()-10.)/1.5*100.);

    zero();
    LCD.Clear();
    // Wait for light
    prongServo.SetDegree(0);
    while (CDS() > 0.9){}

    // Start
    moveVectorDistance(0, -6, 1.5, std::string("Back Up Into Start Button"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 2.5, std::string("Move Forward"), overviewFptr, detailedFptr);

    // Compost bin
    rotateDegrees(-135, overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 5, std::string("Move closer to compost bin"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 9, std::string("Move into wall, normalize"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -3, 0.75, std::string("Back up slightly"), overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 4.5, std::string("Move to compost bin"), overviewFptr, detailedFptr);
    servoSetDegree(1, 150);
    moveVectorDistance(-6, 0, 1.25, std::string("Move away from compost bin"), overviewFptr, detailedFptr);
    servoSetDegree(150, 1);
    moveVectorDistance(6, 0, 1.25, std::string("Move to compost bin"), overviewFptr, detailedFptr);
    servoSetDegree(1, 150);
    moveVectorDistance(-6, 0, 1.25, std::string("Move away from compost bin"), overviewFptr, detailedFptr);
    servoSetDegree(150, 1);
    moveVectorDistance(6, 0, 1.25, std::string("Move to compost bin"), overviewFptr, detailedFptr);
    servoSetDegree(1, 150);
    moveVectorDistance(-6, 0, 1.25, std::string("Move away from compost bin"), overviewFptr, detailedFptr);

    // Apple bucket
    moveVectorDistance(0, -6, 23, std::string("Back up"), overviewFptr, detailedFptr);
    rotateDegrees(180, overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 8, std::string("Move left to the trunk"), overviewFptr, detailedFptr);

    // Window
    SD.FPrintf(overviewFptr, "\nSTART MAJOR Window section\n\n\n");
    moveVectorDistance(0, 6, 3, std::string("Normalize into right wall"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 8, std::string("Move closer to window"), overviewFptr, detailedFptr);
    rotateDegrees(-90, overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 8, std::string("Back into window"), overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 7, std::string("Open window"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 1, std::string("Inch forward out of window"), overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 1, std::string("Inch left of window"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 1, std::string("Inch back to window"), overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 7, std::string("Close window"), overviewFptr, detailedFptr);

    // Red/Blue button
    SD.FPrintf(overviewFptr, "\nSTART MAJOR Red/Blue button section\n\n\n");
    moveVectorDistance(0, 6, 5, std::string("Inch forward out of window, align with black stripe"), overviewFptr, detailedFptr);
    rotateDegrees(-90, overviewFptr, detailedFptr);
    minCDS = 3.0;
    moveVectorDistance(0, 3, 5, std::string("Inch forward to find light"), overviewFptr, detailedFptr);
    if (minCDS < 0.48)
    {
        //Red light seen
        SD.FPrintf(overviewFptr, "\nRed light seen!\n\n");

        moveVectorDistance(3, 0, 2, std::string("Shift right for red button"), overviewFptr, detailedFptr);
    }
    else
    {
        //Go for blue
        if (minCDS < 0.9)
        {
            SD.FPrintf(overviewFptr, "\nBlue light seen!\n\n");
        }
        else
        {
            SD.FPrintf(overviewFptr, "\nNo light seen!\n\n");
        }
        
        moveVectorDistance(-3, 0, 2, std::string("Shift left for blue button"), overviewFptr, detailedFptr);
    }
    moveVectorDistance(0, 6, 7, std::string("Go to touch button"), overviewFptr, detailedFptr);


    // Lever flip
    int lever = RCS.GetLever();
    // keep going...

    for(int i = 0; i < 3; ++i){
        motor[i].Stop();
    }

    SD.FClose(overviewFptr);
    SD.FClose(detailedFptr);
    return 0;
}