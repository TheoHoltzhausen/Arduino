int CWangle = 90; //Angle to rotate clockwise from center position in degrees
int CCWangle = 82; //Angle to rotate counterclockwise from center position in degrees
int Accel = 179; //Acceleration in steps per second squared

int PulPin = 12; //Define pin for pulse output
int DirPin = 13; //Define pin for direction output
int HomePin = 10; //Define pin for home/run condition

float time1; //Define variable for cycle start time
float time2; //Define variable for cycle end time
float cycle; //Define variable for cycle rate
float CWsteps; //Define variable for clockwise number of steps
float CCWsteps; //Define variable for counterclockwise number of steps

#include <AccelStepper.h> //Import stepper library
AccelStepper stepper(AccelStepper::DRIVER, PulPin, DirPin); //Define stepper object and pins

void setup() {

  Serial.begin(9600);

  pinMode(HomePin, INPUT); //Pin connected to switch to select home or run mode

  CWsteps = round((1600./360.)*CWangle); //Convert clockwise angle to number of steps
  CCWsteps = round((1600./360.)*CCWangle); //Convert counterclockwise angle to number of steps
  
  stepper.setAcceleration(Accel); //Set acceleration rate
  stepper.setMaxSpeed(50000); //Set max speed to an unreachable value (to create triangular motion profile)
  stepper.moveTo(CWsteps); //Set iniial movement to clockwise position
}

void loop() {
  
  //If the stepper reached it's counterclockwise position, start moving to clockwise position:
  if (stepper.distanceToGo()==0 && stepper.currentPosition()==-CCWsteps){
    stepper.moveTo(CWsteps);
    time1 = millis(); //Cycle begin time
  }

  //If the stepper reached it's clockwise position, start moving to counterclockwise position:
  if (stepper.distanceToGo()==0 && stepper.currentPosition()==CWsteps) {
    stepper.moveTo(-CCWsteps);
    time2 = millis(); //Cycle end time
    cycle = abs(60000./(time2-time1)); //Calculate cycles per minute
    Serial.print("The number of cycles per minute is ");
    Serial.println(cycle); //Print cycles per minute on serial window
  }
  
  //If selector switch is set to home, move to center position:
  if (digitalRead(HomePin) == HIGH) {
      stepper.moveTo(0);
  }
  
  //If selector switch is set to run and the target position is set to zero, change target position to clockwise position
  if (digitalRead(HomePin) == LOW && stepper.targetPosition() == 0) {
      stepper.moveTo(CWsteps);
  }

  stepper.run();
  
}
