#include <Servo.h>
#include <PID_v1.h>

const int servoPin = 9;
const int TrigPin = 11;
const int EchoPin = 12;


// i/p = distance from sensor aka current position
// o/p = calculated angle adjustment for the servo
// setpoint = required center position
double Input, Output, Setpoint;

double Kp = 2.5; 
double Ki = 0.0;
double Kd = 2.0; 

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

Servo myServo;

const int target_position = 15; 
const int baseServoAngle = 90; 
const int maxTilt = 30; 

float readPosition() {

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  long duration = pulseIn(EchoPin, HIGH, 30000); 
  if (duration == 0) {
      return 1; 
  }
  
  float distanceCm = duration * 0.034 / 2.0; 

  if (distanceCm > 30.0 || distanceCm < 1.0) {
      return Setpoint; // to ignore garbage readings
  }
  
  return distanceCm;
}


void setup() {
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  myServo.attach(servoPin);
  Setpoint = target_position;
  myPID.SetOutputLimits(-maxTilt, maxTilt); 
  myPID.SetMode(AUTOMATIC);
  myServo.write(baseServoAngle);
  myPID.SetSampleTime(20); 

  Serial.println("Ball and Beam PID System Initialized.");
  Serial.println("--- Reading: Position (cm) | Error | Output (Tilt) | Servo Angle ---");
}


void loop() {
  Input = readPosition();
  myPID.Compute();
  int servoAngle = baseServoAngle + Output;
  
  if (servoAngle < 0) {
    servoAngle = 0;
  }
  if (servoAngle > 180) {
    servoAngle = 180;
  }
  
  myServo.write(servoAngle);
  
  Serial.print("Position (cm): "); Serial.print(Input);
  Serial.print(" | Error: "); Serial.print(Setpoint - Input);
  Serial.print(" | Output (Tilt): "); Serial.print(Output);
  Serial.print(" | Servo Angle: "); Serial.println(servoAngle);

  delay(500); 
}