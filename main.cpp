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

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

void fiveRevolution(){
    FEHMotor frontMotor = FEHMotor(FEHMotor::Motor0, 9.0);
    DigitalEncoder frontEncoder = DigitalEncoder(FEHIO::P0_0);

    frontMotor.SetPercent(20.);

    while(frontEncoder.Counts() <= 318 * 5){
        LCD.WriteLine(frontEncoder.Counts());
    }
    frontMotor.Stop();
}

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
        projx += inchPerCount * (-2 * (newCount[0] - oldCount[0]) + (1 - 2 / sqrt(3)) * (newCount[1] - oldCount[1]) + (1 + 2 / sqrt(3)) * (newCount[2] - oldCount[2]));
        projy += inchPerCount * (1 / sqrt(3) * (newCount[1] - oldCount[1]) - 1 / sqrt(3) * (newCount[2] - oldCount[2]));
        DebugLogSection(overview, detailed, debugName);

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
    FinalizeDebugging(overview, debugName);
    motorStop();
    Sleep(0.5);
}

void rotateDegrees(double degrees, FEHFile *overview, FEHFile *detailed) {
    zero();  // Reset encoders

    double robotRadius = 3.91875; // Adjust based on your robot's actual radius (in inches)
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
    
    Sleep(0.5);
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
    for(int i = 0; i < 2; ++i){
        moveVectorDistance(-6, 0, 1.25, std::string("Move away from compost bin"), overviewFptr, detailedFptr);
        servoSetDegree(150, 1);
        moveVectorDistance(0, 3, 1.5, std::string("Move into wall, slightly normalize"), overviewFptr, detailedFptr);
        moveVectorDistance(0, -3, 0.75, std::string("Back up slightly"), overviewFptr, detailedFptr);
        moveVectorDistance(6, 0, 1.25, std::string("Move to compost bin"), overviewFptr, detailedFptr);
        servoSetDegree(1, 150);
    }
    moveVectorDistance(-6, 0, 1.25, std::string("Move away from compost bin"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 3, 1.5, std::string("Move into wall, slightly normalize"), overviewFptr, detailedFptr);

    // Apple bucket
    moveVectorDistance(0, -6, 28, std::string("Back up"), overviewFptr, detailedFptr);
    rotateDegrees(180, overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 5, std::string("Move left to the trunk"), overviewFptr, detailedFptr);

    for(int i = 0; i < 3; ++i){
        motor[i].Stop();
    }

    SD.FClose(overviewFptr);
    SD.FClose(detailedFptr);
    return 0;
}