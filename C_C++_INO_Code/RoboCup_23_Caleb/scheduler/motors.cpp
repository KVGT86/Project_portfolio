// #include "motors.h"
// #include "Arduino.h"
// #include "sensors.h"
// #include <Servo.h>

// /* In this section, the motor speeds should be updated/written.
//  *It is also a good idea to check whether value to write is valid.
//  *It is also a good idea to do so atomically!
//  */

// Servo driveL; 
// Servo driveR; 

// void motor_init() {
//   driveL.attach(0);
//   driveR.attach(1);
  
// }

// void GetDir(){
//   direction = PassDir();
// }

// void set_motor() {

//   if (direction == 0) {
//       Serial.print("turn left");
//       driveL.writeMicroseconds(1150); //pivot left
//       driveR.writeMicroseconds(1950);
//   } else if (direction == 1) {
//       driveL.writeMicroseconds(1950); //Forwards
//       driveR.writeMicroseconds(1950);
//   } else if (direction == 2) {
//       Serial.print("turn right");
//       driveL.writeMicroseconds(1950); //pivot right
//       driveR.writeMicroseconds(1050);
//   } else if (direction == 3) {
//       Serial.print("reverse");
//       driveL.writeMicroseconds(1050); //reverse
//       driveR.writeMicroseconds(1050);
//       delay(1000);                      //replace with a big if statement for (time + 1000) 
//   }
  
//   Serial.printf("\n");
//   Serial.printf("Direction is: %d\t", direction);
// }

#include "motors.h"
#include "Arduino.h"
#include "sensors.h"
#include <Servo.h>

#define MAX_SPEED 1950
#define MIN_SPEED 1050

/* In this section, the motor speeds should be updated/written.
 *It is also a good idea to check whether value to write is valid.
 *It is also a good idea to do so atomically!
 */

// Define PID Constants
double KP = 0.87; //0.8

double KI = 0.01;

double KD = 0.1;
double MOTOR_TOLERANCE = 0.1;

// Initialize Variables

uint16_t TARGET_POSITION_ARR[2] = {0, 0};
double CURRENT_POSITION_L = 1500;
double CURRENT_POSITION_R = 1500;
double L_PREVIOUS_ERROR = 0;
double L_INTEGRAL = 0;
double R_PREVIOUS_ERROR = 0;
double R_INTEGRAL = 0;
int PREV_DIRECTION = 1;

Servo driveL; 
Servo driveR; 

void motor_init() {
  driveL.attach(0);
  driveR.attach(1);
  
}

void GetDir(){
  direction = PassDir();
}

//************ NEED TO LOOP THIS SOMEHOW - done via fast scheuling
void set_motor() {            //for reverse and sharp turn must set left motor before right
  set_target();
  set_left_motor();
  set_right_motor();
}

void set_target()
{  
  if (direction != PREV_DIRECTION) {
    L_INTEGRAL = 0;
    R_INTEGRAL = 0;
  }
  PREV_DIRECTION = direction;
  switch (direction) {
    case 0:
      // Set target position for a left turn
      TARGET_POSITION_ARR[0] = MIN_SPEED;
      TARGET_POSITION_ARR[1] = MAX_SPEED;
      break;

    case 1:
      // Set target position for forwards
      TARGET_POSITION_ARR[0] = MAX_SPEED;
      TARGET_POSITION_ARR[1]= MAX_SPEED;
      break;

    case 2:
      // Set target position for a right turn
      TARGET_POSITION_ARR[0] = MAX_SPEED;
      TARGET_POSITION_ARR[1] = MIN_SPEED;
      break;

    case 3:
      // Set target position for reverse
      TARGET_POSITION_ARR[0] = MIN_SPEED;
      TARGET_POSITION_ARR[1] = MIN_SPEED;
      break;

    case 4:
      // Set target position for a left turn
      TARGET_POSITION_ARR[0] = MIN_SPEED;
      TARGET_POSITION_ARR[1] = MAX_SPEED;
      break;

    case 5:
      // Set target position for a right turn
      TARGET_POSITION_ARR[0] = MAX_SPEED;
      TARGET_POSITION_ARR[1] = MIN_SPEED;
      break;

  }
}

void set_left_motor_lims()
{
  if (CURRENT_POSITION_L < 1050) {
    CURRENT_POSITION_L = 1050;
  } else if (CURRENT_POSITION_L > 1950) {
    CURRENT_POSITION_L = 1950;
  }
}

void set_right_motor_lims()
{
  if (CURRENT_POSITION_R < 1050) {
    CURRENT_POSITION_R = 1050;
  } else if (CURRENT_POSITION_R > 1950) {
    CURRENT_POSITION_R = 1950;
  }
}


void set_left_motor() {
  // Calculate Error
  uint16_t target_position_L = TARGET_POSITION_ARR[0];//!!!!!!!!!!!
  double error = target_position_L - CURRENT_POSITION_L;

  // Calculate PID 
  double P = KP * error;
  L_INTEGRAL += KI * error;
  double D = KD * (error - L_PREVIOUS_ERROR);

  // Calculate PID Output
  double output = P + L_INTEGRAL + D;
  
  if (direction == 0) {
    // Left turn
    CURRENT_POSITION_L = MIN_SPEED + output;
    set_left_motor_lims();
    driveL.writeMicroseconds(CURRENT_POSITION_L); //pivot left
  } else if (direction == 1) {
    // Forwards
    CURRENT_POSITION_L = MAX_SPEED + output;
    set_left_motor_lims();
    set_left_motor_lims();
    driveL.writeMicroseconds(CURRENT_POSITION_L); //Forwards
  } else if (direction == 2) {
    // Right turn
    CURRENT_POSITION_L = MAX_SPEED + output;
    set_left_motor_lims();
    driveL.writeMicroseconds(CURRENT_POSITION_L); //pivot right
  } else if (direction == 3) {
    // Reverse
    CURRENT_POSITION_L = MIN_SPEED;
    set_left_motor_lims();
    driveL.writeMicroseconds(CURRENT_POSITION_L); //reverse
  } else if (direction == 4) {
    // Sharp left
    CURRENT_POSITION_L = MIN_SPEED;
    set_left_motor_lims();
    driveL.writeMicroseconds(CURRENT_POSITION_L); //reverse
  } else if (direction == 5) {
    // Sharp right
    CURRENT_POSITION_L = MAX_SPEED;
    set_left_motor_lims();
    driveL.writeMicroseconds(CURRENT_POSITION_L); //reverse
  }

  // Error for the next iteration
  L_PREVIOUS_ERROR = error;
  Serial.println("Left Motor: ");
  Serial.println(CURRENT_POSITION_L);
  Serial.println(P);
  Serial.println(L_INTEGRAL);
  Serial.println(D);
}

void set_right_motor() {
  // Calculate Error
  uint16_t target_position_R = TARGET_POSITION_ARR[1];

  double error = target_position_R - CURRENT_POSITION_R;

  // Calculate PID 
  double P = KP * error;
  R_INTEGRAL += KI * error;
  double D = KD * (error - R_PREVIOUS_ERROR);

  // Calculate PID Output
  double output = P + R_INTEGRAL + D;
  
  if (direction == 0) {
    // Left turn
    CURRENT_POSITION_R = MAX_SPEED + output;
    set_right_motor_lims();
    driveR.writeMicroseconds(CURRENT_POSITION_R);
  } else if (direction == 1) {
    // Forwards
    CURRENT_POSITION_R = MAX_SPEED + output;
    set_right_motor_lims();
    set_right_motor_lims();
    driveR.writeMicroseconds(CURRENT_POSITION_R);
  } else if (direction == 2) {
    // Right turn
    CURRENT_POSITION_R = MIN_SPEED + output;
    set_right_motor_lims();
    driveR.writeMicroseconds(CURRENT_POSITION_R);
  } else if (direction == 3) {
    // Reverse
    CURRENT_POSITION_R = MIN_SPEED;
    set_right_motor_lims();
    driveR.writeMicroseconds(CURRENT_POSITION_R);
    delay(1000); //replace with a big if statement for (time + 1000)
  } else if (direction == 4) {
    // Sharp Left
    CURRENT_POSITION_R = MAX_SPEED;
    set_right_motor_lims();
    driveR.writeMicroseconds(CURRENT_POSITION_R);
    Serial.println(4);
    delay(1250); //replace with a big if statement for (time + 1000)
  } else if (direction == 5) {
    // Sharp Right
    CURRENT_POSITION_R = MIN_SPEED;
    set_right_motor_lims();
    driveR.writeMicroseconds(CURRENT_POSITION_R);
    Serial.println(5);
    delay(900); //replace with a big if statement for (time + 1000)
  }

  // Error for the next iteration
  R_PREVIOUS_ERROR = error;
  Serial.println("Right Motor: ");
  Serial.println(CURRENT_POSITION_R);
  Serial.println(P);
  Serial.println(R_INTEGRAL);
  Serial.println(D);
}

