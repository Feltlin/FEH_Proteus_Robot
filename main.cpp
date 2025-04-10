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

DigitalEncoder encoder[3] = {
    DigitalEncoder(FEHIO::P0_2),
    DigitalEncoder(FEHIO::P0_4),
    DigitalEncoder(FEHIO::P0_7),
};
FEHMotor motor[3] = {
    FEHMotor(FEHMotor::Motor0, 9.0),
    FEHMotor(FEHMotor::Motor1, 9.0),
    FEHMotor(FEHMotor::Motor2, 9.0),
};
FEHServo armServo(FEHServo::Servo7);
FEHServo prongServo(FEHServo::Servo0);
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
// Relative x,y coordinates and angle of the robot to the course
double relax = 0, relay = 0, relangle = 0;

int newCount[3] = {0, 0, 0}, oldCount[3] = {0, 0, 0};

unsigned long newTime[3] = {0, 0, 0}, oldTime[3] = {0, 0, 0};

bool switchingState = false;
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
        SD.FPrintf(overview, "-------------------------------------------------------------------\n");
        SD.FPrintf(overview, "%s\n\n", sectionName.c_str());
        SD.FPrintf(detailed, "Begin Section:\n%s\nCurrent Time: %f\n\n", sectionName.c_str(), TimeNow());
        
        currentDebugSection = sectionName;
    }
    SD.FPrintf(detailed, "Front: Expected - %f, Actual - %f\n", expectedSpeed[0], actualSpeed[0]);
    SD.FPrintf(detailed, "Left: Expected - %f, Actual - %f\n", expectedSpeed[1], actualSpeed[1]);
    SD.FPrintf(detailed, "Right: Expected - %f, Actual - %f\n\n", expectedSpeed[2], actualSpeed[2]);
}

void FinalizeDebugging(FEHFile *overview, std::string sectionName)
{
    SD.FPrintf(overview, "\tFront: %f | Left: %f | Right: %f\n", (double)newCount[0] * inchPerCount, (double)newCount[1] * inchPerCount, (double)newCount[2] * inchPerCount);
    SD.FPrintf(overview, "Final x: %f | Final y: %f | Final angle: %f \n\n", relax, relay, relangle * 180 / M_PI);
    SD.FPrintf(overview, "\tEnd Time: %f\n\n", sectionName.c_str(), TimeNow());
}

double armStepSize = 0.5;

void moveVectorDistance(double x, double y, double targetDistance, const std::string& debugName, FEHFile *overview, FEHFile *detailed, float startAngle = -1, float endAngle = -1) {
    zero();
    vectorDirection(x, y);

    double projx = 0;
    double projy = 0;

    double powerThreshold = 55;

    while(projx * projx + projy * projy < targetDistance * targetDistance){
        speedPID();
        // projx = inchPerCount * (newCount[0] + newCount[1] * cos(M_PI / 3) + newCount[2] * cos(M_PI / 3));
        // projy = inchPerCount * (newCount[1] * sin(M_PI / 3) + newCount[2] * sin(M_PI / 3));
        projx += inchPerCount * (-2 * (newCount[0] - oldCount[0]) + (1 - 2 / sqrt(3)) * (newCount[1] - oldCount[1]) + (1 + 2 / sqrt(3)) * (newCount[2] - oldCount[2]));
        projy += inchPerCount * (1 / sqrt(3) * (newCount[1] - oldCount[1]) - 1 / sqrt(3) * (newCount[2] - oldCount[2]));
        
        bool angleNotReached = true;
        if (startAngle != -1 && endAngle != -1 && angleNotReached)
        {
            if (startAngle < endAngle)
            {
                if (fabs(startAngle-endAngle) < armStepSize)
                {
                    startAngle = endAngle;
                }
                else
                {
                    startAngle += armStepSize;
                }
                armServo.SetDegree(startAngle);
            }
            else if (endAngle < startAngle)
            {
                if (fabs(startAngle-endAngle) < armStepSize)
                {
                    startAngle = endAngle;
                }
                else
                {
                    startAngle -= armStepSize;
                }
            }
            else
            {
                angleNotReached = false;
            }
        }

        DebugLogSection(overview, detailed, debugName);
        
        minCDS = std::min(minCDS, CdS.Value());

        if (power[0] > powerThreshold) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Front wheel: Power over threshold");
            SD.FPrintf(overview, "Front wheel power over threshold\n");
            break;
        }
        if (power[1] > powerThreshold) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Left wheel: Power over threshold");
            SD.FPrintf(overview, "Left wheel power over threshold\n");
            break;
        }
        if (power[2] > powerThreshold) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Right Wheel: Power over threshold");
            SD.FPrintf(overview, "Right wheel power over threshold\n");
            break;
        }
    }
    relax += sqrt(projx * projx + projy * projy) * cos(relangle + atan2(y, x) - M_PI_2);
    relay += sqrt(projx * projx + projy * projy) * sin(relangle + atan2(y, x) - M_PI_2);
    FinalizeDebugging(overview, debugName);
    motorStop();
    Sleep(0.5);
}

void rotateDegrees(double degrees, FEHFile *overview, FEHFile *detailed) {
    zero();  // Reset encoders

    double robotRadius = 3.9375; // Adjust based on your robot's actual radius (in inches)

    // Set wheel directions for rotation (all wheels move the same way)
    expectedSpeed[0] = 5;
    expectedSpeed[1] = 5;
    expectedSpeed[2] = 5;
    
    int rotationDirection = (degrees > 0) ? 1 : -1; // Counterclockwise (1) for positive, clockwise (-1) for negative
    direction[0] = rotationDirection;
    direction[1] = rotationDirection;
    direction[2] = rotationDirection;

    std::string debugName = std::to_string((double)degrees) + " degree rotation";
    double projangle = 0;
    degrees = fabs(degrees);

    double powerThreshold = 55;

    while (projangle < degrees * M_PI / 180) {
        speedPID(); // Adjust motor speeds
        projangle += inchPerCount * ((newCount[0] - oldCount[0]) / robotRadius + (newCount[1] - oldCount[1]) / (sqrt(3) * robotRadius) - (newCount[2] - oldCount[2]) / (sqrt(3) * robotRadius));
        DebugLogSection(overview, detailed, debugName);

        if (power[0] > powerThreshold) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Front wheel: Power over threshold");
            SD.FPrintf(overview, "Front wheel power over threshold\n");
            break;
        }
        if (power[1] > powerThreshold) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Left wheel: Power over threshold");
            SD.FPrintf(overview, "Left wheel power over threshold\n");
            break;
        }
        if (power[2] > powerThreshold) {
            LCD.SetBackgroundColor(RED);
            LCD.Clear();
            LCD.WriteLine("Right Wheel: Power over threshold");
            SD.FPrintf(overview, "Right wheel power over threshold\n");
            break;
        }
    }
    relangle += projangle * rotationDirection;
    // Normalize relative angle
    while (relangle >= 2 * M_PI) relangle -= 2 * M_PI;
    while (relangle < 0) relangle += 2 * M_PI;
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
    
    Sleep(0.2);
}

void moveToCoord(double x, double y, const std::string& debugName, FEHFile *overview, FEHFile *detailed){
    double distance = sqrt((x - relax) * (x - relax) + (y - relay) * (y - relay));
    double relaVectorAngle = atan2(y - relay, x - relax);
    double robotVectorAngle = relaVectorAngle - relangle + M_PI_2;
    moveVectorDistance(cos(robotVectorAngle) * 6, sin(robotVectorAngle) * 6, distance, debugName, overview, detailed);
}

void rotateToAngle(double degree, FEHFile *overview, FEHFile *detailed){
    double degreeToTurn = degree - relangle * 180 / M_PI;

    // Normalize to [-180, 180)
    while (degreeToTurn >= 180.) degreeToTurn -= 360.;
    while (degreeToTurn < -180.) degreeToTurn += 360.;
    
    rotateDegrees(degreeToTurn, overview, detailed);
}

int main(){
    FEHFile *detailedFptr = SD.FOpen("Detailed.txt","w");
    FEHFile *overviewFptr = SD.FOpen("Overview.txt","w");
    
    armServo.SetMin(500);
    armServo.SetMax(2500);
    prongServo.SetMin(820);
    prongServo.SetMax(2205);

    SD.FPrintf(overviewFptr, "Power on. Battery voltage: %f\nEstimated Percentage: %f\n\n", Battery.Voltage(), (Battery.Voltage()-10.2)/0.7*100.);

    zero();
    LCD.Clear();
    RCS.InitializeTouchMenu("1020C8WIE");

    // Wait for light

    // CDS cell values:
    // 2.1 = nothing
    // 1.2-1.5 = blue
    // 0.54-0.75 = red

    float blueMax = 1.5;
    float redMax = 1.0;

    while (CDS() > blueMax){}

    // Find the relative starting point x,y coordinates
    relax = 0;
    relay = 0;
    relangle = 135 * M_PI / 180;

    // Start
    relangle = 135;
    armServo.SetDegree(40);
    prongServo.SetDegree(0);
    moveVectorDistance(0, -6, 1.5, std::string("Back Up Into Start Button"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 2.5, std::string("Move Forward"), overviewFptr, detailedFptr);

    // Compost bin
    rotateDegrees(135, overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 6, std::string("Move closer to compost bin"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 9, std::string("Move into wall, normalize"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -3, 0.75, std::string("Back up slightly"), overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 4.5, std::string("Move to compost bin"), overviewFptr, detailedFptr);
    servoSetDegree(1, 155);
    for(int i = 0; i < 2; ++i){
        moveVectorDistance(-6, 0, 1.25, std::string("Move away from compost bin"), overviewFptr, detailedFptr);
        servoSetDegree(150, 1);
        moveVectorDistance(0, 3, 1.5, std::string("Move into wall, slightly normalize"), overviewFptr, detailedFptr);
        moveVectorDistance(0, -3, 0.7, std::string("Back up slightly"), overviewFptr, detailedFptr);
        moveVectorDistance(6, 0, 1.25, std::string("Move to compost bin"), overviewFptr, detailedFptr);
        servoSetDegree(1, 150);
    }
    moveVectorDistance(-6, 0, 1.25, std::string("Move away from compost bin"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 3, 1.5, std::string("Move into wall, slightly normalize"), overviewFptr, detailedFptr);

    // Apple bucket
    moveVectorDistance(0, -6, 29.5, std::string("Back up"), overviewFptr, detailedFptr);
    // 40 to 158
    for (int i=40; i<158; ++i)
    {
        armServo.SetDegree(i);
        Sleep(0.025);
    }
    armServo.SetDegree(158);
    rotateDegrees(-180, overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 6.25, std::string("Move left to the trunk"), overviewFptr, detailedFptr);
    armServo.SetDegree(130);
    for (int i=130; i>60; --i)
    {
        armServo.SetDegree(i);
        Sleep(0.025);
    }
    moveVectorDistance(0, -6, 8, std::string("Move backward"), overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 20.375, std::string("Move right to the wall to go up ramp"), overviewFptr, detailedFptr);
    armServo.SetDegree(60);
    // rotateDegrees(90, overviewFptr, detailedFptr);
    // moveVectorDistance(0, -4, 3, std::string("Normalize against wall"), overviewFptr, detailedFptr);
    // moveVectorDistance(0, 4, 2, std::string("Get away from right wall to rotate"), overviewFptr, detailedFptr);
    // rotateDegrees(-90, overviewFptr, detailedFptr);
    moveVectorDistance(0, 7, 30, std::string("Go up ramp"), overviewFptr, detailedFptr);
    rotateDegrees(90, overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 4, std::string("Normalize into right wall"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 2, std::string("Get away from right wall"), overviewFptr, detailedFptr);
    rotateDegrees(190, overviewFptr, detailedFptr);
    for (int i=60; i<130; i++)
    {
        armServo.SetDegree(i);
        Sleep(0.025);
    }
    armServo.SetDegree(130);
    Sleep(0.5);
    armServo.SetDegree(122);
    Sleep(0.5);
    moveVectorDistance(4.5, 0, 7.5, std::string("Leave bucket on table"), overviewFptr, detailedFptr);
    rotateDegrees(-10, overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 2, std::string("Normalize before window section"), overviewFptr, detailedFptr, 130, 40);
    armServo.SetDegree(40);


    


    
    

    // Window
    Sleep(1.0);
    SD.FPrintf(overviewFptr, "\n-- Begin Window section --\n\n\n");
    moveVectorDistance(0, 6, 3, std::string("Normalize into right wall"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 8, std::string("Move closer to window"), overviewFptr, detailedFptr);
    rotateDegrees(90, overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 8, std::string("Back closer to window"), overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 7, std::string("Go left closer to window"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 8, std::string("Back into window"), overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 7, std::string("Open window"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 1.75, std::string("Inch forward out of window"), overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 1.25, std::string("Inch left of window"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 1.25, std::string("Inch back to window"), overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 6.65, std::string("Close window"), overviewFptr, detailedFptr);

    // Red/Blue button
    SD.FPrintf(overviewFptr, "\n-- Begin Red/Blue button section --\n\n\n");
    moveVectorDistance(0, -6, 2, std::string("Inch backward to normalize against window"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 11.3, std::string("Inch forward out of window, align with black stripe"), overviewFptr, detailedFptr);
    rotateDegrees(90, overviewFptr, detailedFptr);
    minCDS = 3.0;
    moveVectorDistance(0, 3, 5, std::string("Inch forward to find light"), overviewFptr, detailedFptr);
    std::string color = "nothing";
    if (minCDS < redMax){
        //Red light seen
        SD.FPrintf(overviewFptr, "\nRed light seen!\n\n");
        color = "red";
        LCD.SetBackgroundColor(RED);
        LCD.Clear();

        moveVectorDistance(3, 0, 1.25, std::string("Shift right for red button"), overviewFptr, detailedFptr);
    }
    else{
        //Go for blue
        if(minCDS < blueMax){
            SD.FPrintf(overviewFptr, "\nBlue light seen!\n\n");
            LCD.SetBackgroundColor(BLUE);
            LCD.Clear();
        }
        else{
            SD.FPrintf(overviewFptr, "\nNo light seen!\n\n");
            LCD.SetBackgroundColor(0xff00ff);
            LCD.Clear();
        }
        color = "blue";
        
        moveVectorDistance(-3, 0, 1.25, std::string("Shift left for blue button"), overviewFptr, detailedFptr);
    }
    moveVectorDistance(0, 6, 7, std::string("Go to touch button"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 7, std::string("Unpress button"), overviewFptr, detailedFptr);
    if (color == "red")
    {
        moveVectorDistance(-3, 0, 1.5, std::string("Shift left to middle"), overviewFptr, detailedFptr);
    }
    else
    {
        moveVectorDistance(3, 0, 1.5, std::string("Shift right to middle"), overviewFptr, detailedFptr);
    }
    Sleep(5.0);
    
    // Lever flip
    SD.FPrintf(overviewFptr, "\n-- Begin Lever flip section --\n\n\n");
    int lever = RCS.GetLever();
    rotateDegrees(-135, overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 3, std::string("Move to lever 0"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 3 * lever, std::string("Move to lever 1 or 2"), overviewFptr, detailedFptr);
    moveVectorDistance(-6, 0, 8, std::string("Move closer to lever"), overviewFptr, detailedFptr);
    // Servo mortor flip
    // Move backward
    // Lower the arm
    // Move forward
    // Raise the arm
    moveVectorDistance(6, 0, 5, std::string("Move away from lever"), overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 3 * lever, std::string("Move to lever 1 or 2"), overviewFptr, detailedFptr);
    moveVectorDistance(6, 0, 14, std::string("Move away from lever"), overviewFptr, detailedFptr);
    rotateDegrees(135, overviewFptr, detailedFptr);


    // Return to button
    moveVectorDistance(0, -6, 17, std::string("Back into right wall"), overviewFptr, detailedFptr);
    moveVectorDistance(0, 6, 2, std::string("Get away from wall"), overviewFptr, detailedFptr);
    rotateDegrees(-90, overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 34, std::string("Back down the ramp"), overviewFptr, detailedFptr);
    rotateDegrees(45, overviewFptr, detailedFptr);
    moveVectorDistance(0, -6, 3, std::string("Back into the end button"), overviewFptr, detailedFptr);

    // Done!

    for(int i = 0; i < 3; ++i){
        motor[i].Stop();
    }

    SD.FClose(overviewFptr);
    SD.FClose(detailedFptr);
    return 0;
}