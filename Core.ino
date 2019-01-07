//AICD > , automate integrer dans le systeme Kara
// HI project [For Kara]
// Author: S@n1x-d4rk3r [https://github.com/Sanix-Darker]

//#include <ArduinoJson.h>

#include <AFMotor.h>

AF_DCMotor motor1(1);  //motor1 is the left motor
AF_DCMotor motor2(2);  //motor2 is the right motor

// The automate mode if it's true, the car will drive itself otherhise it will wait fo commands
bool AUTO = true;

// Command for Remote control
String command;

// Sensor variable
//collects data from an analog sensor
int sensorpin = 0;                    // analog pin used to connect the sharp sensor
int val = 0;                          // variable to store the values from sensor(initially zero)

void setup()
{
  Serial.begin(9600);                 // starts the serial monitor
  
  motor1.setSpeed(255); //motor speed is set
  motor2.setSpeed(255);
  Stop();
}
 
void loop()
{

    // Checking the mode
    if(AUTO){ // in Automatik mode

      // Waiting for a course to start the driving
//      DynamicJsonBuffer jsonBuffer;
//      delay(300);
//      JsonObject& course = jsonBuffer.parseObject(getCourse());

      Serial.println("AICD >  Starting the course...");
//      StaticJsonBuffer<200> jsonBuffer;
//      JsonArray& course = jsonBuffer.parseArray(getCourse());
      const int array_x[15] = {1,0,-1,1,0,-1,0,1,1,-1,1,-1,0,0,0};
      const int array_y[15] = {-1,1,0,0,1,0,1,-1,0,0,0,1,1,0,-1};
      const int count = sizeof(array_y); 
      for (int i = 0; i < count; i++){
        Serial.print("array_y[i]: ");
        Serial.println(array_y[i]);

        Serial.print("array_x[i]: ");
        Serial.println(array_x[i]);
        
        if(array_x[i] > 0){
        
          if(array_y[i] > 0){

            check_and_go_right();
            
          }else if(array_y[i] < 0){
            
            check_and_go_back_right(); // go right back ==> macharriere en allant vers la droite
          
          }else{

            check_and_go_several_right();// ultimate right
          
          }
          
        }else if(array_x[i] < 0){

          if(array_y[i] > 0){

            check_and_go_left();
            
          }else if(array_y[i] < 0){
            
            check_and_go_back_left(); // go left back ==> macharriere en allant vers la droite
          
          }else{
            check_and_go_several_left(); // ultimate right
          }
          
        }else{
          
          if(array_y[i] > 0){

            check_and_go_forward();
            
          }else if(array_y[i] < 0){
            
            check_and_go_backward(); // go right back ==> macharriere en allant vers la droite
          
          }else{
            Stop();  // Stop the engine
          } 
        }
      }
      
      Stop();
      Serial.println("Destination reached!");
      delay(150000);
    }else{ // in Maual mode

      // Waiting for a command to work and operate
      remoteControl();
    }
   
  delay(300);                         // wait for this much time before printing next value
}


void check_and_go_backward(){
    if(checkObstacleGo_or_Stop()){
      go_backward();
    }else{
     Stop(); 
    }  
}

void check_and_go_forward(){
    if(checkObstacleGo_or_Stop()){
      go_forward();
    }else{
     Stop(); 
    }  
}


void check_and_go_right(){
    if(checkObstacleGo_or_Stop()){
      go_right();
    }else{
     Stop(); 
    }  
}

void check_and_go_left(){
    if(checkObstacleGo_or_Stop()){
      go_left();
    }else{
     Stop(); 
    }  
}

void check_and_go_several_right(){
    if(checkObstacleGo_or_Stop()){
      go_several_right();
    }else{
     Stop(); 
    }  
}

void check_and_go_several_left(){
    if(checkObstacleGo_or_Stop()){
      go_several_left();
    }else{
     Stop(); 
    }  
}

void check_and_go_back_right(){
    if(checkObstacleGo_or_Stop()){
      go_back_right();
    }else{
     Stop(); 
    }  
}

void check_and_go_back_left(){
    if(checkObstacleGo_or_Stop()){
      go_back_left();
    }else{
     Stop(); 
    }  
}


/**
 * checkObstacleGo_or_Stop 
 */
bool checkObstacleGo_or_Stop(){
  if(detectProximity())               // if detect an obstacle, do an action
  {
    int num_try = 0;
    while(!avoidObstacle() && num_try < 3){
      Serial.println("The Road look blocking...");
      num_try = num_try + 1;
      Serial.println("AICD >  calculating a solution...");
    }
    if(num_try >= 3){
      Serial.println("AICD >  can not continious!");
      return false;
    }else{
      return true;  
    }
  }else{ // No obstacle continue the process
    return true;
  }
}

// /*
//  * checkCourse This method get the course
//  */
// String getCourse()
// {
//   // this method is to get a programmated course
//   return "[{\"x\":0, \"y\":1}, {\"x\":0, \"y\":0}, {\"x\":1, \"y\":0}, {\"x\":0, \"y\":0}]";
// }

// Just to switch mode from auto to maual or viseversat
void switchMode(bool wantAuto){
  AUTO = wantAuto;  
}

/**
 * detectProximity to detect proximity
 */
bool detectProximity()
{
  val = analogRead(sensorpin);        // reads the value of the sharp sensor
  Serial.println(val);                // prints the value of the sensor to the serial monitor
  return val > 0 ? false : false;
}

/**
 * go_forward to make the car drive normally
 */
void go_forward()
{
  Serial.println("Going forward ....");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  delay(100);
}

/**
 * go_forward to make the car go back
 */
void go_backward()
{
  Serial.println("Going backward ....");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  delay(100);
}

/**
 * go_left to make the car go left
 */
void go_left()
{
  Serial.println("Going to left ...");
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(100);
}

/**
 * go_right to make the car go right
 */
void go_right()
{
  Serial.println("Going to right ...");
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  delay(100);
}


/**
 * go_several_left to make the car go left
 */
void go_several_left()
{
  Serial.println("Going several to left ...");
  //motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(100);
}

/**
 * go_several_right to make the car go right
 */
void go_several_right()
{
  Serial.println("Going several to right ...");
  motor1.run(FORWARD);
  //motor2.run(BACKWARD);
  delay(100);
}


/**
 * go_left to make the car go left
 */
void go_back_left()
{
  Serial.println("Going back to left ...");
  go_backward();
  //motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  delay(100);
}

/**
 * go_right to make the car go right
 */
void go_back_right()
{
  Serial.println("Going back to right ...");
  go_backward();
  motor1.run(BACKWARD);
  //motor2.run(BACKWARD);
  delay(100);
}


/**
 * Stop to stop the car
 */
void Stop()
{
  Serial.println("AICD >  Stop the engine.");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(100);
}

void avoidOnLeft()
{
    go_backward();
    go_left();
    delay(200);
}

void avoidOnRight()
{
    go_backward();
    go_right();
    delay(200);
}

/**
 * avoidObstacle this method is to avoid obstacle on the road
 */
bool avoidObstacle()
{
  // go back
  go_backward();
  int try_for_left = 0;
  int try_for_right = 0;
  bool openRoad = false;
  
  // try a random avoid way
  if(random(300) > 150){
    avoidOnLeft();
  }else{
    avoidOnRight();
  }

  // Check if the road is available
  if(detectProximity()){

    while(detectProximity()){
      
      if(try_for_left < 5){
        
        go_backward();
        go_left();
        try_for_left = try_for_left + 1;
  
      }else{
        
        if(try_for_right == 0){
          
          for (int i = 0; i < 7; i++){
            
            if(detectProximity()){
              
              go_backward();
              go_right();
              try_for_right = try_for_right + 1;
              
            }else{
              openRoad = true;
              break;
            }
          }
    
        }else{
          break;  
        }
      }
      if(openRoad){
        break;
      }
    }

  }else{
    openRoad = true;
  }
  
  return openRoad;
}

void remoteControl()
{
//    command = Serial.read();
//    // Remote control MODE
//    if(command=='1')  //when the bluetooth module recieves 1 the car moves forward
//    {
//     go_forward(); 
//    }
//    if(command=='2')  //when the bluetooth module recieves 2 the car moves backward
//    {
//     go_backward(); 
//    }
//    if(command=='3') //when the bluetooth module recieves 3 the car moves left
//    {
//     go_left(); 
//    }
//    if(command=='4')  //when the bluetooth module recieves 4 the car moves right
//    {
//     go_right(); 
//    }
//    if(command=='5') //when the bluetooth module recieves 5 the car stops
//    {
//     Stop(); 
//    }  
}
