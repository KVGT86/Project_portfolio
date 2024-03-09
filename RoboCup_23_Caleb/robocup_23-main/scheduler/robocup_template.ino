
/********************************************************************************
 *                               ROBOCUP TEMPLATE                              
 *        
 *  
 *  This is a template program design with modules for 
 *  different components of the robot, and a task scheduler
 *  for controlling how frequently tasks sholud run
 *  
 *  
 *  written by: Logan Chatfield, Ben Fortune, Lachlan McKenzie, Jake Campbell
 *  
 ******************************************************************************/

#include <Servo.h>                  //control the DC motors
//#include <Herkulex.h>             //smart servo
#include <Adafruit_TCS34725.h>      //colour sensor
#include <Wire.h>                   //for I2C and SPI
#include <TaskScheduler.h>          //scheduler 
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
// Custom headers
#include "motors.h"
#include "sensors.h"
#include "ultrasonic.hpp"
#include "weight_collection.h"
#include "return_to_base.h" 
#include "IMU.h"

//**********************************************************************************
// Local Definitions
//**********************************************************************************

// Task period Definitions
// ALL OF THESE VALUES WILL NEED TO BE SET TO SOMETHING USEFUL !!!!!!!!!!!!!!!!!!!!
#define US_READ_TASK_PERIOD                 40
// #define IR_READ_TASK_PERIOD                 40
//#define COLOUR_DETECTION_TASK_PERIOD        1000
// #define SENSOR_AVERAGE_PERIOD               40
#define SET_MOTOR_TASK_PERIOD               40
// #define WEIGHT_SCAN_TASK_PERIOD             40
#define COLLECTION_CONTROL_TASK_PERIOD      100
// #define RETURN_TO_BASE_TASK_PERIOD          40
// #define DETECT_BASE_TASK_PERIOD             40
// #define UNLOAD_WEIGHTS_TASK_PERIOD          40
// #define CHECK_WATCHDOG_TASK_PERIOD          40
// #define VICTORY_DANCE_TASK_PERIOD           40
// #define SET_IMU_TASK_PERIOD                 40




// Task execution amount definitions
// -1 means indefinitely
#define US_READ_TASK_NUM_EXECUTE           -1
#define IR_READ_TASK_NUM_EXECUTE           -1
//#define COLOUR_DETECTION_TASK_NUM_EXECUTE       -1
#define SENSOR_AVERAGE_NUM_EXECUTE         -1
#define SET_MOTOR_TASK_NUM_EXECUTE         -1
#define WEIGHT_SCAN_TASK_NUM_EXECUTE       -1
#define COLLECTION_CONTROL_TASK_NUM_EXECUTE -1
#define RETURN_TO_BASE_TASK_NUM_EXECUTE    -1
#define DETECT_BASE_TASK_NUM_EXECUTE       -1
#define UNLOAD_WEIGHTS_TASK_NUM_EXECUTE    -1
#define CHECK_WATCHDOG_TASK_NUM_EXECUTE    -1
#define VICTORY_DANCE_TASK_NUM_EXECUTE     -1
#define SET_IMU_TASK_NUM_EXECUTE           -1


// Pin deffinitions
#define IO_POWER  49

// Serial deffinitions
#define BAUD_RATE 9600




//**********************************************************************************
// Task Scheduler and Tasks
//**********************************************************************************

/* The first value is the period, second is how many times it executes
   (-1 means indefinitely), third one is the callback function */

// Tasks for reading sensors 
Task tRead_ultrasonic(US_READ_TASK_PERIOD,       US_READ_TASK_NUM_EXECUTE,        &read_ultrasonic);

// Task to set the motor speeds and direction
Task tSet_motor(SET_MOTOR_TASK_PERIOD,           SET_MOTOR_TASK_NUM_EXECUTE,      &set_motor);

//Task tSet_IMU(SET_IMU_TASK_PERIOD,           SET_IMU_TASK_NUM_EXECUTE,      &GetHeading);

// Tasks to scan for weights and collection upon detection
// Task tWeight_scan(WEIGHT_SCAN_TASK_PERIOD,       WEIGHT_SCAN_TASK_NUM_EXECUTE,    &weight_scan);
Task tCollection_control(COLLECTION_CONTROL_TASK_PERIOD, COLLECTION_CONTROL_TASK_NUM_EXECUTE, &collection_control);
//Task tColour_detection(COLOUR_DETECTION_TASK_PERIOD, COLOUR_DETECTION_TASK_NUM_EXECUTE, &colour_detection);

// Tasks to search for bases and unload weights
// Task tReturn_to_base(RETURN_TO_BASE_TASK_PERIOD, RETURN_TO_BASE_TASK_NUM_EXECUTE, &return_to_base);
// Task tDetect_base(DETECT_BASE_TASK_PERIOD,       DETECT_BASE_TASK_NUM_EXECUTE,    &detect_base);
// Task tUnload_weights(UNLOAD_WEIGHTS_TASK_PERIOD, UNLOAD_WEIGHTS_TASK_NUM_EXECUTE, &unload_weights);

// Tasks to check the 'watchdog' timer (These will need to be added in)
//Task tCheck_watchdog(CHECK_WATCHDOG_TASK_PERIOD, CHECK_WATCHDOG_TASK_NUM_EXECUTE, &check_watchdog);
//Task tVictory_dance(VICTORY_DANCE_TASK_PERIOD,   VICTORY_DANCE_TASK_NUM_EXECUTE,  &victory_dance);

Scheduler taskManager;

//**********************************************************************************
// Function Definitions
//**********************************************************************************
void pin_init();
void robot_init();
void task_init();

//**********************************************************************************
// put your setup code here, to run once:
//**********************************************************************************
void setup() {
  Serial.begin(BAUD_RATE);
  pin_init();
  robot_init();
  task_init();
  Wire.begin();
  motor_init();
  sensor_init();
  //IMU_init();
  weight_collection_initialisation();
}

//**********************************************************************************
// Initialise the pins as inputs and outputs (otherwise, they won't work) 
// Set as high or low
//**********************************************************************************
void pin_init(){
    
    Serial.println("Pins have been initialised \n"); 

    pinMode(IO_POWER, OUTPUT);              //Pin 49 is used to enable IO power
    digitalWrite(IO_POWER, 1);              //Enable IO power on main CPU board
}

//**********************************************************************************
// Set default robot state
//**********************************************************************************
void robot_init() {

    usCounterInit(); 
    // Add ultrasonic sensors to array

    Serial.println("Robot is ready \n");
}

//**********************************************************************************
// Initialise the tasks for the scheduler
//**********************************************************************************
void task_init() {  
  
  // This is a class/library function. Initialise the task scheduler
  taskManager.init();     
 
  // Add tasks to the scheduler
  taskManager.addTask(tRead_ultrasonic);   //reading ultrasonic 
  // taskManager.addTask(tRead_infrared);
  //taskManager.addTask(tColour_detection);
  // taskManager.addTask(tSensor_average);
  taskManager.addTask(tSet_motor); 
  // taskManager.addTask(tWeight_scan);
  taskManager.addTask(tCollection_control);
  // taskManager.addTask(tReturn_to_base);
  // taskManager.addTask(tDetect_base);
  // taskManager.addTask(tUnload_weights);
  //taskManager.addTask(tSet_IMU);
  //taskManager.addTask(tCheck_watchdog);
  //taskManager.addTask(tVictory_dance);      

  //enable the tasks
  tRead_ultrasonic.enable();
  // tRead_infrared.enable();
  //tColour_detection.enable();
  // tSensor_average.enable();
  tSet_motor.enable();
 // tSet_IMU.enable();
  // tWeight_scan.enable();
  tCollection_control.enable();
  // tReturn_to_base.enable();
  // tDetect_base.enable();
  // tUnload_weights.enable();
 //tCheck_watchdog.enable();
 //tVictory_dance.enable();

 Serial.println("Tasks have been initialised \n");
}



//**********************************************************************************
// put your main code here, to run repeatedly
//**********************************************************************************
void loop() {
  
  taskManager.execute();    //execute the scheduler

}
