#include <

#define SENSOR_LO 
#define SENSOR_LI
#define SENSOR_M
#define SENSOR_RI
#define SENSOR_RO
#define MOTOR_L
#define MOTOR_R

//CHANGE THESE TO GLOBAL VARIABLES DUE TO #DEFINES NOT BEING ABLE TO BE CHANGED
bool sensor_LO_o = false;
bool sensor_LI_o= false;
bool sensor_RO_o= false;
bool sensor_RI_o= false;
bool sensor_M_o= false;
int motor_slow= 50; //0=0%duty cycle 255=100% dutycycle
int motor_fast= 150; //0=0%duty cycle 255=100% dutycycle
int motor_max=255 ; //0=0%duty cycle 255=100% dutycycle
int flag_left= 0;
int flag_right = 0;
int sensor_limit = ;


void setup() {
  // put your setup code here, to run once:
  pinmode(SENSOR_LO, INPUT);
  pinmode(SENSOR_LI, INPUT);
  pinmode(SENSOR_M, INPUT);
  pinmode(SENSOR_RI, INPUT);
  pinmode(SENSOR_RO, INPUT);
  pinmode(MOTOR_L, OUTPUT);
  pinmode(MOTOR_R, OUTPUT);
  
}

void read_sensors(void)
{
  if (analogRead(SENSOR_LO) > sensor_limit) {
      sensor_LO_o = true;
  } else {
      sensor_LO_o = false;
  }
  if (analogRead(SENSOR_LI) > sensor_limit) {
      sensor_LI_o = true;
  } else {
      sensor_LI_o = false;
  }
  if (analogRead(SENSOR_M) > sensor_limit) {
      sensor_M_o = true;
  } else {
     sensor_M_o = false;
  }
  if (analogRead(SENSOR_RI) > sensor_limit) {
      sensor_RI_o = true;
  } else {
     sensor_RI_o = false;
  }
  if (analogRead(SENSOR_RO) > sensor_limit) {
      sensor_RO_o = true;
  } else {
     sensor_RO_o = false;
  }

}

void sensor_middle(void)
{
  // if middle sensor on the line and no other sensors are activated
  if (sensor_M_o && !(sensor_LO_o || sensor_LI_o || sensor_RI_o || sensor_RO_o)) {
    // set motor to max speed
    analogWrite(MOTOR_L, motor_max);
    analogWrite(MOTOR_R, motor_max);
  }
  // if no sensors on the line & flag activated (robot not on line at all)
  if (!(sensor_M_o || sensor_LO_o || sensor_LI_o || sensor_RI_o || sensor_RO_o)) {
    // check which flag is activated
    if (flag_left) {
      analogWrite(MOTOR_L, motor_slow);
    } else if (flag_right) {
      analogWrite(MOTOR_R, motor_slow);
    }
  }
}

void sensor_left(void) 
{
  if (sensor_LO_o ) {
      analogWrite(MOTOR_L, motor_slow);
      flag_left = 1;
  }
  if (sensor_LI_o && !(sensor_LO_o)) {
      analogWrite(MOTOR_L, motor_fast);
  }
}
void sensor_right(void)
{
  if (sensor_RO_o) {
    analogWrite(MOTOR_R, motor_slow);
    flag_right = 1;
  }
  if (sensor_RI_o && !(sensor_RO_o)) {
    analogWrite(MOTOR_R, motor_fast);
  }
}
void reset_flags(void)
{
  // reset flags if any other sensors activated 
  if (flag_left == 1) {
    if (sensor_LI_o || sensor_RI_o || sensor_RO_o || sensor_M_o){
      flag_left = 0;
    }
  }
  if (flag_right == 1) {
    if (sensor_LI_o || sensor_RI_o || sensor_LO_o || sensor_M_o) {
      flag_right = 0;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  read_sensors();
  sensor_left();
  sensor_right();
  sensor_middle();
  reset_flags();
  // maybe wait?
}
