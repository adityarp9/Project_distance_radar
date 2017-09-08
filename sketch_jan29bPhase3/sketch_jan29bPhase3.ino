/*   Distance finder + Radar project
 *      
 *  by Aditya R. Patgaonkar  
 *  LinkedIn profile : https://www.linkedin.com/in/aditya-patgaonkar-9aa657113
 *  & Patanjali Dwivedi
 *  LinkedIn profile : https://www.linkedin.com/in/vidyutdwivedi
 
 *  with some references from Dejan Nedelkovski, 
 *  www.HowToMechatronics.com
 *  
 */

#include <dht.h>
// Includes the Servo library
#include <Servo.h>

#define trigPin 9
#define echoPin 13
#define DHT11_PIN 3
int led = 7, piezoPin = 11, servoPin = 5, laser = 2; // Led, buzzer pin & servo pin.

// Variables for the duration and the distance
int distance, thDistance = 7; 
long duration;
float speed;
Servo myServo; // Creates a servo object for controlling the servo motor
dht DHT; // Temperature & humidity sensor

void setup() 
{
  pinMode(laser, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  myServo.attach(servoPin); // Defines on which pin is the servo motor attached
}
void loop() 
{
  // rotates the servo motor from 0 to 180 degrees
  for(int angle=0;angle<=180;angle++)
  {  
    myServo.write(angle);
    delay(20);
    distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
    Serial.print(angle); // Sends the current degree into the Serial Port
    Serial.print(" "); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distance); // Sends the distance value into the Serial Port
    Serial.print("\n"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    while (distance < thDistance) // Anomaly to fix. Update: Fixed!
    {
      distance = calculateDistance();
      Serial.print(angle); // Sends the current degree into the Serial Port
      Serial.print(" "); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
      Serial.print(distance); // Sends the distance value into the Serial Port
      Serial.print("\n"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing    
      digitalWrite(led, HIGH);
      digitalWrite(laser, HIGH);
      if (distance < thDistance) // avoid distortion
        tone(piezoPin, distance * 1000, 200);
      delay(1500);
    } 
    digitalWrite(laser, LOW);
    digitalWrite(led, LOW);
    
  }
  // Repeats the previous lines from 165 to 15 degrees
  for(int angle=180;angle>0;angle--)
  {  
    myServo.write(angle);
    delay(20);
    distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
    Serial.print(angle); // Sends the current degree into the Serial Port
    Serial.print(" "); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distance); // Sends the distance value into the Serial Port
    Serial.print("\n"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    while (distance < thDistance) // Anomaly to fix. Update: Fixed!
    {
      distance = calculateDistance();    
      /*Serial.print("Distance = ");
      Serial.print(d);
      Serial.println(" cm");*/
      digitalWrite(led, HIGH);
      digitalWrite(laser, HIGH);
      if (distance < thDistance) // avoid distortion
        tone(piezoPin, distance * 1000, 200);
      delay(1500);
    } 
    digitalWrite(laser, LOW);
    digitalWrite(led, LOW);
  }
}

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance()
{ 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  speed = 331.4 + (0.606 * DHT.temperature) + (0.0124 * DHT.humidity);
  distance = (duration / 2) * (speed / 10000);
  return distance;
}
